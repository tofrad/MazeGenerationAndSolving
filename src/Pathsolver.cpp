#include "Pathsolver.hpp"
#include <queue>

Pathsolver::Pathsolver()
{

}

Pathsolver::Pathsolver(vector<Cell*> Initial, Cell* start, SolvingMethod method)
{
	path_record = Recorder(Initial);

	solveMaze(start, method);

}

Pathsolver::~Pathsolver()
{

}

void Pathsolver::solveMaze(Cell* start, SolvingMethod method)
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

bool Pathsolver::playRecording()
{
	return path_record.playRecording();
}

void Pathsolver::loopRecording()
{
	path_record.loopRecording();
}

void Pathsolver::stopRecording()
{
	path_record.stopPlaying();
}

bool Pathsolver::isVisitable(Cell* cell)
{
	if (cell != nullptr && !cell->pathVisited) {
		return true;
	}
	else {
		return false;
	}
}

bool Pathsolver::DFS(Cell* start)
{
	//abort if target is found
	if (start->isTarget) {
		return true;
	}
	
	//mark and record cell as visited
	if (!start->pathVisited) {
		start->pathVisited = true;
	}

	//set cell active and record it
	start->isActive = true;
	//record here#########################################################################################
	path_record.recordStep(start);
	
	//mark it as path, will be delete in recursion
	start->isPath = true;

	//add all valid adjecent to list
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
	while (adjCells.size() > 0) {

		//get next cell
		Cell* next = adjCells.back();

		//delete start from isActive and record it 
		start->isActive = false;
		//record here#########################################################################################
		path_record.recordStep(start);

		if (DFS(next)) {

			//recursion animation?
			start->isfinishedPath = true;
			//next node is onpath to target or is target

			//record path cell
			path_record.recordStep(start);

			return true;
		}	

		//delete cell from list
		adjCells.pop_back();

	}

	//cell is not on Path
	start->isActive = false;
	start->isPath = false;
	//record here#########################################################################################
	path_record.recordStep(start);

	return false;
}

bool Pathsolver::BFS(Cell* start)
{
	queue<vector<Cell*>> tobevisited;

	tobevisited.push(vector<Cell*>{start});

	bool wasTargetFound = false;
	Cell* foundTarget = nullptr;

	vector<Cell*> next_cells;

	while (!tobevisited.empty() && wasTargetFound == false) {

		vector<Cell*> current_cells = tobevisited.front();
		tobevisited.pop();

		next_cells.clear();

		if (current_cells.size() > 0) {

			for (auto cell : current_cells) {

				if (cell->isTarget) {
					wasTargetFound = true;
					foundTarget = cell;
					break;
				}

				cell->pathVisited = true;
		
				if (isVisitable(cell->getNorth())) {

					cell->getNorth()->setParent(cell);
					next_cells.push_back(cell->getNorth());
				}
				if (isVisitable(cell->getEast())) {

					cell->getEast()->setParent(cell);
					next_cells.push_back(cell->getEast());
				}
				if (isVisitable(cell->getSouth())) {

					cell->getSouth()->setParent(cell);
					next_cells.push_back(cell->getSouth());
				}
				if (isVisitable(cell->getWest())) {

					cell->getWest()->setParent(cell);
					next_cells.push_back(cell->getWest());
				}
			}
			// record current cells here###############################################################
			path_record.recordStep(current_cells);
			tobevisited.push(vector<Cell*>(next_cells));
		}
		
	}

	if (wasTargetFound) {
		Cell* backtrack = foundTarget;

		while (!backtrack->isStart) {

			backtrack->isfinishedPath = true;
			//record here####################################################################
			path_record.recordStep(backtrack);

			backtrack = backtrack->getParent();
		}
	}

	return wasTargetFound;
}