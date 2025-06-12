#pragma once

#include <cstdint>

#include "raylib.h"
#include "raymath.h"

#include "Point.hpp"

class Cell
{
	public:
		Cell();
		Cell(Point p1);
		Cell(Point p1, int cellsize, Vector2 offset);
		Cell(Point p1, int weight);

		~Cell();

		Point getPosition();

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

		void setColor(Color color);
		Color getColor();
		void updateColor();

		void drawCell();
		void resetCell();
		void makeWall();

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

		Color color = LIGHTGRAY;
		int cellsize;
		Vector2 offset;

		uint64_t cell_id = 0;

		Point p;
		int weight;

		void setId();

};



