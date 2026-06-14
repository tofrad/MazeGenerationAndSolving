#pragma once

#include "Point.hpp"

#include "../lib/raylib/include/raylib.h"
#include "../lib/raylib/include/raymath.h"


class Base_Cell
{
	public:

		Base_Cell();
		explicit Base_Cell(const Point& p1);
		Base_Cell(const Point& p1, int cellsize, Vector2 offset);

		Base_Cell get_Base_copy() const;

		~Base_Cell();

		void drawCell() const;

		void drawEmptyCell() const;

		Point getPosition() const;

		void setColor(Color Color);
		Color getColor() const;
		void onColorChange();

		Point p;
		int cellsize = 10;
		Vector2 offset{10.f, 10.f};

		Color color = LIGHTGRAY;
		int cellsize_{};

	private:

		Color prev_color = LIGHTGRAY;


};
