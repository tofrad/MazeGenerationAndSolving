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

    createTileMap();

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

    x_below_canvas = x_below_canvas + 2 * Labelwidth + 3*Border_Offset;

    slider_value_int = static_cast<int>(slider_value_float);
    GuiValueBox(Rectangle{ x_below_canvas , top_offset_canvas + canvas_height + Border_Offset, Labelwidth/3, Label_height }, " ",&slider_value_int, 10, 100, false);

    if (slider_value_int != old_slider_value_int) 
    {
        createTileMap();
        old_slider_value_int = slider_value_int;

    }

    Rectangle CanvasRect = Rectangle{ Border_Offset, top_offset_canvas, canvas_width, canvas_height };

    DrawRectangleRec(CanvasRect, LIGHTGRAY);

    Vector2  mouse = GetMousePosition();

    if(CheckCollisionPointRec(mouse, CanvasRect))
    {
        int tile_size = min((int)(canvas_width - Border_Offset) / slider_value_int, (int)(canvas_height - Border_Offset) / tile_map_height);

        //center drawing
        int x_tile_offset = Border_Offset + ((canvas_width - (CustomMaze.size * tile_size)) / 2);
        int y_tile_offset = top_offset_canvas + ((canvas_height - (CustomMaze.height * tile_size)) / 2);
        
        //get corresponding mouse tile
        int temp_x = -1;
        int temp_y = -1;
        if (mouse.x > x_tile_offset) 
        {
            temp_x = (mouse.x - x_tile_offset) / tile_size;
        }
        if (mouse.y > y_tile_offset)
        {
            temp_y = (mouse.y - y_tile_offset) / tile_size;
        }

        Rectangle highlighted_tile;
        bool highlight_tile = false;

        //check bounds for valid tile and define rec
        if (temp_x >= 0 && temp_y >= 0 && temp_x < CustomMaze.size && temp_y < CustomMaze.height) {
            highlighted_tile = Rectangle(x_tile_offset + temp_x * tile_size, y_tile_offset + temp_y * tile_size, tile_size, tile_size);
            highlight_tile = true;
        }
        else {
            highlight_tile = false;
        }
      

        //handle edit events
        // TODO: capture deletion off start or target to pre check validity

        //set walls
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 2) {

            CustomMaze.TileArray[temp_x][temp_y] = 1;
        }

        //clear walls
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 3 || (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && highlight_tile)) 
        {
            CustomMaze.TileArray[temp_x][temp_y] = 0;
        }

        //set start
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 0) {

            CustomMaze.Start.setX(temp_x);
            CustomMaze.Start.setY(temp_y);

            CustomMaze.TileArray[temp_x][temp_y] = 2;
        }

        //set target
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 1) {

            CustomMaze.Target.setX(temp_x);
            CustomMaze.Target.setY(temp_y);

            CustomMaze.TileArray[temp_x][temp_y] = 3;
        }

        //draw full grid
        for (int x = 0; x < CustomMaze.TileArray.size(); x++)
        {
            for (int y = 0; y < CustomMaze.TileArray[x].size(); y++)
            {
                Rectangle temp_rec = Rectangle(x_tile_offset + x * tile_size, y_tile_offset + y * tile_size, tile_size, tile_size);
                if (CustomMaze.TileArray[x][y] == 1) {
                    DrawRectangleRec(temp_rec, BLACK);
                    DrawRectangleLinesEx(temp_rec, 1, SKYBLUE);
                }
                else if(CustomMaze.TileArray[x][y] == 2)
                {
                    DrawRectangleRec(temp_rec, BLUE);
                    DrawRectangleLinesEx(temp_rec, 1, SKYBLUE);
                }
                else if (CustomMaze.TileArray[x][y] == 3)
                {
                    DrawRectangleRec(temp_rec, ORANGE);
                    DrawRectangleLinesEx(temp_rec, 1, SKYBLUE);
                }
                else 
                {
                    DrawRectangleLinesEx(temp_rec, 1, SKYBLUE);
                }
                

            }
        }

        //draw highlighted tile if valid 
        if (highlight_tile) {           
            DrawRectangleLinesEx(highlighted_tile, 3, RED);
        }
        
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
    tile_map_height = max(10, (slider_value_int / 16) * 9);

    CustomMaze.size = slider_value_int;
    CustomMaze.height = tile_map_height;

    CustomMaze.TileArray.clear();

    for (int w = 0; w < slider_value_int ; w++)
    {
        vector<char> temp(tile_map_height, 0); 
        
        CustomMaze.TileArray.push_back(temp);

    }
}
