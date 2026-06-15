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
		Maze(const int w, const int h, GenerationMethod method);
		~Maze();

		explicit Maze(const TileMap* custom_maze);

		Cell* getStart() const;
		vector<Cell*> getGeneratedMaze();

		//int getCellsize() const;
		int getHeight() const;
		int getWidth() const;

		void createEmptyMaze();

		void createConnectedMaze();

		void resetMaze() const;

		void drawMaze() const;
	
		bool playRecording();
		void loopRecording();
		void stopRecording();

		Recorder* getRecording();

		void displayInitialFrame() const;

	private:

		int height;
		int width;

		//int cellsize = 10;
		//Vector2 Offset{10.f, 10.f};

		vector<vector<Cell*>> Cell_Grid;

		vector<Cell*> Cell_List;

		Cell* Start = nullptr;
		Cell* Target = nullptr;

		Recorder record;

		std::mt19937 rand_gen;

		void generateMaze(GenerationMethod method);

		void drawCells() const;

		void deleteConnections() const;

		void RecursiveBacktracking(Cell& cell);

		void Kruskal();

		void HuntAndKill();

		vector<Cell*> getUnvisitedNeighbors(const Cell* cell) const;
		vector<Cell*> getVisitedNeighbors(const Cell* cell) const;
		vector<Cell*> getWalkableNeighborsFromWall(const Cell* cell) const;

		Cell* connectCells(Cell* first, Cell* second) const;
};

