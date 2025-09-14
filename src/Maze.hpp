#pragma once

#include <random>
#include <vector>

#include "Cell.hpp"
#include "Recorder.hpp"

#include "State_Definitions.hpp"

using namespace std;

class Maze
{
	public:
		Maze();
		Maze(int width, int screenwidth, int screenheight, GenerationMethod method);
		~Maze();

		Cell* getStart();
		vector<Cell*> getGeneratedMaze();

		void createEmptyMaze();

		void createConnectedMaze();

		void resetMaze();

		void drawMaze();
	
		bool playRecording();
		void loopRecording();
		void stopRecording();

	private:

		int height;
		int width;
		int cellsize;
		Vector2 Offset;

		vector<vector<Cell*>> Cell_Grid;

		vector<Cell*> Cell_List;

		Cell* Start = nullptr;
		Cell* Target = nullptr;

		Recorder record;

		std::mt19937 rand_gen;

		void generateMaze(GenerationMethod method);

		void drawCells();

		void deleteConnections();

		void RecursiveBacktracking(Cell& cell);

		void Kruskal();

		void HuntAndKill();

		void uniteGroupByParents(Cell* start);

		vector<Cell*> getUnvisitedNeighbors(Cell* cell);
		vector<Cell*> getVisitedNeighbors(Cell* cell);

		void connectCells(Cell* first, Cell* second);
};

