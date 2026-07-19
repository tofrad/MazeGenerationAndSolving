#pragma once

#include <memory>
#include "Point.hpp"


//states for maze generating
struct MazeFlags
{
	//special states
	bool isStart = false;
	bool isTarget = false;
	bool isWall = false;
	bool hasWeight = false;

	bool isActive = false;
	bool Maze_CellWasVisited = false;
};

//states for path solving
struct PathFlags
{
	//states for solving behavior
	bool Path_IsFrontier = false;
	bool Path_CellWasVisited = false;
	bool Path_IsCurrentPath = false;  //marker for dfs recursion when dead end was found
	bool Path_IsFinishedPath = false;
};

class Cell
{
	public:
		MazeFlags maze_next_flags;
		PathFlags path_next_flags;

		Cell();
		explicit Cell(const Point& p1);
		Cell(const Point& p1, int weight);

		~Cell();
		Point getPosition() const;
		const MazeFlags* getMazeFlags_Current() const;
		const MazeFlags* getMazeFlags_Next() const;
		const PathFlags* getPathFlags_Current() const;
		const PathFlags* getPathFlags_Next() const;
		void updateMazeFlags();
		void updatePathFlags();
		void setStart();
		void setTarget();

		void setNorth(Cell* north);
		void setEast(Cell* east);
		void setSouth(Cell* south);
		void setWest(Cell* west);
		void setParent(Cell* parent);

		void setPathConnectFrom(Cell* origin);
		Cell* getPathConnectFrom() const;

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

		int getWeight() const;
		void setWeight(int w);
		void addWeight(int w);

		int getCellID() const;
		void setId(const int id);

	private:
		Point p;

		MazeFlags maze_current_flags;
		PathFlags path_current_flags;

		Cell* North = nullptr;
		Cell* East = nullptr;
		Cell* South = nullptr;
		Cell* West = nullptr;

		Cell* Parent = this;

		int cell_id = 0;

		Cell*  PathConnectFrom = nullptr;

		int weight = 0;

		void setId();
};



