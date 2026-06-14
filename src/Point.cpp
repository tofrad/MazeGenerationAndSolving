#include "Point.hpp"

Point::Point(){
	this->X = 0;
	this->Y = 0;
}

Point::~Point() {

}

Point::Point(const int x, const int y) : X(x), Y(y) {

}

int Point::getX() const
{
	return this->X;
}

int Point::getY() const
{

	return this->Y;
}

void Point::setX(const int x) {

	this->X = x;
}

void Point::setY(const int y) {

	this->Y = y;
}

bool Point::operator==(const Point& p1) const
{

	if (this->X == p1.X && this->Y == p1.Y) {
		return true;
	}
	else {
		return false;
	}
}
