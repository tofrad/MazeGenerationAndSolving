#pragma once

#include "Editor.hpp"
#include "raygui.h"

#include <iostream>

Editor::Editor()
{

}

Editor::~Editor()
{

}

void Editor::init(ProgramCallbacks& callbacks)
{
	this->callbacks = callbacks;

}

void Editor::open() 
{
    //SetTargetFPS(60);
}

void Editor::close()
{
    //SetTargetFPS(30);
}

void Editor::displayEditor() 
{
	ClearBackground(GRAY);

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

    float Labelwidth = (window_width - (3 * Border_Offset)) / 8;
    float Label_height = 24;

    float top_left_colum_x = window_width - Border_Offset - Labelwidth;

    if (GuiButton(Rectangle{ top_left_colum_x, Labelpos_y, Labelwidth, Label_height }, "Exit to Menu")) {

        callbacks.onStateRequest(MENU);
    }

    if (GuiButton(Rectangle{ top_left_colum_x, window_height - Border_Offset - Label_height, Labelwidth, Label_height }, "Generate")) {

        //get state of editor 
        //check if state is valid and able to convert in solveable Maze
        //send Costum Maze Request to Program
        callbacks.onStateRequest(MENU);
    }

    //separate buttons with newline, #x# exchange x for icons
    //0 = Start, 1 = Target, 2 = Set Wall, 3 = clear Wall
    GuiToggleGroup(Rectangle{ top_left_colum_x, Labelpos_y + 4 * (Border_Offset + Label_height), Labelwidth, Label_height }, "Start\nTarget\nSet\nClear", &toggle_group); 

    float canvas_width = 7 * Labelwidth;

    float canvas_height = (canvas_width / 16) * 9;

    float top_offset_canvas = (window_height - canvas_height) / 2;
    
    
    float x_below_canvas = 2 * Border_Offset;

    GuiSlider(Rectangle{ x_below_canvas, top_offset_canvas + canvas_height + Border_Offset, 2*Labelwidth, Label_height }, "10", "100", &slider_value_float, 10, 100);

    slider_value_int = static_cast<int>(slider_value_float);

    x_below_canvas = x_below_canvas + 2 * Labelwidth + 3*Border_Offset;

    GuiValueBox(Rectangle{ x_below_canvas , top_offset_canvas + canvas_height + Border_Offset, Labelwidth/3, Label_height }, " ",&slider_value_int, 10, 100, false);



    Vector2  mouse = GetMousePosition();

    tile_map_height = (slider_value_int / 16) * 9;

    int tilesize = min(canvas_width / slider_value_int, canvas_height / tile_map_height);

    Rectangle CanvasRect = Rectangle{ Border_Offset, top_offset_canvas, canvas_width, canvas_height };

    DrawRectangleRec(CanvasRect, LIGHTGRAY);

    if(CheckCollisionPointRec(mouse, CanvasRect))
    {
        DrawCircleV(mouse, 15, PURPLE);
    }

    //create a canvas as rectangle obj
    //create buffer to draw in from canvas rect
    //check collision with rect and mouse
    //calculate coordinates in canvas
    //calculate coordinates to grid 
    //observe click event and editmode from togglegroup

    //do changes at current posiion
    //update grid obj
    //draw grid in buffer    

    
}
void calculateLandmarks() 
{

}
void Editor::syncToProgram() 
{

}

void Editor::createTileMap()
{
    for (int h = 0; h < tile_map_height; h++) 
    {
        for(int w = 0; w < slider_value_int; w++)
        {
            tile_map[w][h] = 0;
        }

    }
}
