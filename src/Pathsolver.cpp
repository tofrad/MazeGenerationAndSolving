#include "Pathsolver.hpp"

Pathsolver::Pathsolver()
{

}

Pathsolver::Pathsolver(vector<Cell*> Initial)
{
	path_record = Recorder(Initial);
}

Pathsolver::~Pathsolver()
{

}

void Pathsolver::solveMaze(Cell* start)
{
	
	//start recording
	path_record.startRecording();

	//solve Maze
	DFS(start);

	//stop Recording
	path_record.stopRecording(); 
}

void Pathsolver::playRecording()
{
	path_record.startPlaying();
	path_record.stepForward();
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

	if (North != nullptr && North->pathVisited == false) {
		adjCells.push_back(North);
	}
	if (East != nullptr && East->pathVisited == false) {
		adjCells.push_back(East);
	}
	if (South != nullptr && South->pathVisited == false) {
		adjCells.push_back(South);
	}
	if (West != nullptr && West->pathVisited == false) {
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
