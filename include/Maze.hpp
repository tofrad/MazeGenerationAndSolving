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
		Maze(const int w, const int h, GenerationMethod method, Recorder* recorder);
		~Maze();

		explicit Maze(const TileMap* custom_maze, Recorder* recorder);

		void GetTileMapFromMaze( TileMap& custom) const;

		Cell* getStart() const;
		vector<Cell*> getGeneratedMaze();

		int getHeight() const;
		int getWidth() const;
		void createConnectedMaze();

		void createEmptyMaze();

		void createNoWallMaze();

		void resetMaze() const;

	private:

		int height = 10;
		int width = 10;

		vector<vector<Cell*>> Cell_Grid;

		vector<Cell*> Cell_List;

		Cell* Start = nullptr;
		Cell* Target = nullptr;

		Recorder* record = nullptr;

		std::mt19937 rand_gen;


		void generateMaze(GenerationMethod method, Recorder* recorder);

		void RecursiveBacktracking(Cell* cell, uint step);
		void Kruskal();
		void HuntAndKill();

		void setWeights(const int weight_count);

		vector<Cell*> getUnvisitedNeighbors(const Cell* cell) const;
		vector<Cell*> getVisitedNeighbors(const Cell* cell) const;
		vector<Cell*> getWalkableNeighborsFromWall(const Cell* cell) const;

		Cell* connectCells(Cell* first, Cell* second) const;
		void deleteConnections() const;
};

