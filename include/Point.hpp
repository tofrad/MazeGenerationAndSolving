#pragma once

class Point
{
	public:
		Point();
		Point(int x, int y);
		~Point();

		void setX(int x);
		void setY(int y);

		int getX();
		int getY();

		bool operator==(Point p1);

	private:
		int X;
		int Y;

};

