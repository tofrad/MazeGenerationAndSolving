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
    GuiSlider(Rectangle{ BelowCanvas.Point.x + 2 * General_Offset, BelowCanvas.Point.y, slider_width, Label_height }, "10", "100", &slider_value_float, 10, 100);

    slider_value_int = static_cast<int>(slider_value_float);

    char slide_buffer[5]; // Statischer Puffer
    snprintf(slide_buffer, sizeof(slide_buffer), "%d", slider_value_int);

    GuiTextBox(Rectangle{ BelowCanvas.Point.x + slider_width + 5 * General_Offset, BelowCanvas.Point.y, WindowSection / 3, Label_height }, slide_buffer, 11, false);

    //GuiValueBox(Rectangle{ BelowCanvas.Point.x + slider_width +  4 * General_Offset, BelowCanvas.Point.y, WindowSection / 3, Label_height }, " ", &slider_value_int, 10, 150, false);

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

    int x_offset =  (Canvas.width - (CustomMaze.size * tile_size)) / 2;
    int y_offset =  (Canvas.height - (CustomMaze.height * tile_size)) / 2;

    //center drawing
    int x_tile_offset = Canvas.Point.x + x_offset;
    int y_tile_offset = Canvas.Point.y + y_offset;
    //((Canvas.Point.y + Canvas.height) - (CustomMaze.size * tile_size)) / 2;
    if(CheckCollisionPointRec(mouse, CanvasRect))
    {   
        //Canvas Borders right and under for bound checking
        float canvas_x_right = Canvas.Point.x + Canvas.width - x_offset;
        float canvas_y_under = Canvas.Point.y + Canvas.height - y_offset;

        //get corresponding mouse tile
        int temp_x = -1;
        int temp_y = -1;
        if (x_tile_offset < mouse.x && mouse.x < canvas_x_right &&  y_tile_offset < mouse.y && mouse.y < canvas_y_under)
        {
            temp_x = (mouse.x - x_tile_offset) / tile_size;
            temp_y = (mouse.y - y_tile_offset) / tile_size;
        }

        Point Mouse_Tile = Point(-3,-3);

        Rectangle highlighted_tile;
        bool highlight_tile = false;

        //check bounds for valid tile and define rec
        if (temp_x >= 0 && temp_y >= 0 && temp_x < CustomMaze.size && temp_y < CustomMaze.height) {
            highlighted_tile = Rectangle(x_tile_offset + temp_x * tile_size, y_tile_offset + temp_y * tile_size, tile_size, tile_size);
            Mouse_Tile = Point(temp_x, temp_y);
            highlight_tile = true;

            //show tile coord.
            char point_buffer_x[5]; // Statischer Puffer
            snprintf(point_buffer_x, sizeof(point_buffer_x), "%d", Mouse_Tile.getX());

            GuiTextBox(Rectangle{ AboveCanvas.Point.x , AboveCanvas.Point.y, WindowSection, Label_height }, point_buffer_x, 11, false);

            char point_buffer_y[5]; // Statischer Puffer
            snprintf(point_buffer_y, sizeof(point_buffer_y), "%d", Mouse_Tile.getY());

            GuiTextBox(Rectangle{ AboveCanvas.Point.x + WindowSection , AboveCanvas.Point.y, WindowSection, Label_height }, point_buffer_y, 11, false);

        }
        else {
            highlight_tile = false;

        }
        
        //handle edit events
        // TODO: capture deletion off start or target to pre check validity
        Point Invalid_Point = Point(-1, -1);
        //set walls
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 2) {

            if (Mouse_Tile == CustomMaze.Start) 
            {
                CustomMaze.Start = Point(-1, -1);
            }
            else if (Mouse_Tile == CustomMaze.Target) 
            {
                CustomMaze.Target = Point(-1, -1);
            }

            CustomMaze.TileArray[temp_x][temp_y] = 1;
        }

        //clear walls
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 3 || (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && highlight_tile)) 
        {
            if (Mouse_Tile == CustomMaze.Start)
            {
                CustomMaze.Start = Point(-1, -1);
            }
            else if (Mouse_Tile == CustomMaze.Target)
            {
                CustomMaze.Target = Point(-1, -1);
            }

            CustomMaze.TileArray[temp_x][temp_y] = 0;
        }

        //set start
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 0) {

            //if valid start exists and current tile isnt this start
            if (Mouse_Tile != CustomMaze.Start && CustomMaze.Start != Invalid_Point)
            {
                CustomMaze.TileArray[CustomMaze.Start.getX()][CustomMaze.Start.getY()] = 0;
                CustomMaze.Start = Point(-1, -1);
                
            }
            else if (Mouse_Tile == CustomMaze.Target)
            {
                CustomMaze.Target = Point(-1, -1);

            }
            
            CustomMaze.Start.setX(temp_x);
            CustomMaze.Start.setY(temp_y);

            CustomMaze.TileArray[temp_x][temp_y] = 2;



        }

        //set target
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile && toggle_group == 1) {

            if (Mouse_Tile == CustomMaze.Start)
            {
                CustomMaze.Start = Point(-1, -1);
            }//if valid target exists and current tile isnt this target
            else if (Mouse_Tile != CustomMaze.Target && CustomMaze.Target != Invalid_Point)
            {
                CustomMaze.TileArray[CustomMaze.Target.getX()][CustomMaze.Target.getY()] = 0;
                CustomMaze.Target = Point(-1, -1);
            }

            CustomMaze.Target.setX(temp_x);
            CustomMaze.Target.setY(temp_y);

            CustomMaze.TileArray[temp_x][temp_y] = 3;
        }

        drawGrid(tile_size, x_tile_offset, y_tile_offset);

        //draw highlighted tile if valid 
        if (highlight_tile) {           
            DrawRectangleLinesEx(highlighted_tile, 3, RED);
        } 
    }
    else {
        drawGrid(tile_size, x_tile_offset, y_tile_offset);
    }
}

void Editor::drawGrid(int tile_size, int x_tile_offset, int y_tile_offset) 
{
    //draw full grid
    for (int x = 0; x < CustomMaze.TileArray.size(); x++)
    {
        for (int y = 0; y < CustomMaze.TileArray[x].size(); y++)
        {
            Rectangle draw_rec = Rectangle(x_tile_offset + x * tile_size, y_tile_offset + y * tile_size, tile_size, tile_size);
            if (CustomMaze.TileArray[x][y] == 1) {
                DrawRectangleRec(draw_rec, BLACK);
                DrawRectangleLinesEx(draw_rec, 1, SKYBLUE);
            }
            else if (CustomMaze.TileArray[x][y] == 2)
            {
                DrawRectangleRec(draw_rec, BLUE);
                DrawRectangleLinesEx(draw_rec, 1, SKYBLUE);
            }
            else if (CustomMaze.TileArray[x][y] == 3)
            {
                DrawRectangleRec(draw_rec, ORANGE);
                DrawRectangleLinesEx(draw_rec, 1, SKYBLUE);
            }
            else
            {
                DrawRectangleLinesEx(draw_rec, 1, SKYBLUE);
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

    CustomMaze.Start = Point(-1, -1);
    CustomMaze.Target = Point(-1, -1);

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

void Editor::isValid()
{
    Point Invalid_Point = Point(-1, -1);
    
    if (CustomMaze.Start != Invalid_Point && CustomMaze.Target != Invalid_Point) 
    {
        CustomMaze.isValid = true;
    }
    else
    {
        CustomMaze.isValid = false;
    }


}
