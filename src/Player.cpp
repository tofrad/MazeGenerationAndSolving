#pragma once

#include "Player.hpp"


Player::Player()
{
}

Player::~Player()
{
}

void Player::init(ProgramCallbacks& callbacks) {

    state = OPEN;
    this->callbacks = callbacks;
}

void Player::open() {
    state = OPEN;
}

void Player::close() {
    state = CLOSED;
}

void Player::displayPlayerGUI()
{
    calculateLandmarks();
    ClearBackground(GRAY);

    int window_height = 1080;
    int window_width = 1920;

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

    float Labelpos_y = 10;

    float Label_offset = 10;

    float Labelwidth = (window_width - (4 * Label_offset)) / 3;
    float Label_height = 30;


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

    //Calculate Canvas as Center
    Canvas.width = 7 * WindowSection;
    Canvas.height = (Canvas.width / 16) * 9;
    Canvas.Point = Vector2{ General_Offset, (window_height - Canvas.height) / 2 };

    //BelowCanvas Field
    BelowCanvas.width = Canvas.width;
    BelowCanvas.Point = Vector2{ General_Offset, Canvas.Point.y + Canvas.height + General_Offset };
    BelowCanvas.height = window_height - General_Offset - BelowCanvas.Point.y;


    //Bottom right Field
    BottomRight.width = 0; //RightToCanvas.width;
    BottomRight.height = BelowCanvas.height;
    BottomRight.Point = Vector2{ 0 /*TopRight.Point.x*/, BelowCanvas.Point.y };

}