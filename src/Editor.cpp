#include "Editor.hpp"
#include <iostream>
#include "raygui.h"
#include "Maze_Config.hpp"
#include "Global_Config.hpp"

#include "UI_Config.hpp"

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

    GuiLine(Scaled_DividerLineWeights,"weights");

    GuiTextBox(Scaled_TextBoxWeights, "weight amount",DEFAULT, false);

    //bug in raygui only limits constraints in false
    GuiSpinner(Scaled_SpinnerWeightAmount, "", &weight_spinner_val, 1, 20, false);

    GuiCheckBox(Scaled_CheckBoxWithNeighbor, "neighbor degr.", &is_with_neighbors);

    GuiCheckBox(Scaled_CheckBoxWithColor, "show colors", &is_with_colors);

    GuiLine(Scaled_DividerLineListView,"Maze Gen");

    GuiListView(Scaled_ListViewMazeGen, Maze_Config::GEN_INPUT, &list_view_scroll_idx, &list_view_active);

    if (GuiButton(Scaled_ButtonGenerate, "Generate"))
    {
        editor_callbacks.onCustomGenerateRequest(CustomMaze, static_cast<GenerationMethod>(list_view_active));
    }
    if (GuiButton(Scaled_ButtonClearMaze, "Reset"))
    {
        //clear whole tile map
        createTileMap();
    }
    if (GuiButton(Scaled_ButtonSaveAndGen, "Save & Gen"))
    {
        isValid();
        //TODO
        //check valid maze
        if (CustomMaze.isValid)
        {
            editor_callbacks.onGenerateRequest(slider_value_int, GenerationMethod::CUSTOM, 0, 0);
        }

    }

    GuiLine(Scaled_DividerLineGen, "");

    if (CustomMaze.isValid)
    {
        GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, greenHex);
    }else
    {
        GuiSetStyle(STATUSBAR, BASE_COLOR_NORMAL, redHex);
    }
    GuiStatusBar(Scaled_StatusBarValidMaze, "Maze Validity");

    GuiSlider(Scaled_SizeSlider, Maze_Config::MIN_W_STR.c_str(), Maze_Config::MAX_W_STR.c_str(),
                                &slider_value_float,
                                Maze_Config::MIN_WIDTH, Maze_Config::MAX_WIDTH);

    slider_value_int = static_cast<int>(slider_value_float);

    if (slider_value_int != old_slider_value_int) 
    {
        createTileMap();
        clamp_sizes_to_uneven();
        old_slider_value_int = slider_value_int;
    }
    GuiTextBox(Scaled_TextBoxSize, const_cast<char*>(tile_map_width_str.c_str()), 11, false);

    // Check Canvas Interact #################################################################################################################################################################
    const auto CanvasRect = Scaled_Canvas;

    const Vector2  mouse = GetMousePosition();

    const int tile_size = min(static_cast<int>(CanvasRect.width) / slider_value_int,
                              static_cast<int>(CanvasRect.height) / tile_map_height);

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
        bool highlight_tile_exists = false;

        //check bounds for valid tile and define rec
        if (temp_x >= 0 && temp_y >= 0 && temp_x < CustomMaze.size && temp_y < CustomMaze.height) {
            highlighted_tile = (Rectangle(x_tile_offset + temp_x * tile_size, y_tile_offset + temp_y * tile_size, tile_size, tile_size));
            Mouse_Tile = Point(temp_x, temp_y);
            highlight_tile_exists = true;
        }
        else {
            highlight_tile_exists = false;
        }
        
        //handle edit events
        // TODO: capture deletion off start or target to pre check validity
        const Point Invalid_Point = Point(-1, -1);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && highlight_tile_exists)
        {
            switch (toggle_group)
            {
            case 0: //set start
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
                CustomMaze.TileArray[temp_x][temp_y] = START_CHAR;
                break;

            case 1: //set target
                //if valid target exists and current tile isn't this target
                if (Mouse_Tile != CustomMaze.Target && CustomMaze.Target != Invalid_Point)
                {
                    CustomMaze.TileArray[CustomMaze.Target.getX()][CustomMaze.Target.getY()] = 0;
                    CustomMaze.Target = Point(-1, -1);

                }
                else if (Mouse_Tile == CustomMaze.Start)
                {
                    CustomMaze.Start = Point(-1, -1);
                }
                CustomMaze.Target.setX(temp_x);
                CustomMaze.Target.setY(temp_y);
                CustomMaze.TileArray[temp_x][temp_y] = TARGET_CHAR;

                break;
            case 2: // set wall
                if (Mouse_Tile == CustomMaze.Start)
                {
                    CustomMaze.Start = Point(-1, -1);
                }
                else if (Mouse_Tile == CustomMaze.Target)
                {
                    CustomMaze.Target = Point(-1, -1);
                }

                CustomMaze.TileArray[temp_x][temp_y] = WALL_CHAR;
                break;

            case 3: // clear cell
                if (Mouse_Tile == CustomMaze.Start)
                {
                    CustomMaze.Start = Point(-1, -1);
                }
                else if (Mouse_Tile == CustomMaze.Target)
                {
                    CustomMaze.Target = Point(-1, -1);
                }
                CustomMaze.TileArray[temp_x][temp_y] = 0;
                break;

            case 4: // add weights
                if (Mouse_Tile == CustomMaze.Start)
                {
                    //no weight to start
                }
                else if (Mouse_Tile == CustomMaze.Target)
                {
                    //no weight to target
                }
                //add weight logic here
                handleWeightAdd(temp_x, temp_y, weight_spinner_val);
                break;

            default:
                break;
            }

        //TODO
        //handle weight behav. here

        //right click delete
        }else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && highlight_tile_exists)
        {
            if (Mouse_Tile == CustomMaze.Start)
            {
                CustomMaze.Start = Point(-1, -1);
            }else if (Mouse_Tile == CustomMaze.Target)
            {
                CustomMaze.Target = Point(-1, -1);
            }
            CustomMaze.TileArray[temp_x][temp_y] = 0;
        }

        drawGrid(tile_size, x_tile_offset, y_tile_offset);

        //draw highlighted tile if valid 
        if (highlight_tile_exists) {
            DrawRectangleLinesEx(highlighted_tile, 3, RED);
        }

    }
    else {
        drawGrid(tile_size, x_tile_offset, y_tile_offset);
    }
}

