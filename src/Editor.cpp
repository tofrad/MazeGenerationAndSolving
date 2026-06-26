#include "Editor.hpp"
#include <iostream>
#include "raygui.h"


Editor::Editor()
{

}

Editor::~Editor()
{

}

void Editor::init(const ProgramCallbacks& cb)
{
    this->editor_callbacks = cb;
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
    layout_manager.UpdateScale();
    UpdateRectValues();

	ClearBackground(LIGHTGRAY);
    // draw ui stuff ###################################################################################################
    DrawRectangleRec(Scaled_Canvas, LIGHTGRAY);
    DrawRectangleLinesEx(Scaled_Canvas, 3,  BLACK);
    DrawRectangleLinesEx(Scaled_SideRect, 3, BLACK);


    if (GuiButton(Scaled_ButtonMenu, "Menu") ){

        editor_callbacks.onStateRequest(MENU);
    }

    //0 = Start, 1 = Target, 2 = Set Wall, 3 = clear Wall 4 change to weight mode
    GuiToggleGroup(Scaled_ButtonSetStart, "Start\nTarget\nSet Wall\nClear\nWeights", &toggle_group);
    //Bottom right ###########################################################################################################################################################################

    if (GuiButton(Scaled_ButtonGenerate, "Generate")) {

        //get state of editor 
        //check if state is valid and able to convert in solvable Maze
        //send costum Maze Request to Program
        editor_callbacks.onStateRequest(MENU);
    }
    GuiLine(Scaled_DividerLineWeights,"weights");

    GuiTextBox(Scaled_TextBoxWeights, "weight amount",DEFAULT, false);

    int weight_spinner_val = 1;
    GuiSpinner(Scaled_SpinnerWeightAmount, "", &weight_spinner_val, 1, 20, true);

    bool is_with_neighbors = false;
    GuiCheckBox(Scaled_CheckBoxWithNeighbor, "neighbor degr.", &is_with_neighbors);

    bool is_with_colors = false;
    GuiCheckBox(Scaled_CheckBoxWithColor, "show colors", &is_with_colors);

    GuiLine(Scaled_DividerLineListView,"Maze Gen");

    int list_view_scroll_idx = 0;
    int list_view_active = 0;
    GuiListView(Scaled_ListViewMazeGen, "Recursive Backtracking\nKruskal\nHunt&Kill", &list_view_scroll_idx, &list_view_active);

    if (GuiButton(Scaled_ButtonGenerate, "Generate")) {}
    if (GuiButton(Scaled_ButtonClearMaze, "Reset")) {}

    GuiLine(Scaled_DividerLineGen, "");

    GuiStatusBar(Scaled_StatusBarValidMaze, "Maze Validity");


    GuiSlider(Scaled_SizeSlider, "10", "100", &slider_value_float, 10, 100);

    slider_value_int = static_cast<int>(slider_value_float);

    char slide_buffer[5];
    snprintf(slide_buffer, sizeof(slide_buffer), "%d", slider_value_int);

    // TODO
    //needs coords
    //GuiTextBox(layout_manager.ScaleRect(Slider_Textbox), slide_buffer, 11, false);

    if (slider_value_int != old_slider_value_int) 
    {
        createTileMap();
        old_slider_value_int = slider_value_int;
    }

    // Check Canvas Interact #################################################################################################################################################################

    const auto CanvasRect = Scaled_Canvas;

    const Vector2  mouse = GetMousePosition();

    const int tile_size = min(static_cast<int>(CanvasRect.width - General_Offset) / slider_value_int,
                              static_cast<int>(CanvasRect.height - General_Offset) / tile_map_height);

    const int x_offset =  (CanvasRect.width - (CustomMaze.size * tile_size)) / 2;
    const int y_offset =  (CanvasRect.height - (CustomMaze.height * tile_size)) / 2;

    //center drawing
    const int x_tile_offset = CanvasRect.x + x_offset;
    const int y_tile_offset = CanvasRect.y + y_offset;

    //Canvas Borders right and under for bound checking
    const float canvas_x_right = x_tile_offset + (CustomMaze.size * tile_size);
    const float canvas_y_under = y_tile_offset + (CustomMaze.height * tile_size);

    if(CheckCollisionPointRec(mouse, CanvasRect))
    {
        //get corresponding mouse tile
        int temp_x = -1;
        int temp_y = -1;
        if (x_tile_offset < mouse.x && mouse.x < canvas_x_right &&  y_tile_offset < mouse.y && mouse.y < canvas_y_under)
        {
            temp_x = (mouse.x - x_tile_offset) / tile_size;
            temp_y = (mouse.y - y_tile_offset) / tile_size;
        }

        Point Mouse_Tile = Point(-3,-3);

        Rectangle highlighted_tile = {};
        bool highlight_tile = false;

        //check bounds for valid tile and define rec
        if (temp_x >= 0 && temp_y >= 0 && temp_x < CustomMaze.size && temp_y < CustomMaze.height) {
            highlighted_tile = (Rectangle(x_tile_offset + temp_x * tile_size, y_tile_offset + temp_y * tile_size, tile_size, tile_size));
            Mouse_Tile = Point(temp_x, temp_y);
            highlight_tile = true;

            //show tile coord.
            char point_buffer_x[5];
            snprintf(point_buffer_x, sizeof(point_buffer_x), "%d", Mouse_Tile.getX());

            // GuiTextBox(Rectangle{ AboveCanvas.Point.x , AboveCanvas.Point.y, WindowSection, Label_height }, point_buffer_x, 11, false);

            char point_buffer_y[5];
            snprintf(point_buffer_y, sizeof(point_buffer_y), "%d", Mouse_Tile.getY());

            // GuiTextBox(Rectangle{ AboveCanvas.Point.x + WindowSection , AboveCanvas.Point.y, WindowSection, Label_height }, point_buffer_y, 11, false);

        }
        else {
            highlight_tile = false;
        }
        
        //handle edit events
        // TODO: capture deletion off start or target to pre check validity
        const Point Invalid_Point = Point(-1, -1);
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

            //if valid start exists and current tile isn't this start
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
            }//if valid target exists and current tile isn't this target
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

        this->isValid();

    }
    else {
        drawGrid(tile_size, x_tile_offset, y_tile_offset);
    }
}

