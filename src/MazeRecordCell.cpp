#include "MazeRecordCell.hpp"

MazeRecordCell::MazeRecordCell(Cell* cell) : Base_RecordCell(cell)
{
	this->currentColor = getColorFromFlags(cell->getMazeFlags_Current());
	this->nextColor = getColorFromFlags(cell->getMazeFlags_Next());
	cell->updateMazeFlags();
}

MazeRecordCell::~MazeRecordCell()
{

}

void MazeRecordCell::draw(const float cellsize, const Direction dir) const
{
	drawCell(cellsize, dir);
}

// to get to path drawing, maybe get back one step in recording and get coordinates of changed cells, if adjacent prob the needed connection

void MazeRecordCell::drawCell(const float cellsize, const Direction dir) const
{
    const Point P = p;

    const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
    const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);

    const Vector2 topleft = Vector2{ left, top };

    const auto size = Vector2{ static_cast<float>(cellsize), static_cast<float>(cellsize) };
	if (dir == FORWARD)
	{
		DrawRectangleV(topleft, size, this->getNextColor());
	}else
	{
		DrawRectangleV(topleft, size, this->getCurrentColor());
	}

	if (cellData.hasWeight)
	{
		switch (cellData.Weight)
		{
			case 1:
				DrawRectangleV(topleft, size, GREEN);
				break;
			case 2:
				DrawRectangleV(topleft, size, YELLOW);
				break;
			case 3:
				DrawRectangleV(topleft, size, ORANGE);
				break;
			case 4:
				DrawRectangleV(topleft, size, MAROON);
				break;
			case 5:
				DrawRectangleV(topleft, size, PURPLE);
				break;
			default:
				break;

		}
	}
}

void MazeRecordCell::drawPath(const float cellsize, const Direction dir) const
{
	const Point P = p;

	const float center_x = static_cast<float>(P.getX()) * static_cast<float>(cellsize) + (0.5 * cellsize);
	const float center_y = static_cast<float>(P.getY()) * static_cast<float>(cellsize) + (0.5 * cellsize);

	const Vector2 center = Vector2{ center_x, center_y };

	const float radius = cellsize / 2;

	if (dir == FORWARD)
	{
		DrawCircleV(center, radius, this->getNextColor());
	}else
	{
		DrawCircleV(center, radius, this->getCurrentColor());
	}
}

void MazeRecordCell::drawEmptyCell(const float cellsize) const
{
	const Point P = p;

	const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
	const float right = left + static_cast<float>(cellsize);

	const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);
	const float bottom = top + static_cast<float>(cellsize);

}

Color MazeRecordCell::getColorFromFlags(const MazeFlags* flags)
{
	if (flags->isStart){ return BLUE;}
	if (flags->isTarget){ return RED;}
	if (flags->isActive){ return LIME;}
	if (flags->isWall){ return BLACK;}

	if (flags->Maze_CellWasVisited){ return DARKGRAY;}

	// Default
	return LIGHTGRAY;
}
