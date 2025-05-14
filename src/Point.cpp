#include "Point.hpp"

Point::Point(){
	this->X = 0;
	this->Y = 0;
}

Point::~Point() {

}

Point::Point(int x, int y) : X(x), Y(y) {

}

int Point::getX() {
	return this->X;
}

int Point::getY() {

	return this->Y;
}

void Point::setX(int x) {

	this->X = x;
}

void Point::setY(int y) {

	this->Y = y;
}

bool Point::operator==(Point p1) {

	if (this->X == p1.X && this->Y == p1.Y) {
		return true;
	}
	else {
		return false;
	}
}