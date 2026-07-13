#pragma once

#include"raylib.h"
#include "Cell.hpp"
#include "Base_RecordCell.hpp"
#include "State_Definitions.hpp"

class MazeRecordCell : public Base_RecordCell
{
    public:
        explicit MazeRecordCell(Cell* cell);
        ~MazeRecordCell() override;
        void draw(float cellsize, const Direction dir) const override;

        void drawEmptyCell(float cellsize) const;

    private:
        static Color getColorFromFlags(const MazeFlags* flags);
        // static Color getColorFromFlags(const PathFlags* flags);

        void drawCell(float cellsize, const Direction dir) const;
        void drawPath(float cellsize, const Direction dir) const;

};


