#include "Cell.hpp"

#include <iostream>
#include <vector>

#include "Global_Config.hpp"

Cell::Cell()
{

}

Cell::Cell(const Point& p1)
{
	this->p = p1;
}

Cell::Cell(const Point& p1, const int weight) : Cell(p1)
{
	this->weight = weight;
}

Cell::~Cell() {
	delete this;
}

Point Cell::getPosition() const
{
	return this->p;
}

const MazeFlags* Cell::getMazeFlags_Current() const
{
	return &this->maze_current_flags;
}

const MazeFlags* Cell::getMazeFlags_Next() const
{
	return &this->maze_next_flags;
}

const PathFlags* Cell::getPathFlags_Current() const
{
	return &this->path_current_flags;
}

const PathFlags* Cell::getPathFlags_Next() const
{
	return &this->path_next_flags;
}

void Cell::updateMazeFlags()
{
	//do not touch start or target
	this->maze_current_flags.isActive = this->maze_next_flags.isActive;
	this->maze_current_flags.isWall = this->maze_next_flags.isWall;
	this->maze_current_flags.Maze_CellWasVisited = this->maze_next_flags.Maze_CellWasVisited;
}

void Cell::updatePathFlags()
{
	//do not touch start or target
	this->path_current_flags.Path_IsFinishedPath = this->path_next_flags.Path_IsFinishedPath;
	this->path_current_flags.Path_IsCurrentPath = this->path_next_flags.Path_IsCurrentPath;
	this->path_current_flags.Path_CellWasVisited = this->path_next_flags.Path_CellWasVisited;
}

void Cell::setStart()
{
	this->maze_current_flags.isStart = true;
	this->maze_next_flags.isStart = true;
}

void Cell::setTarget()
{
	this->maze_current_flags.isTarget = true;
	this->maze_next_flags.isTarget = true;
}

void Cell::setNorth(Cell* north)
{
	this->North = north;
}

void Cell::setEast(Cell* east)
{
	this->East = east;
}

void Cell::setSouth(Cell* south)
{
	this->South = south;
}

void Cell::setWest(Cell* west)
{
	this->West = west;
}

void Cell::setParent(Cell* parent)
{
	this->Parent = parent;
}

void Cell::setPathConnectFrom(Cell* origin)
{
	if (origin != nullptr)
	{
		PathConnectFrom = origin;
	}
}

Cell* Cell::getPathConnectFrom() const
{
	return PathConnectFrom;
}

Cell* Cell::getNorth() const
{
	return North;
}

Cell* Cell::getEast() const
{
	return East;
}

Cell* Cell::getSouth() const
{
	return South;
}

Cell* Cell::getWest() const
{
	return West;
}

Cell* Cell::getParent() const
{
	return Parent;
}

//find with path compression
Cell* Cell::findRoot()
{
	if(Parent != this) {
		Parent = Parent->findRoot();
	}
	return Parent;
}

void Cell::resetCell()
{
	this->maze_next_flags.Maze_CellWasVisited = false;
	this->maze_next_flags.isActive = false;

	this->path_next_flags.Path_CellWasVisited = false;
	this->path_next_flags.Path_IsCurrentPath = false;
	this->path_next_flags.Path_IsFinishedPath = false;

	this->maze_current_flags.Maze_CellWasVisited = false;
	this->maze_current_flags.isActive = false;

	this->path_current_flags.Path_CellWasVisited = false;
	this->path_current_flags.Path_IsCurrentPath = false;
	this->path_current_flags.Path_IsFinishedPath = false;

	this->Parent = nullptr;
}

void Cell::makeWall()
{
	if(this->getNorth() != nullptr){

		this->getNorth()->setSouth(nullptr);
		this->setNorth(nullptr);
	}

	if (this->getEast() != nullptr) {

		this->getEast()->setWest(nullptr);
		this->setEast(nullptr);
	}

	if (this->getSouth() != nullptr) {

		this->getSouth()->setNorth(nullptr);
		this->setSouth(nullptr);
	}

	if (this->getWest() != nullptr) {

		this->getWest()->setEast(nullptr);
		this->setWest(nullptr);
	}

	this->maze_next_flags.isWall = true;

}

void Cell::breakWall()
{
	this->maze_next_flags.isWall = false;
}

int Cell::getWeight() const
{
	return this->weight;
}

void Cell::setWeight(const int w)
{
	this->weight = w;

	if (this->weight > MAX_WEIGHT)
	{
		this->weight = MAX_WEIGHT;
	}
	if (this->weight < MIN_WEIGHT)
	{
		this->weight = MIN_WEIGHT;
	}

	if (this->weight > 0)
	{
		this->maze_next_flags.hasWeight = true;
		this->maze_current_flags.hasWeight = true;
	}
}

void Cell::addWeight(const int w)
{
	this->weight += w;
	this->maze_next_flags.hasWeight = true;
	this->maze_current_flags.hasWeight = true;

	if (this->weight > MAX_WEIGHT)
	{
		this->weight = MAX_WEIGHT;
	}
}

int Cell::getCellID() const
{
	return cell_id;
}

void Cell::setId(const int id)
{
	cell_id = id;
}
