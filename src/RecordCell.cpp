#include "RecordCell.hpp"

RecordCell::RecordCell(const Cell& cell)
{
    this->p = cell.getPosition();
    this->currentColor = cell.getColor();
    this->prevColor = cell.getPrevColor();
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
    //Vector2 topright = Vector2Add(Vector2{ right, top }, offset);
    // Vector2 bottomleft = Vector2Add(Vector2{ left, bottom }, offset);
    // Vector2 bottomright = Vector2Add(Vector2{ right, bottom }, offset);

    const auto size = Vector2{ static_cast<float>(cellsize), static_cast<float>(cellsize) };
	if (m == FORWARD)
	{
		DrawRectangleV(topleft, size, this->getColor());
	}else
	{
		DrawRectangleV(topleft, size, this->getPrevColor());
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

Color RecordCell::getColor() const
{
	return this->currentColor;
}

Color RecordCell::getPrevColor() const
{
	return this->prevColor;
}
