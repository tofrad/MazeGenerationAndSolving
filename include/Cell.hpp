#pragma once

#include "Base_Cell.hpp"

#include <cstdint>

#include "../lib/raylib/include/raymath.h"

#include "Point.hpp"

class Cell : public Base_Cell
{
	public:
		Cell();
		explicit Cell(const Point& p1);
		Cell(const Point& p1, int cellsize, Vector2 offset);
		Cell(const Point& p1, int weight);

		~Cell();

		void setNorth(Cell* north);
		void setEast(Cell* east);
		void setSouth(Cell* south);
		void setWest(Cell* west);
		void setParent(Cell* parent);

		Cell* getNorth() const;
		Cell* getEast() const;
		Cell* getSouth() const;
		Cell* getWest() const;
		Cell* getParent() const;

		Cell* findRoot();

		void updateColor();

		static void drawCell() {};
		void resetCell();
		void makeWall();
		void breakWall();

		uint64_t getCellID() const;

		bool isStart = false;
		bool isTarget = false;

		bool wasVisited = false;
		bool isActive = false;

		bool isWall = false;

		bool pathVisited = false;
		bool isPath = false;  //marker for dfs recursion when dead end was found
		bool isfinishedPath = false;


	private:
		Cell* North = nullptr;
		Cell* East = nullptr;
		Cell* South = nullptr;
		Cell* West = nullptr;

		Cell* Parent = this;

		uint64_t cell_id = 0;
		
		int weight{};

		void setId();

};



