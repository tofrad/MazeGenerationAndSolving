#pragma once

#include <unordered_map>

#include "Cell.hpp"
#include "Recorder.hpp"


class Pathsolver
{
	public:
		Pathsolver();
		Pathsolver(vector<Cell*> Initial);
		~Pathsolver();

		void solveMaze(Cell* start);

		bool playRecording();
		void loopRecording();
		void stopRecording();

	private:

		Cell* startNode = nullptr;

		vector<Cell*> Cell_List;

		unordered_map<uint64_t, int> visitedCells;

		Recorder path_record;

		bool DFS(Cell* start);

};


