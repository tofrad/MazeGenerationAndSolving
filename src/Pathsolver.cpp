#include "Pathsolver.hpp"
#include <queue>

Pathsolver::Pathsolver()
{

}

Pathsolver::Pathsolver(Cell* start, const SolvingMethod method, Recorder* recorder)
{
	path_record = recorder;
	solveMaze(start, method);
}

Pathsolver::~Pathsolver()
{

}

bool Pathsolver::solveMaze(Cell* start, const SolvingMethod method) const
{
	//start recording
	path_record->startRecording();

	bool is_solved = false;

	//solve Maze
	switch (method) {

		case SM_DFS:
			is_solved = DFS(start);
			break;
		case SM_BFS:
			is_solved = BFS(start);
			break;

	}
	//stop Recording
	path_record->stopRecording();

	return is_solved;
}

Recorder* Pathsolver::getRecording() const
{
	return path_record;
}

bool Pathsolver::isVisitable(const Cell* cell)
{
	if (cell != nullptr && !cell->path_next_flags.Path_CellWasVisited) {
		return true;
	}else{
		return false;
	}
}

//TODO
// Update recording of DFS
//flickering, marking of leading cell
bool Pathsolver::DFS(Cell* start) const
{
	//abort if target is found
	if (start->maze_next_flags.isTarget) {
		return true;
	}
	
	//mark and record cell as visited
	if (!start->path_next_flags.Path_CellWasVisited) {
		start->path_next_flags.Path_CellWasVisited = true;
	}

	//set cell active and record it
	start->maze_next_flags.isActive = true;
	//record here#########################################################################################
	path_record->recordStep({start});
	
	//mark it as path, will be deleted in recursion
	start->path_next_flags.Path_IsCurrentPath = true;

	//add all valid adjacent to list
	vector<Cell*> adjCells;

	Cell* North = start->getNorth();
	Cell* East = start->getEast();
	Cell* South = start->getSouth();
	Cell* West = start->getWest();

	if (isVisitable(North)) {
		adjCells.push_back(North);
	}
	if (isVisitable(East)) {
		adjCells.push_back(East);
	}
	if (isVisitable(South)) {
		adjCells.push_back(South);
	}
	if (isVisitable(West)) {
		adjCells.push_back(West);
	}

	//go through all adj cells
	while (!adjCells.empty()) {

		//get next cell
		Cell* next = adjCells.back();

		//delete start from isActive and record it 
		start->maze_next_flags.isActive = false;
		//record here#########################################################################################
		path_record->recordStep({start});

		if (DFS(next)) {

			//recursion animation?
			start->path_next_flags.Path_IsFinishedPath = true;
			//next node is on path to target or is the target

			//record path cell
			path_record->recordStep({start});

			return true;
		}
		//delete cell from list
		adjCells.pop_back();
	}

	//cell is not on Path
	start->maze_next_flags.isActive = false;
	start->path_next_flags.Path_IsCurrentPath = false;
	//record here#########################################################################################
	path_record->recordStep({start});

	return false;
}

//TODO
// Update BFS
//add leading edges to frontier cells
bool Pathsolver::BFS(Cell* start) const
{
	queue<vector<Cell*>> tobevisited;

	start->path_next_flags.Path_CellWasVisited = true;
	tobevisited.push({start});

	bool wasTargetFound = false;
	Cell* foundTarget = nullptr;

	vector<Cell*> next_cells;

	while (!tobevisited.empty() && wasTargetFound == false) {

		vector<Cell*> current_cells = tobevisited.front();
		tobevisited.pop();

		next_cells.clear();

		if (!current_cells.empty()) {

			for (const auto cell : current_cells) {

				if (cell->maze_next_flags.isTarget) {
					wasTargetFound = true;
					foundTarget = cell;
					break;
				}
				
				Cell* Neighbors[4] = {
					cell->getNorth(),
					cell->getEast(),
					cell->getSouth(),
					cell->getWest()
				};
				
				for (Cell* neighbor : Neighbors) {

					if (neighbor && isVisitable(neighbor)) {

						neighbor->setParent(cell);
						neighbor->path_next_flags.Path_CellWasVisited = true;
						next_cells.push_back(neighbor);
					}
				}
			}
			// record current cells here###############################################################
			path_record->recordStep(current_cells);
			tobevisited.push(next_cells);
		}
	}

	if (wasTargetFound && foundTarget!= nullptr) {
		Cell* backtrack = foundTarget;

		while (!backtrack->maze_next_flags.isStart) {

			backtrack->path_next_flags.Path_IsFinishedPath = true;
			//record here####################################################################
			path_record->recordStep({backtrack});

			backtrack = backtrack->getParent();
		}
	}
	return wasTargetFound;
}
