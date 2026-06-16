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

void Cell::updateColor()
{
	onColorChange();

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
	this->wasVisited = false;
	this->Parent = nullptr;
	this->pathVisited = false;
	this->isPath = false;
	this->isfinishedPath = false;
	this->isActive = false;
	updateColor();
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
	this->updateColor();

}

void Cell::breakWall()
{
	this->isWall = false;
	this->updateColor();
}

uint64_t Cell::getCellID() const
{
	return cell_id;
}

void Cell::setColor(const Color Color)
{
	this->color = Color;
}

Color Cell::getColor() const
{
	return this->color;
}

Color Cell::getPrevColor() const
{
	return this->prev_color;
}


void Cell::onColorChange()
{
	this->prev_color = color;
}

void Cell::setId()
{
	cell_id = (uint64_t) (static_cast<uint64_t>(p.getX())	<< 32) | static_cast<uint64_t>(p.getY());
}
