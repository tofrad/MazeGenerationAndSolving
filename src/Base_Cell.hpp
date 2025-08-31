#pragma once

#include "Point.hpp"

#include "raylib.h"
#include "raymath.h"


class Base_Cell
{
	public:

		Base_Cell();
		Base_Cell(Point p1);
		Base_Cell(Point p1, int cellsize, Vector2 offset);

		Base_Cell get_Base_copy() const;

		~Base_Cell();

		void drawCell();

		Point getPosition();

		void setColor(Color color);
		Color getColor();
		void onColorChange();

		Point p;
		int cellsize = 10;
		Vector2 offset;

		Color color = LIGHTGRAY;

	private:

		Color prev_color = LIGHTGRAY;


};
