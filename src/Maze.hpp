#pragma once

#include <vector>
#include "Cell.hpp"
#include "Recorder.hpp"

using namespace std;

class Maze
{
	public:
		Maze();
		Maze(int height, int width, int screenwidth, int screenheight);
		~Maze();

		Cell* getStart();
		vector<Cell*> getGeneratedMaze();

		void createEmptyMaze();

		void createConnectedMaze();

		void generateMaze();

		void drawMaze();

		void playRecording();

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

		void saveLastState();

		void drawCells();

		void deleteConnections();

		void RecursiveBacktracking(Cell& cell);
};