TileMap* Editor::getCustomMaze()
{
    return &CustomMaze;
}

void Editor::drawGrid(const int tile_size, const int x_tile_offset, const int y_tile_offset) const
{
    constexpr Color GridColor = BLACK;
    //draw full grid
    for (int x = 0; x < CustomMaze.TileArray.size(); x++)
    {
        for (int y = 0; y < CustomMaze.TileArray[x].size(); y++)
        {
            const Rectangle draw_rec = (Rectangle(x_tile_offset + x * tile_size, y_tile_offset + y * tile_size, tile_size,
                                           tile_size));
            if (CustomMaze.TileArray[x][y] == WALL_CHAR) {
                DrawRectangleRec(draw_rec, UI_Config::WALL_COLOR);
                DrawRectangleLinesEx(draw_rec, 1, GridColor);
            }
            else if (CustomMaze.TileArray[x][y] == START_CHAR)
            {
                DrawRectangleRec(draw_rec, BLUE);
                DrawRectangleLinesEx(draw_rec, 1, GridColor);
            }
            else if (CustomMaze.TileArray[x][y] == TARGET_CHAR)
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
    Scaled_TextBoxSize = layout_manager.ScaleRect(TextBoxSize);
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
//pre-check maze constraints
void Editor::clamp_sizes_to_uneven()
{
    if (slider_value_int % 2 == 0)
    {
        tile_map_width = slider_value_int - 1;
    }else
    {
        tile_map_width = slider_value_int;
    }

    tile_map_height = (tile_map_width / 16) * 9;
    if (tile_map_height % 2 == 0)
    {
        tile_map_height = tile_map_height - 1;
    }
    tile_map_width = std::clamp(tile_map_width, Maze_Config::MIN_WIDTH, Maze_Config::MAX_WIDTH);
    tile_map_height = std::clamp(tile_map_height, Maze_Config::MIN_HEIGHT, Maze_Config::MAX_HEIGHT);

    //update original dependencies
    slider_value_int = tile_map_width;
    slider_value_float = static_cast<float>(tile_map_width);
    tile_map_width_str = std::to_string(tile_map_width);
}

void Editor::handleWeightAdd(const int x, const int y, const int weight)
{
    CustomMaze.TileArray[x][y] = weight;

    //TODO
    // add progressive weight decrease
    //
    // if (is_with_neighbors)
    // {
    //     int max_y = CustomMaze.TileArray.size();
    //     int max_x = CustomMaze.TileArray[0].size();
    //
    //     //how often decrease by ten to get to 0
    //     int number_of_iterations = weight / 10;
    //
    //     vector<tuple<int, int>> temp{};
    //
    // }
}

void Editor::createTileMap()
{
    clamp_sizes_to_uneven();

    CustomMaze.Start = Point(-1, -1);
    CustomMaze.Target = Point(-1, -1);

    CustomMaze.size = tile_map_width;
    CustomMaze.height = tile_map_height;

    CustomMaze.TileArray.clear();

    for (int w = 0; w < tile_map_width ; w++)
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
        //callback to generate maze and bfs
        CustomMaze.isValid = true;
    }
    else
    {
        CustomMaze.isValid = false;
    }
}
