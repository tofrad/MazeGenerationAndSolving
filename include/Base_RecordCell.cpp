#include "Base_RecordCell.hpp"

Base_RecordCell::Base_RecordCell(Cell* cell)
{
    this->p = cell->getPosition();

}

Color Base_RecordCell::getCurrentColor() const
{
    return this->currentColor;
}

Color Base_RecordCell::getNextColor() const
{
    return this->nextColor;
}