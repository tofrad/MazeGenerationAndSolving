#include "MazeRecordCell.hpp"
#include "UI_Config.hpp"

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

Color MazeRecordCell::getColorFromFlags(const MazeFlags* flags) const
{
	if (flags->isStart){ return UI_Config::START_COLOR;}
	if (flags->isTarget){ return UI_Config::TARGET_COLOR;}
	if (flags->isActive){ return UI_Config::MAZE_ACTIVE_COLOR;}
	if (flags->isWall){ return UI_Config::WALL_COLOR;}

	if (flags->hasWeight)
	{
		const int weight_case = cellData.Weight / 10;

		switch (weight_case)
		{
		case 0:
			return UI_Config::WEIGHT_00_COLOR;
			break;
		case 1:
			return UI_Config::WEIGHT_10_COLOR;
			break;
		case 2:
			return UI_Config::WEIGHT_20_COLOR;
			break;
		case 3:
			return UI_Config::WEIGHT_30_COLOR;
			break;
		case 4:
			return  UI_Config::WEIGHT_40_COLOR;
			break;
		case 5:
			return  UI_Config::WEIGHT_50_COLOR;
			break;
		case 6:
			return  UI_Config::WEIGHT_60_COLOR;
			break;
		case 7:
			return  UI_Config::WEIGHT_70_COLOR;
			break;
		case 8:
			return  UI_Config::WEIGHT_80_COLOR;
			break;
		case 9:
			return  UI_Config::WEIGHT_90_COLOR;
			break;
		default:
			break;

		}
	}

	if (flags->Maze_CellWasVisited){ return DARKGRAY;}

	// Default
	return LIGHTGRAY;
}
