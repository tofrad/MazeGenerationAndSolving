#include "PathRecordCell.hpp"

// #include "Maze.hpp"


PathRecordCell::PathRecordCell(Cell* cell) : Base_RecordCell(cell)
{
	this->currentColor = getColorFromFlags(cell->getPathFlags_Current());
	this->nextColor = getColorFromFlags(cell->getPathFlags_Next());
	cell->updateMazeFlags();
}

PathRecordCell::~PathRecordCell()
{

}

void PathRecordCell::draw(const float cellsize, const Direction dir) const
{
	drawPath(cellsize, dir);
}

// to get to path drawing, maybe get back one step in recording and get coordinates of changed cells, if adjacent prob the needed connection

void PathRecordCell::drawPath(const float cellsize, const Direction dir) const
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

void PathRecordCell::drawEmptyCell(const float cellsize) const
{
	const Point P = p;

	const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
	const float right = left + static_cast<float>(cellsize);

	const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);
	const float bottom = top + static_cast<float>(cellsize);

}

Color PathRecordCell::getColorFromFlags(const PathFlags *flags)
{
	if (flags->Path_IsFinishedPath){ return MAGENTA;}
	if (flags->Path_IsCurrentPath){ return GOLD;}
	if (flags->Path_CellWasVisited){ return BEIGE;}

	// Default
	return LIGHTGRAY;
}
