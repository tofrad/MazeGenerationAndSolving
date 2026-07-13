#include "Base_RecordCell.hpp"

Base_RecordCell::Base_RecordCell(const Cell* cell)
{
    this->p = cell->getPosition();
    updateCellData(cell);
}

Color Base_RecordCell::getCurrentColor() const
{
    return this->currentColor;
}

Color Base_RecordCell::getNextColor() const
{
    return this->nextColor;
}

void Base_RecordCell::updateCellData(const Cell* cell)
{
    if (cell->getMazeFlags_Current()->hasWeight)
    {
        cellData.hasWeight = true;
        cellData.Weight = cell->getWeight();
    }
}
