#pragma once

class Point
{
	public:
		Point();
		Point(int x, int y);
		~Point();

		void setX(int x);
		void setY(int y);

		int getX() const;
		int getY() const;

		bool operator==(const Point& p1) const;

	private:
		int X;
		int Y;

};

