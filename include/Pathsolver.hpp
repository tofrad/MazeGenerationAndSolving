#pragma once

#include <unordered_map>

#include "Cell.hpp"
#include "Recorder.hpp"

#include "State_Definitions.hpp"


class Pathsolver
{
	public:
		Pathsolver();
		Pathsolver(const vector<Cell*>& Initial, Cell* start, SolvingMethod method);
		~Pathsolver();

		bool playRecording();
		void loopRecording();
		void stopRecording();

		void displayInitialFrame() const;

		Recorder* getRecording();

	private:

		Cell* startNode = nullptr;

		vector<Cell*> Cell_List;

		unordered_map<uint64_t, int> visitedCells;

		Recorder path_record;

		static bool isVisitable(const Cell* cell);

		void solveMaze(Cell* start, SolvingMethod method);

		bool DFS(Cell* start);

		bool BFS(Cell* start);

		

};


