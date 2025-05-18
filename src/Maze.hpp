#pragma once

#include <random>
#include <vector>

#include "Cell.hpp"
#include "Recorder.hpp"

using namespace std;

class Maze
{
	public:
		Maze();
		Maze(int width, int height, int screenwidth, int screenheight);
		~Maze();

		Cell* getStart();
		vector<Cell*> getGeneratedMaze();

		void createEmptyMaze();

		void createConnectedMaze();

		void generateMaze();

		void drawMaze();

		bool playRecording();
		void loopRecording();
		void stopRecording();

	private:

		int height;
		int width;
		int cellsize;

		vector<vector<Cell*>> Cell_Grid;

		vector<Cell*> Cell_List;

		vector<Cell*> GeneratedMaze;

		Cell* Start = nullptr;
		Cell* Target = nullptr;

		Recorder record;

		std::mt19937 rand_gen;

		void saveLastState();

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

