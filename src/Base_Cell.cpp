#include "Base_Cell.hpp"


Base_Cell::Base_Cell()
{
	this->p = Point();
}

Base_Cell::Base_Cell(Point p1)
{
	this->p = p1;
}

Base_Cell::Base_Cell(Point p1, int cellsize, Vector2 offset) : Base_Cell(p1)
{
	this->cellsize = cellsize;
	this->offset = offset;
}

Base_Cell Base_Cell::get_Base_copy() const
{
	return *this;
}


Base_Cell::~Base_Cell()
{

}


Point Base_Cell::getPosition()
{
	return this->p;
}

void Base_Cell::setColor(Color color)
{
	this->color = color;
}

Color Base_Cell::getColor()
{
	return this->color;
}

void Base_Cell::onColorChange()
{
	this->prev_color = color;
}

void Base_Cell::drawCell()
{

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

}



