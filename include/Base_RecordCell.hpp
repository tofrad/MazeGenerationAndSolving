#pragma once

#include "Cell.hpp"
#include "raylib.h"
#include "State_Definitions.hpp"

struct ConnectionData
{
    int ConnectionWeight = 0;
    Vector2  ConnectsFrom{-1.f,-1.f};
    bool hasConnection = false;
};

class Base_RecordCell
{
    public:
        Base_RecordCell(Cell* cell);
        virtual ~Base_RecordCell() = default;

        virtual void draw(float cellsize, const Direction dir) const{};

        Color getCurrentColor() const;
        Color getNextColor() const;

    protected:
        Color currentColor = LIGHTGRAY;
        Color nextColor = LIGHTGRAY;
        Point p;

};
