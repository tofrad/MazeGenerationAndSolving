#pragma once

#include <cstdint>
#include "Point.hpp"
#include "raylib.h"

class Cell
{
	public:
		bool isStart = false;
		bool isTarget = false;

		bool wasVisited = false;
		bool isActive = false;

		bool isWall = false;

		bool pathVisited = false;
		bool isPath = false;  //marker for dfs recursion when dead end was found
		bool isfinishedPath = false;

		Cell();

		explicit Cell(const Point& p1);
		Cell(const Point& p1, int weight);

		~Cell();
		Point getPosition() const;

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

		void setColor(Color Color);
		Color getColor() const;
		Color getPrevColor() const;

	private:
		Point p;

		Color color = LIGHTGRAY;
		Color prev_color = LIGHTGRAY;
		void onColorChange();

		Cell* North = nullptr;
		Cell* East = nullptr;
		Cell* South = nullptr;
		Cell* West = nullptr;

		Cell* Parent = this;

		uint64_t cell_id = 0;
		
		int weight{};

		void setId();

};



