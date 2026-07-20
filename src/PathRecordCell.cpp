#include "PathRecordCell.hpp"

#include "UI_Config.hpp"


PathRecordCell::PathRecordCell(Cell* cell) : Base_RecordCell(cell)
{
	this->currentColor = getColorFromFlags(cell->getPathFlags_Current());
	this->nextColor = getColorFromFlags(cell->getPathFlags_Next());
	cell->updatePathFlags();
	extractConnectionsData(cell);
}

PathRecordCell::~PathRecordCell()
{

}

void PathRecordCell::draw(const float cellsize, const Direction dir) const
{
	drawPath(cellsize, dir);
}

void PathRecordCell::drawPath(const float cellsize, const Direction dir) const
{
	const Point P = p;

	const float origin_x = Data.ConnectsFrom.x * static_cast<float>(cellsize) + (0.5 * cellsize);
	const float origin_y = Data.ConnectsFrom.y * static_cast<float>(cellsize) + (0.5 * cellsize);
	const Vector2 origin = Vector2(origin_x, origin_y);

	const float center_x = static_cast<float>(P.getX()) * static_cast<float>(cellsize) + (0.5 * cellsize);
	const float center_y = static_cast<float>(P.getY()) * static_cast<float>(cellsize) + (0.5 * cellsize);

	const Vector2 center = Vector2{ center_x, center_y };

	const float radius = cellsize / 4;

	if (dir == FORWARD)
	{
		DrawCircleV(center, radius, this->getNextColor());
		// DrawCircleLinesV(center, radius, BLACK);
		if (Data.hasConnection)
		{
			DrawLineEx(origin, center,radius, this->getNextColor());
		}

	}else
	{
		DrawCircleV(center, radius, this->getCurrentColor());
		// DrawCircleLinesV(center, radius, BLACK);
		if (Data.hasConnection)
		{
			DrawLineEx(origin, center,radius, this->getCurrentColor());
		}
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
	if (flags->Path_IsFrontier){return UI_Config::PATH_FRONT_COLOR;}
	if (flags->Path_IsFinishedPath){ return UI_Config::PATH_FIN_COLOR;}
	if (flags->Path_IsCurrentPath){ return UI_Config::PATH_CUR_COLOR;}
	if (flags->Path_CellWasVisited){ return UI_Config::PATH_VISITED_COLOR;}

	// Default
	return LIGHTGRAY;
}

void PathRecordCell::extractConnectionsData(const Cell* cell)
{
	//get origin point
	if (cell->getPathConnectFrom() != nullptr)
	{
		Data.ConnectsFrom.x = cell->getPathConnectFrom()->getPosition().getX();
		Data.ConnectsFrom.y = cell->getPathConnectFrom()->getPosition().getY();
		Data.hasConnection = true;
	}
}
