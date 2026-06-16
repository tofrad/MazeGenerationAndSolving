#pragma once

#include "Cell.hpp"

class RecordCell
{
    public:
        explicit RecordCell(const Cell& cell);
        ~RecordCell();
        void drawCell(float cellsize, Color color) const;
        void drawEmptyCell(float cellsize) const;

        Color getColor() const;
        Color getPrevColor() const;

    private:
        Point p;
        Color currentColor;
        Color prevColor;
};