void Editor::drawGrid(const int tile_size, const int x_tile_offset, const int y_tile_offset) const
{
    Color GridColor = BLACK;
    //draw full grid
    for (int x = 0; x < CustomMaze.TileArray.size(); x++)
    {
        for (int y = 0; y < CustomMaze.TileArray[x].size(); y++)
        {
            const Rectangle draw_rec = (Rectangle(x_tile_offset + x * tile_size, y_tile_offset + y * tile_size, tile_size,
                                           tile_size));
            if (CustomMaze.TileArray[x][y] == 1) {
                DrawRectangleRec(draw_rec, BLACK);
                DrawRectangleLinesEx(draw_rec, 1, GridColor);
            }
            else if (CustomMaze.TileArray[x][y] == 2)
            {
                DrawRectangleRec(draw_rec, BLUE);
                DrawRectangleLinesEx(draw_rec, 1, GridColor);
            }
            else if (CustomMaze.TileArray[x][y] == 3)
            {
                DrawRectangleRec(draw_rec, ORANGE);
                DrawRectangleLinesEx(draw_rec, 1, GridColor);
            }
            else
            {
                DrawRectangleLinesEx(draw_rec, 1, GridColor);
            }
        }
    }

}

void Editor::syncToProgram() 
{
    
}

void Editor::UpdateRectValues()
{
         Scaled_Canvas = layout_manager.ScaleRect(Canvas);
         Scaled_SideRect = layout_manager.ScaleRect(SideRect);
         Scaled_Button_Save = layout_manager.ScaleRect(Button_Save);
         Scaled_ButtonSaveAndGen = layout_manager.ScaleRect(ButtonSaveAndGen);
         Scaled_SizeSlider = layout_manager.ScaleRect(SizeSlider);

         Scaled_ButtonSetStart = layout_manager.ScaleRect(ButtonSetStart);
         Scaled_ButtonSetTarget = layout_manager.ScaleRect(ButtonSetTarget);
         Scaled_ButtonSetWall = layout_manager.ScaleRect(ButtonSetWall);
         Scaled_ButtonClear = layout_manager.ScaleRect(ButtonClear);
         Scaled_ButtonWeightMode = layout_manager.ScaleRect(ButtonWeightMode);
         Scaled_DividerLineWeights = layout_manager.ScaleRect(DividerLineWeights);
         Scaled_TextBoxWeights = layout_manager.ScaleRect(TextBoxWeights);
         Scaled_SpinnerWeightAmount = layout_manager.ScaleRect(SpinnerWeightAmount);
         Scaled_CheckBoxWithNeighbor = layout_manager.ScaleRect(CheckBoxWithNeighbor);

         Scaled_CheckBoxWithColor = layout_manager.ScaleRect(CheckBoxWithColor);

         Scaled_DividerLineListView = layout_manager.ScaleRect(DividerLineListView);
         Scaled_ListViewMazeGen = layout_manager.ScaleRect(ListViewMazeGen);
         Scaled_ButtonGenerate = layout_manager.ScaleRect(ButtonGenerate);
         Scaled_ButtonClearMaze = layout_manager.ScaleRect(ButtonClearMaze);
         Scaled_DividerLineGen = layout_manager.ScaleRect(DividerLineGen);
         Scaled_ButtonMenu = layout_manager.ScaleRect(ButtonMenu);

         Scaled_StatusBarValidMaze = layout_manager.ScaleRect(StatusBarValidMaze);

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
    const Point Invalid_Point = Point(-1, -1);

    if (CustomMaze.Start != Invalid_Point && CustomMaze.Target != Invalid_Point)
    {
        CustomMaze.isValid = true;
    }
    else
    {
        CustomMaze.isValid = false;
    }
}
