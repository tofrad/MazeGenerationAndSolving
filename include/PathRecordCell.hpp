#pragma once

#include"raylib.h"
#include "Cell.hpp"
#include "Base_RecordCell.hpp"
#include "State_Definitions.hpp"

struct ConnectionData
{
    int ConnectionWeight = 0;
    Vector2  ConnectsFrom{-1.f,-1.f};
    bool hasConnection = false;

};

class PathRecordCell : public Base_RecordCell
{
public:
    explicit PathRecordCell(Cell* cell);
    ~PathRecordCell() override;
    void draw(float cellsize, const Direction dir) const override;

    void drawEmptyCell(float cellsize) const;

private:
    static Color getColorFromFlags(const PathFlags* flags);
    // static Color getColorFromFlags(const PathFlags* flags);
    void extractConnectionsData(const Cell* cell);

    void drawPath(float cellsize, const Direction dir) const;

    ConnectionData Data;
};



