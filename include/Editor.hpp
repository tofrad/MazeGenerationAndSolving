#pragma once

#include <string>
#include <vector>
#include "../lib/raylib/include/raylib.h"
#include "ProgramCallbacks.hpp"
#include "LayoutManager.hpp"
#include "Maze_Config.hpp"

using namespace std;

class Program;

class Editor
{

public:

	Editor();
	~Editor();

	void init(const ProgramCallbacks& cb);

	static void open();
	static void close();

	void displayEditor();

	TileMap* getCustomMaze();

private:
	LayoutManager& layout_manager = LayoutManager::GetInstance();

	TileMap CustomMaze;

	void createTileMap();
	void isValid();

	void drawGrid(int tile_size, int x_tile_offset, int y_tile_offset) const;
	
	ProgramCallbacks editor_callbacks;
	static void syncToProgram();

	//Windows and UI Size Stuff ################################################################################################################################
	int window_height = 1600;
	int window_width = 900;

	void UpdateRectValues();

	Vector2 anchor01 = { 0, 0 };
	//Defining Rects
	Rectangle Canvas{anchor01.x + 16, anchor01.y + 16, 1400, 792};
	Rectangle SideRect{ anchor01.x + 1424, anchor01.y + 16, 168, 792};
	Rectangle Button_Save{anchor01.x + 1448, anchor01.y + 680, 120, 24 };
	Rectangle ButtonSaveAndGen{ anchor01.x + 1448, anchor01.y + 712, 120, 24 };
	Rectangle SizeSlider{ anchor01.x + 400, anchor01.y + 824, 576, 16 };
	Rectangle TextBoxSize{ anchor01.x + 1008, anchor01.y + 824, 56, 24 };
	//should be toggle group
	Rectangle ButtonSetStart{ anchor01.x + 1448, anchor01.y + 24, 120, 32 };
	Rectangle ButtonSetTarget{ anchor01.x + 1448, anchor01.y + 64, 120, 32 };
	Rectangle ButtonSetWall{ anchor01.x + 1448, anchor01.y + 104, 120, 32 };
	Rectangle ButtonClear{ anchor01.x + 1448, anchor01.y + 144, 120, 32 };
	Rectangle ButtonWeightMode{ anchor01.x + 1448, anchor01.y + 184, 120, 32 };

	Rectangle DividerLineWeights{ anchor01.x + 1448, anchor01.y + 224, 120, 16 };
	Rectangle TextBoxWeights{ anchor01.x + 1448, anchor01.y + 248, 120, 24 };
	Rectangle SpinnerWeightAmount{ anchor01.x + 1448, anchor01.y + 280, 120, 24 };
	Rectangle CheckBoxWithNeighbor{ anchor01.x + 1432, anchor01.y + 312, 24, 24 };
	//TextBox needed??
	Rectangle CheckBoxWithColor{ anchor01.x + 1432, anchor01.y + 344, 24, 24 };
	//Textbox needed?
	Rectangle DividerLineListView{ anchor01.x + 1448, anchor01.y + 376, 120, 16 };
	Rectangle ListViewMazeGen{ anchor01.x + 1448, anchor01.y + 400, 128, 184 };
	Rectangle ButtonGenerate{ anchor01.x + 1448, anchor01.y + 592, 120, 24 };
	Rectangle ButtonClearMaze{ anchor01.x + 1448, anchor01.y + 624, 120, 24 };
	Rectangle DividerLineGen{ anchor01.x + 1448, anchor01.y + 656, 120, 16 };
	Rectangle ButtonMenu{ anchor01.x + 1448, anchor01.y + 816, 128, 40 };

	//Just for test for now
	Rectangle StatusBarValidMaze{ anchor01.x + 1104, anchor01.y + 824, 120, 24 };

	// Scaled Values for displaying ####################################################################################
	Rectangle Scaled_Canvas = layout_manager.ScaleRect(Canvas);
	Rectangle Scaled_SideRect = layout_manager.ScaleRect(SideRect);
	Rectangle Scaled_Button_Save = layout_manager.ScaleRect(Button_Save);
	Rectangle Scaled_ButtonSaveAndGen = layout_manager.ScaleRect(ButtonSaveAndGen);
	Rectangle Scaled_SizeSlider = layout_manager.ScaleRect(SizeSlider);
	Rectangle Scaled_TextBoxSize = layout_manager.ScaleRect(TextBoxSize);
	//should be toggle group
	Rectangle Scaled_ButtonSetStart = layout_manager.ScaleRect(ButtonSetStart);
	Rectangle Scaled_ButtonSetTarget = layout_manager.ScaleRect(ButtonSetTarget);
	Rectangle Scaled_ButtonSetWall = layout_manager.ScaleRect(ButtonSetWall);
	Rectangle Scaled_ButtonClear = layout_manager.ScaleRect(ButtonClear);
	Rectangle Scaled_ButtonWeightMode = layout_manager.ScaleRect(ButtonWeightMode);

	Rectangle Scaled_DividerLineWeights = layout_manager.ScaleRect(DividerLineWeights);
	Rectangle Scaled_TextBoxWeights = layout_manager.ScaleRect(TextBoxWeights);
	Rectangle Scaled_SpinnerWeightAmount = layout_manager.ScaleRect(SpinnerWeightAmount);
	Rectangle Scaled_CheckBoxWithNeighbor = layout_manager.ScaleRect(CheckBoxWithNeighbor);
	//TextBox needed??
	Rectangle Scaled_CheckBoxWithColor = layout_manager.ScaleRect(CheckBoxWithColor);
	//Textbox needed?
	Rectangle Scaled_DividerLineListView = layout_manager.ScaleRect(DividerLineListView);
	Rectangle Scaled_ListViewMazeGen = layout_manager.ScaleRect(ListViewMazeGen);
	Rectangle Scaled_ButtonGenerate = layout_manager.ScaleRect(ButtonGenerate);
	Rectangle Scaled_ButtonClearMaze = layout_manager.ScaleRect(ButtonClearMaze);
	Rectangle Scaled_DividerLineGen = layout_manager.ScaleRect(DividerLineGen);
	Rectangle Scaled_ButtonMenu = layout_manager.ScaleRect(ButtonMenu);

	//Just for test for now
	Rectangle Scaled_StatusBarValidMaze = layout_manager.ScaleRect(StatusBarValidMaze);

	//end of scaled values #############################################################################################

	//helping functions for ui values
	void clamp_sizes_to_uneven();
	//References for UI Elements #######################################################################################
	//size
	const std::string min_width_str = std::to_string(Maze_Config::MIN_WIDTH);
	const std::string max_width_str = std::to_string(Maze_Config::MAX_WIDTH);



	float slider_value_float = 50.0f;
	int slider_value_int = static_cast<int>(slider_value_float);
	int old_slider_value_int = static_cast<int>(slider_value_float);
	//weights
	int weight_spinner_val = 1;
	bool is_with_neighbors = false;
	bool is_with_colors = false;
	//generator
	int list_view_scroll_idx = 0;
	int list_view_active = 0;

	int tile_map_width = slider_value_int;
	int tile_map_height = max(10, (tile_map_width / 16) * 9);

	std::string tile_map_width_str = std::to_string(tile_map_width);

	int toggle_group = 0;

	RenderTexture2D buffer = LoadRenderTexture(window_width, window_height);

};