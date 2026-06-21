#pragma once

#include "Cell.hpp"

enum Mode
{
    FORWARD,
    BACKWARD
};
class RecordCell
{
    public:
        explicit RecordCell(const Cell& cell);
        ~RecordCell();
        void drawCell(float cellsize, Color color,Mode m) const;
        void drawEmptyCell(float cellsize) const;

        Color getColor() const;
        Color getPrevColor() const;

    private:
        Point p;
        Color currentColor;
        Color prevColor;
};


