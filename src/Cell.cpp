#include "Cell.hpp"

Cell::Cell() {
	this->p = Point();
	this->weight = 0;
}

Cell::Cell(Point p1) {

	this->p = p1;
	this->weight = 0;
	setId();
}

Cell::Cell(Point p1, int cellsize, Vector2 offset) : Cell(p1)
{
	this->cellsize = cellsize;
	this->offset = offset;
}

Cell::Cell(Point p1, int w) {

	this->p = p1;
	this->weight = w;
	setId();
}

Cell::~Cell() {

}

Point Cell::getPosition()
{
	return this->p;
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

void Cell::setColor(Color color)
{
	this->color = color;
}

Color Cell::getColor()
{
	return this->color;
}

void Cell::updateColor()
{
	if (isActive) {
		this->color = LIME;
	}
	else if (isStart) {
		this->color = BLUE;
	}
	else if(isTarget){
		this->color = RED;
	}
	else if (isfinishedPath) {
		this->color = MAGENTA;
	}
	else if (isPath) {
		this->color = GOLD;
	}
	else if (pathVisited) {
		this->color = BEIGE;
	}
	else if (wasVisited){
		this->color = DARKGRAY;
	}
	else {
		this->color = LIGHTGRAY;
	}
}

void Cell::drawCell()
{
	updateColor();

	//Vector2 offset_vec = Vector2{ 0, 0 };

	Point P = getPosition();

	float left = (float)P.getX() * cellsize;
	float right = left + cellsize;

	float top = (float)P.getY() * cellsize;
	float bottom = top + cellsize;

	Vector2 topleft = Vector2Add(Vector2{ left, top }, offset);
	Vector2 topright = Vector2Add(Vector2{ right, top }, offset);
	Vector2 bottomleft = Vector2Add(Vector2{ left, bottom }, offset);
	Vector2 bottomright = Vector2Add(Vector2{ right, bottom }, offset);

	Vector2 size = Vector2{ (float)cellsize, (float)cellsize };

	DrawRectangleV(topleft, size, getColor());

	if (getSouth() == nullptr) {
		DrawLineEx(bottomleft, bottomright,3, BLACK);
	}
	else {
		DrawLineEx(bottomleft, bottomright, 3, getColor());
	}

	if (getEast() == nullptr) {
		DrawLineEx(topright, bottomright,3, BLACK);
	}
	else {
		DrawLineEx(topright, bottomright, 3, getColor());
	}

	if (getNorth() == nullptr) {
		DrawLineEx(topleft, topright,3, BLACK);
	}
	else {
		DrawLineEx(topleft, topright, 3, getColor());
	}

	if (getWest() == nullptr) {
		DrawLineEx(topleft, bottomleft,3, BLACK);
	}
	else {
		DrawLineEx(topleft, bottomleft, 3, getColor());
	}
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

uint64_t Cell::getCellID()
{
	return cell_id;
}

void Cell::setId()
{
	cell_id = (uint64_t) ((uint64_t)p.getX()	<< 32) | (uint64_t)p.getY();	
}
