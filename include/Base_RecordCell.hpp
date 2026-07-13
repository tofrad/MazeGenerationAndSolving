#pragma once

#include "Cell.hpp"
#include "raylib.h"
#include "State_Definitions.hpp"

struct CellData
{
    int Weight = 0;
    bool hasWeight = false;

};

class Base_RecordCell
{
    public:
        Base_RecordCell(const Cell* cell);
        virtual ~Base_RecordCell() = default;

        virtual void draw(float cellsize, const Direction dir) const{};

        Color getCurrentColor() const;
        Color getNextColor() const;

    protected:
        Color currentColor = LIGHTGRAY;
        Color nextColor = LIGHTGRAY;
        Point p;
        CellData cellData;
        void updateCellData(const Cell* cell);

};
