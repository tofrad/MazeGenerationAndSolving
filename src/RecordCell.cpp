#include "RecordCell.hpp"

RecordCell::RecordCell(Cell* const cell)
{
    this->p = cell->getPosition();
    this->currentColor = getColorFromFlags(cell->getCellFlags_Current());
    this->nextColor = getColorFromFlags(cell->getCellFlags_Next());
	cell->updateCellFlags();
}

RecordCell::~RecordCell()
{

}

void RecordCell::drawCell(const float cellsize, const Color color, Mode m) const
{
    const Point P = p;

    const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
    const float right = left + static_cast<float>(cellsize);

    const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);
    const float bottom = top + static_cast<float>(cellsize);

    const Vector2 topleft = Vector2{ left, top };

    const auto size = Vector2{ static_cast<float>(cellsize), static_cast<float>(cellsize) };
	if (m == FORWARD)
	{
		DrawRectangleV(topleft, size, this->getNextColor());
	}else
	{
		DrawRectangleV(topleft, size, this->getCurrentColor());
	}
}

void RecordCell::drawEmptyCell(const float cellsize) const
{
	const Point P = p;

	const float left = static_cast<float>(P.getX()) * static_cast<float>(cellsize);
	const float right = left + static_cast<float>(cellsize);

	const float top = static_cast<float>(P.getY()) * static_cast<float>(cellsize);
	const float bottom = top + static_cast<float>(cellsize);

}

Color RecordCell::getCurrentColor() const
{
	return this->currentColor;
}

Color RecordCell::getNextColor() const
{
	return this->nextColor;
}

Color RecordCell::getColorFromFlags(const CellFlags *flags)
{
	if (flags->isStart){ return BLUE;}
	if (flags->isTarget){ return RED;}
	if (flags->isActive){ return LIME;}
	if (flags->isWall){ return BLACK;}
	if (flags->isfinishedPath){ return MAGENTA;}
	if (flags->isPath){ return GOLD;}
	if (flags->pathVisited){ return BEIGE;}
	if (flags->wasVisited){ return DARKGRAY;}

	// Default
	return LIGHTGRAY;
}
