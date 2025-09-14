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
    calculateLandmarks();

	ClearBackground(GRAY);


    float Labelpos_y = 10;

    //Above Canvas ############################################################################################################################################################################



    //Top Right ###############################################################################################################################################################################

    if (GuiButton(Rectangle{ TopRight.Point.x , TopRight.Point.y , WindowSection, Label_height }, "Exit to Menu")) {

        callbacks.onStateRequest(MENU);
    }

    //Right to Canvas #########################################################################################################################################################################
    
    //separate buttons with newline, #x# exchange x for icons
    //0 = Start, 1 = Target, 2 = Set Wall, 3 = clear Wall
    //height describes height off each button
    GuiToggleGroup(Rectangle{ RightToCanvas.Point.x, RightToCanvas.Point.y, RightToCanvas.width, (RightToCanvas.height - 4 * General_Offset) / 4 }, "Start\nTarget\nSet\nClear", &toggle_group);

    //Bottom right ###########################################################################################################################################################################

    if (GuiButton(Rectangle{ BottomRight.Point.x, BottomRight.Point.y + (BottomRight.height - Label_height), BottomRight.width, Label_height }, "Generate")) {

        //get state of editor 
        //check if state is valid and able to convert in solveable Maze
        //send Costum Maze Request to Program
        callbacks.onStateRequest(MENU);
    }

    // Below Canvas ##########################################################################################################################################################################

    float slider_width = 2 * WindowSection;
    GuiSlider(Rectangle{ BelowCanvas.Point.x + 2 * General_Offset, BelowCanvas.Point.y, slider_width, Label_height }, "10", "150", &slider_value_float, 10, 150);

    slider_value_int = static_cast<int>(slider_value_float);

    GuiValueBox(Rectangle{ BelowCanvas.Point.x + slider_width +  4 * General_Offset, BelowCanvas.Point.y, WindowSection / 3, Label_height }, " ", &slider_value_int, 10, 150, false);

    if (slider_value_int != old_slider_value_int) 
    {
        createTileMap();
        old_slider_value_int = slider_value_int;

    }

    // Check Canvas Interact #################################################################################################################################################################

    Rectangle CanvasRect = Rectangle{ Canvas.Point.x, Canvas.Point.y, Canvas.width, Canvas.height };

    DrawRectangleRec(CanvasRect, LIGHTGRAY);

    Vector2  mouse = GetMousePosition();

    int tile_size = min((int)(Canvas.width - General_Offset) / slider_value_int, (int)(Canvas.height - General_Offset) / tile_map_height);

    if(CheckCollisionPointRec(mouse, CanvasRect))
    {
        
        //center drawing
        int x_tile_offset = General_Offset + ((Canvas.width - (CustomMaze.size * tile_size)) / 2);
        int y_tile_offset = Canvas.Point.y + ((Canvas.height - (CustomMaze.height * tile_size)) / 2);
        
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

        drawGrid(tile_size);

        //draw highlighted tile if valid 
        if (highlight_tile) {           
            DrawRectangleLinesEx(highlighted_tile, 3, RED);
        } 
    }
    else {
        drawGrid(tile_size);
    }
}

void Editor::drawGrid(int tile_size) 
{
    int x_tile_offset = General_Offset + ((Canvas.width - (CustomMaze.size * tile_size)) / 2);
    int y_tile_offset = Canvas.Point.y + ((Canvas.height - (CustomMaze.height * tile_size)) / 2);
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
            else if (CustomMaze.TileArray[x][y] == 2)
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

}
void Editor::calculateLandmarks() 
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

    //Above Canvas Field
    AboveCanvas.width = Canvas.width;
    AboveCanvas.height = Canvas.Point.y - 2 * General_Offset;
    AboveCanvas.Point = Vector2{General_Offset, General_Offset};

    //BelowCanvas Field
    BelowCanvas.width = Canvas.width;
    BelowCanvas.Point = Vector2{ General_Offset, Canvas.Point.y + Canvas.height + General_Offset };
    BelowCanvas.height = window_height - General_Offset - BelowCanvas.Point.y ;

    //Top Right Field
    TopRight.width = 1 * WindowSection;
    TopRight.height = AboveCanvas.height;
    TopRight.Point = Vector2{ AboveCanvas.Point.x + AboveCanvas.width + General_Offset, General_Offset};

    //Field right to Canvas
    RightToCanvas.width = TopRight.width;
    RightToCanvas.height = Canvas.height;
    RightToCanvas.Point = Vector2{ TopRight.Point.x , Canvas.Point.y };

    //Bottom right Field
    BottomRight.width = RightToCanvas.width;
    BottomRight.height = BelowCanvas.height;
    BottomRight.Point = Vector2{TopRight.Point.x, BelowCanvas.Point.y};

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
