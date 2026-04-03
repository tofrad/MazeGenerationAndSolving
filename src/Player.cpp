#pragma once

#include "Player.hpp"

#include "raygui.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::init(ProgramCallbacks& callbacks) {

    this->callbacks = callbacks;
}

void Player::open() {
    
}

void Player::close() {
    
}

void Player::displayPlayerGUI()
{
    calculateLandmarks();
    ClearBackground(GRAY);

    int Bottom_Split_x = BelowCanvas.width / 10;
    int BetweenOffset = Bottom_Split_x / 4;

    int Button_Size_x = Bottom_Split_x;

    int Bottom_Split_y = BelowCanvas.height / 9;

    int Button_Y = Bottom_Split_y * 5;
    int ButtonSize_y = Bottom_Split_y * 4;

    int Button_FirstFrame = 2 * Bottom_Split_x;
    int Button_Rewind = Button_FirstFrame + Button_Size_x + BetweenOffset;
    int Button_Pause = Button_Rewind + Button_Size_x + BetweenOffset;
    int Button_Resume = Button_Pause + Button_Size_x + BetweenOffset;
    int Button_LastFrame = Button_Resume + Button_Size_x + BetweenOffset;

    if (GuiButton(Rectangle(Button_FirstFrame, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y), GuiIconText(129, ""))) { /* ACTION */ }
    if (GuiButton(Rectangle(Button_Rewind, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y), GuiIconText(130, ""))) { /* ACTION */ }
    if (GuiButton(Rectangle(Button_Pause, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y), GuiIconText(132, ""))) { /* ACTION */ }
    if (GuiButton(Rectangle(Button_Resume, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y), GuiIconText(131, ""))) { /* ACTION */ }
    if (GuiButton(Rectangle(Button_LastFrame, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y), GuiIconText(134, ""))) { /* ACTION */ }


    DrawRectangle(Canvas.Point.x, Canvas.Point.y, Canvas.width, Canvas.height, RAYWHITE);

    //DrawRectangle(BelowCanvas.Point.x, BelowCanvas.Point.y, BelowCanvas.width, BelowCanvas.height, RED);

    //DrawRectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height, GREEN);

    if (GuiButton(Rectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height), GuiIconText(142, ""))) { /* ACTION */ }

}

void Player::calculateLandmarks()
{
    Screensize size = callbacks.getWindowSize();
    switch (size) {
    case UHD:
        window_height = 1440;
        window_width = 2560;
        break;
    case FHD:
        window_height = 1080;
        window_width = 1920;
        break;
    case WSXGA:
        window_height = 900;
        window_width = 1600;
        break;
    case SMALL:
        window_height = 540;
        window_width = 960;
        break;
    default:
        window_height = 540;
        window_width = 960;
        break;
    }

    WindowSection = (window_width - (3 * General_Offset)) / 8;
    int WindowSection_Vertical = (window_height - (2 * General_Offset)) / 10;

    //Calculate Canvas as Center
    Canvas.width = 8 * WindowSection;
    Canvas.height = WindowSection_Vertical * 9 - General_Offset;//(Canvas.width / 16) * 9;
    Canvas.Point = Vector2{ General_Offset, General_Offset};

    //BelowCanvas Field
    BelowCanvas.width = WindowSection * 7;
    BelowCanvas.Point = Vector2{ General_Offset, Canvas.Point.y + Canvas.height};
    BelowCanvas.height = WindowSection_Vertical * 1 - General_Offset; // window_height - General_Offset - BelowCanvas.Point.y;


    //Bottom right Field
    BottomRight.width = 1 * WindowSection; //RightToCanvas.width;
    BottomRight.height = BelowCanvas.height;
    BottomRight.Point = Vector2{ BelowCanvas.Point.x + BelowCanvas.width, BelowCanvas.Point.y };

}