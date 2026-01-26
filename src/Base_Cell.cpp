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
	Vector2 topleft_rect = Vector2Add(Vector2{ left+1, top+1 }, offset);

	DrawRectangleV(topleft, size, getColor());

	float thickness = 3;

	if (wallbits & 0b1000) {
		DrawLineEx(topleft, topright, thickness, BLACK);
	}
	else {
		DrawLineEx(	Vector2Add(topleft, Vector2{3, 0 }) ,
					Vector2Add(topright, Vector2{ -3, 0}),
					thickness, getColor());
	}

	if (wallbits & 0b0100) {
		DrawLineEx(topright, bottomright, thickness, BLACK);
	}
	else {
		DrawLineEx( Vector2Add(topright, Vector2{ 0, 3}),
					Vector2Add(bottomright, Vector2{ 0, -3 }),
					thickness, getColor());
	}

	if (wallbits & 0b0010) {
		DrawLineEx(bottomleft, bottomright, thickness, BLACK);
	}
	else {
		DrawLineEx(	Vector2Add(bottomleft, Vector2{ 3, 0 }),
					Vector2Add(bottomright, Vector2{ -3, 0}),
					thickness, getColor());
	}

	if (wallbits & 0b0001) {
		DrawLineEx(topleft, bottomleft, thickness, BLACK);
	}
	else {
		DrawLineEx(	Vector2Add(topleft, Vector2{ 0, 3 }),
					Vector2Add(bottomleft, Vector2{ 0, -3 }),
					thickness, getColor());
	}

}

void Base_Cell::drawEmptyCell()
{
	Point P = getPosition();

	float left = (float)P.getX() * cellsize;
	float right = left + cellsize;

	float top = (float)P.getY() * cellsize;
	float bottom = top + cellsize;

	Vector2 topleft = Vector2Add(Vector2{ left, top }, offset);
	Vector2 topright = Vector2Add(Vector2{ right, top }, offset);
	Vector2 bottomleft = Vector2Add(Vector2{ left, bottom }, offset);
	Vector2 bottomright = Vector2Add(Vector2{ right, bottom }, offset);

	float thickness = 3;

	if (wallbits & 0b1000) {
		DrawLineEx(topleft, topright, thickness, BLACK);
	}

	if (wallbits & 0b0100) {
		DrawLineEx(topright, bottomright, thickness, BLACK);
	}


	if (wallbits & 0b0010) {
		DrawLineEx(bottomleft, bottomright, thickness, BLACK);
	}

	if (wallbits & 0b0001) {
		DrawLineEx(topleft, bottomleft, thickness, BLACK);
	}



}



