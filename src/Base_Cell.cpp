#include "Base_Cell.hpp"


Base_Cell::Base_Cell()
{
	this->p = Point();
}

Base_Cell::Base_Cell(const Point& p1)
{
	this->p = p1;
}

Base_Cell::Base_Cell(const Point& p1, const int cellsize, const Vector2 offset) : Base_Cell(p1)
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


void Base_Cell::drawCell() const
{

	//Vector2 offset_vec = Vector2{ 0, 0 };

	Point P = getPosition();

	const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
	const float right = left + static_cast<float>(cellsize);

	const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);
	const float bottom = top + static_cast<float>(cellsize);

	const Vector2 topleft = Vector2Add(Vector2{ left, top }, offset);
	//Vector2 topright = Vector2Add(Vector2{ right, top }, offset);
	// Vector2 bottomleft = Vector2Add(Vector2{ left, bottom }, offset);
	// Vector2 bottomright = Vector2Add(Vector2{ right, bottom }, offset);

	const auto size = Vector2{ static_cast<float>(cellsize), static_cast<float>(cellsize) };
	Vector2 topleft_rect = Vector2Add(Vector2{ left+1, top+1 }, offset);

	DrawRectangleV(topleft, size, getColor());


}

void Base_Cell::drawEmptyCell() const
{
	Point P = getPosition();

	const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
	const float right = left + static_cast<float>(cellsize);

	const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);
	const float bottom = top + static_cast<float>(cellsize);

	// Vector2 topleft = Vector2Add(Vector2{ left, top }, offset);
	// Vector2 topright = Vector2Add(Vector2{ right, top }, offset);
	// Vector2 bottomleft = Vector2Add(Vector2{ left, bottom }, offset);
	// Vector2 bottomright = Vector2Add(Vector2{ right, bottom }, offset);



}

Point Base_Cell::getPosition() const
{
	return this->p;
}

void Base_Cell::setColor(const Color Color)
{
	this->color = Color;
}

Color Base_Cell::getColor() const
{
	return this->color;
}

void Base_Cell::onColorChange()
{
	this->prev_color = color;
}



