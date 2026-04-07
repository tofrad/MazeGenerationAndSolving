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
    calculateLandmarks();
}

void Player::open(Recorder* Rec) {
    Record_Object = Rec;
}

void Player::close() {
    Record_Object = nullptr;
}

void Player::displayPlayerGUI()
{
    calculateLandmarks();
    ClearBackground(GRAY);

    if (GuiButton(Button_FullRewind, GuiIconText(129, ""))) { /* ACTION */ }
    if (GuiButton(Button_Rewind, GuiIconText(130, ""))) { /* ACTION */ }
    if (GuiButton(Button_Pause, GuiIconText(132, ""))) { /* ACTION */ }
    if (GuiButton(Button_Forward, GuiIconText(131, ""))) { /* ACTION */ }
    if (GuiButton(Button_FullForward, GuiIconText(134, ""))) { /* ACTION */ }

    //Menu Button
    if (GuiButton(Rectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height), GuiIconText(142, "Menu"))) { /* ACTION */ }


    if (Record_Object != nullptr) {

        stepValue = Record_Object->getStep();
        maxValue = Record_Object->getSize();
    }
    
    char maxValue_char[8];
    snprintf(maxValue_char, sizeof(maxValue_char), "%d", maxValue);


    GuiSlider(Slider, "", "", &slider_value_float, 0, (float)maxValue);

    slider_value_int = static_cast<int>(slider_value_float);
    char slide_buffer[8]; // Statischer Puffer
    snprintf(slide_buffer, sizeof(slide_buffer), "%d", slider_value_int);



    DrawRectangleRec(Slider_TextBox, RAYWHITE);
    GuiTextBox(Slider_TextBox, slide_buffer, 11, false);

    DrawRectangleRec(MaxValue_TextBox, RAYWHITE);
    GuiTextBox(MaxValue_TextBox, maxValue_char, 11, false);

    DrawRectangle(Canvas.Point.x, Canvas.Point.y, Canvas.width, Canvas.height, RAYWHITE);

    //DrawRectangle(BelowCanvas.Point.x, BelowCanvas.Point.y, BelowCanvas.width, BelowCanvas.height, RED);

    //DrawRectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height, GREEN);

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


    int Bottom_Split_x = BelowCanvas.width / 10;
    int BetweenOffset = Bottom_Split_x / 4;

    int Button_Size_x = Bottom_Split_x;

    int Bottom_Split_y = BelowCanvas.height / 9;

    int Button_Y = Bottom_Split_y * 5;
    int ButtonSize_y = Bottom_Split_y * 4;

    int Button_FirstFrame_x = 2 * Bottom_Split_x;
    int Button_Rewind_x = Button_FirstFrame_x + Button_Size_x + BetweenOffset;
    int Button_Pause_x = Button_Rewind_x + Button_Size_x + BetweenOffset;
    int Button_Resume_x = Button_Pause_x + Button_Size_x + BetweenOffset;
    int Button_LastFrame_x = Button_Resume_x + Button_Size_x + BetweenOffset;

    
    Button_FullRewind = Rectangle(Button_FirstFrame_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_Rewind = Rectangle(Button_Rewind_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_Pause = Rectangle(Button_Pause_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_Forward = Rectangle(Button_Resume_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_FullForward = Rectangle(Button_LastFrame_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);

    Slider = Rectangle(BelowCanvas.Point.x, BelowCanvas.Point.y, Bottom_Split_x * 7 ,Bottom_Split_y * 4);
    Slider_TextBox = Rectangle(BelowCanvas.Point.x + Bottom_Split_x * 8, BelowCanvas.Point.y, Bottom_Split_x, Bottom_Split_y * 4 );
    MaxValue_TextBox = Rectangle(BelowCanvas.Point.x + Bottom_Split_x * 9, BelowCanvas.Point.y, Bottom_Split_x, Bottom_Split_y * 4);

}