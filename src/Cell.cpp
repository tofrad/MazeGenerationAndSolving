#include "Cell.hpp"

Cell::Cell() : Base_Cell()
{
	this->weight = 0;
}

Cell::Cell(Point p1) : Base_Cell(p1)
{
	this->weight = 0;
	setId();
}

Cell::Cell(Point p1, int cellsize, Vector2 offset) : Base_Cell(p1, cellsize, offset)
{
	setId();
}

Cell::Cell(Point p1, int w) : Cell(p1)
{
	this->weight = w;
	setId();
}

Cell::~Cell() {

}

void Cell::setNorth(Cell* cellptr)
{
	this->North = cellptr;
}

void Cell::setEast(Cell* cellptr)
{
	this->East = cellptr;
}

void Cell::setSouth(Cell* cellptr)
{
	this->South = cellptr;
}

void Cell::setWest(Cell* cellptr)
{
	this->West = cellptr;
}

void Cell::setParent(Cell* parent)
{
	this->Parent = parent;
}

Cell* Cell::getNorth()
{
	return North;
}

Cell* Cell::getEast()
{
	return East;
}

Cell* Cell::getSouth()
{
	return South;
}

Cell* Cell::getWest()
{
	return West;
}

Cell* Cell::getParent()
{
	return Parent;
}

void Cell::updateColor()
{
	Base_Cell::onColorChange();

	if (isActive)		{ color = LIME;      return; }
	if (isStart)		{ color = BLUE;      return; }
	if (isTarget)		{ color = RED;       return; }
	if (isfinishedPath) { color = MAGENTA;   return; }
	if (isPath)			{ color = GOLD;      return; }

	if (pathVisited)	{ color = BEIGE;     return; }
	if (wasVisited)		{ color = DARKGRAY;  return; }
	if (isWall)			{ color = BLACK;     return; }

	// Default
	color = LIGHTGRAY;
}

void Cell::resetCell()
{
	this->Parent = nullptr;
	this->pathVisited = false;
	this->isPath = false;
	this->isfinishedPath = false;
	this->isActive = false;
	this->color = LIGHTGRAY;
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

	this->isWall = true;

}

void Cell::setWallbits()
{
	wallbits &= 0b0000;

	if (this->getNorth() == nullptr) {
		wallbits |= 0b1000;
	}
	if (this->getEast() == nullptr) {
		wallbits |= 0b0100;
	}
	if (this->getSouth() == nullptr) {
		wallbits |= 0b0010;
	}
	if (this->getWest() == nullptr) {
		wallbits |= 0b0001;
	}

}

uint64_t Cell::getCellID()
{
	return cell_id;
}

void Cell::setId()
{
	cell_id = (uint64_t) ((uint64_t)p.getX()	<< 32) | (uint64_t)p.getY();	
}
