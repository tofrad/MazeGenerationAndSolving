#pragma once

#include <unordered_map>

#include "Cell.hpp"
#include "Recorder.hpp"

typedef enum {
	SM_DFS,
	SM_BFS

} SolvingMethod;

class Pathsolver
{
	public:
		Pathsolver();
		Pathsolver(vector<Cell*> Initial, Cell* start, SolvingMethod method);
		~Pathsolver();

		bool playRecording();
		void loopRecording();
		void stopRecording();

	private:

		Cell* startNode = nullptr;

		vector<Cell*> Cell_List;

		unordered_map<uint64_t, int> visitedCells;

		Recorder path_record;

		bool isVisitable(Cell* cell);

		void solveMaze(Cell* start, SolvingMethod method);

		bool DFS(Cell* start);

		bool BFS(Cell* start);

		

};


