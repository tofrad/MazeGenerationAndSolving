#pragma once

#include "Cell.hpp"
#include"raylib.h"

enum Mode
{
    FORWARD,
    BACKWARD
};
class RecordCell
{
    public:
        explicit RecordCell( Cell* const cell);
        ~RecordCell();
        void drawCell(float cellsize, Color color,Mode m) const;
        void drawEmptyCell(float cellsize) const;

        Color getCurrentColor() const;
        Color getNextColor() const;

    private:
        static Color getColorFromFlags(const CellFlags *flags);
        Point p;
        Color currentColor;
        Color nextColor;
};


