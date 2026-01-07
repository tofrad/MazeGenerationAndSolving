#pragma once

#include "Base_Cell.hpp"

#include <cstdint>

#include "raymath.h"

#include "Point.hpp"

class Cell : public Base_Cell
{
	public:
		Cell();
		Cell(Point p1);
		Cell(Point p1, int cellsize, Vector2 offset);
		Cell(Point p1, int weight);

		~Cell();

		void setNorth(Cell* north);
		void setEast(Cell* east);
		void setSouth(Cell* south);
		void setWest(Cell* west);
		void setParent(Cell* parent);

		Cell* getNorth();
		Cell* getEast();
		Cell* getSouth();
		Cell* getWest();
		Cell* getParent();

		void updateColor();

		void drawCell() {};
		void resetCell();
		void makeWall();

		void setWallbits();

		uint64_t getCellID();

		bool isStart = false;
		bool isTarget = false;

		bool wasVisited = false;
		bool isActive = false;

		bool isWall = false;

		bool pathVisited = false;
		bool isPath = false;  //marker for dfs recursion when deadend was found
		bool isfinishedPath = false;


	private:
		Cell* North = nullptr;
		Cell* East = nullptr;
		Cell* South = nullptr;
		Cell* West = nullptr;

		Cell* Parent = nullptr;

		uint64_t cell_id = 0;
		
		int weight;

		void setId();

};



