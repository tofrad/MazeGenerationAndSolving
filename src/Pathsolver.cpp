#include "Pathsolver.hpp"
#include <queue>

Pathsolver::Pathsolver()
{

}

Pathsolver::Pathsolver(Cell* start, const SolvingMethod method, const Recorder& recorder)
{
	path_record = recorder;
	solveMaze(start, method);
}

Pathsolver::~Pathsolver()
{

}

void Pathsolver::solveMaze(Cell* start, const SolvingMethod method)
{
	//start recording
	path_record.startRecording();

	//solve Maze
	switch (method) {

		case SM_DFS:
			DFS(start);
			break;
		case SM_BFS:
			BFS(start);
			break;

	}
	//stop Recording
	path_record.stopRecording();
}

void Pathsolver::displayInitialFrame() const
{
	path_record.playInitialGrid();
}

Recorder* Pathsolver::getRecording()
{
	return &path_record;
}

bool Pathsolver::isVisitable(const Cell* cell)
{
	if (cell != nullptr && !cell->next_flags.pathVisited) {
		return true;
	}
	else {
		return false;
	}
}

bool Pathsolver::DFS(Cell* start)
{
	//abort if target is found
	if (start->next_flags.isTarget) {
		return true;
	}
	
	//mark and record cell as visited
	if (!start->next_flags.pathVisited) {
		start->next_flags.pathVisited = true;
	}

	//set cell active and record it
	start->next_flags.isActive = true;
	//record here#########################################################################################
	path_record.recordStep({start});
	
	//mark it as path, will be deleted in recursion
	start->next_flags.isPath = true;

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
		start->next_flags.isActive = false;
		//record here#########################################################################################
		path_record.recordStep({start});

		if (DFS(next)) {

			//recursion animation?
			start->next_flags.isfinishedPath = true;
			//next node is on path to target or is the target

			//record path cell
			path_record.recordStep({start});

			return true;
		}	

		//delete cell from list
		adjCells.pop_back();

	}

	//cell is not on Path
	start->next_flags.isActive = false;
	start->next_flags.isPath = false;
	//record here#########################################################################################
	path_record.recordStep({start});

	return false;
}

bool Pathsolver::BFS(Cell* start)
{
	queue<vector<Cell*>> tobevisited;

	start->next_flags.pathVisited = true;
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

				if (cell->next_flags.isTarget) {
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
						neighbor->next_flags.pathVisited = true;
						next_cells.push_back(neighbor);
					}
				}
			}
			// record current cells here###############################################################
			path_record.recordStep(current_cells);
			tobevisited.push(next_cells);
		}
	}

	if (wasTargetFound) {
		Cell* backtrack = foundTarget;

		while (!backtrack->next_flags.isStart) {

			backtrack->next_flags.isfinishedPath = true;
			//record here####################################################################
			path_record.recordStep({backtrack});

			backtrack = backtrack->getParent();
		}
	}
	return wasTargetFound;
}
