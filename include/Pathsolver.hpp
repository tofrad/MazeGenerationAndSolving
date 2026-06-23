#pragma once

#include <unordered_map>

#include "Cell.hpp"
#include "Recorder.hpp"

#include "State_Definitions.hpp"


class Pathsolver
{
	public:
		Pathsolver();
		Pathsolver(Cell* start, SolvingMethod method, Recorder* recorder);
		~Pathsolver();

		Recorder* getRecording() const;

	private:

		Cell* startNode = nullptr;

		vector<Cell*> Cell_List;

		unordered_map<uint64_t, int> visitedCells;

		Recorder* path_record = nullptr;

		static bool isVisitable(const Cell* cell);

		bool solveMaze(Cell* start, SolvingMethod method);

		bool DFS(Cell* start) const;

		bool BFS(Cell* start) const;

		

};


