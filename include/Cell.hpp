#pragma once

#include <cstdint>
#include "Point.hpp"

//TODO
//setCellStart / Target to ensure setting current flags too for correct behav

struct CellFlags
{
	//special states
	bool isStart = false;
	bool isTarget = false;
	bool isWall = false;
	//states for maze generating and solving behavior
	bool wasVisited = false;
	bool isActive = false;
	bool pathVisited = false;
	bool isPath = false;  //marker for dfs recursion when dead end was found
	bool isfinishedPath = false;
};

class Cell
{
	public:
		CellFlags next_flags;

		Cell();
		explicit Cell(const Point& p1);
		Cell(const Point& p1, int weight);

		~Cell();
		Point getPosition() const;
		const CellFlags* getCellFlags_Current() const;
		const CellFlags* getCellFlags_Next() const;
		void updateCellFlags();
		void setStart();
		void setTarget();

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

		static void drawCell() {};
		void resetCell();
		void makeWall();
		void breakWall();

		uint64_t getCellID() const;

	private:
		Point p;

		CellFlags current_flags;

		Cell* North = nullptr;
		Cell* East = nullptr;
		Cell* South = nullptr;
		Cell* West = nullptr;

		Cell* Parent = this;

		uint64_t cell_id = 0;
		
		int weight{};

		void setId();
};



