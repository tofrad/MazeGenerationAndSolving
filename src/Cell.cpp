#include "Cell.hpp"

Cell::Cell()
{
	this->weight = 0;
}

Cell::Cell(const Point& p1)
{
	this->p = p1;
	this->weight = 0;
	setId();
}

Cell::Cell(const Point& p1, const int w) : Cell(p1)
{
	this->weight = w;
	setId();
}

Cell::~Cell() {

}

Point Cell::getPosition() const
{
	return this->p;
}

const CellFlags* Cell::getCellFlags_Current() const
{
	return &this->current_flags;
}

const CellFlags* Cell::getCellFlags_Next() const
{
	return &this->next_flags;
}

void Cell::updateCellFlags()
{
	//do not touch start or target
	this->current_flags.isActive = this->next_flags.isActive;
	this->current_flags.isfinishedPath = this->next_flags.isfinishedPath;
	this->current_flags.isPath = this->next_flags.isPath;
	this->current_flags.isWall = this->next_flags.isWall;
	this->current_flags.pathVisited = this->next_flags.pathVisited;
	this->current_flags.wasVisited = this->next_flags.wasVisited;
}

void Cell::setStart()
{
	this->current_flags.isStart = true;
	this->next_flags.isStart = true;
}

void Cell::setTarget()
{
	this->current_flags.isTarget = true;
	this->next_flags.isTarget = true;
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
	this->next_flags.wasVisited = false;
	this->next_flags.pathVisited = false;
	this->next_flags.isPath = false;
	this->next_flags.isfinishedPath = false;
	this->next_flags.isActive = false;

	this->current_flags.wasVisited = false;
	this->current_flags.pathVisited = false;
	this->current_flags.isPath = false;
	this->current_flags.isfinishedPath = false;
	this->current_flags.isActive = false;

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

	this->next_flags.isWall = true;

}

void Cell::breakWall()
{
	this->next_flags.isWall = false;
}

uint64_t Cell::getCellID() const
{
	return cell_id;
}

void Cell::setId()
{
	cell_id = (static_cast<uint64_t>(p.getX())	<< 32) | static_cast<uint64_t>(p.getY());
}
