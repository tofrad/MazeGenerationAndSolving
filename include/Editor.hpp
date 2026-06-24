#pragma once

#include <vector>
#include "../lib/raylib/include/raylib.h"
#include "ProgramCallbacks.hpp"
#include "LayoutManager.hpp"

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

	TileMap CustomMaze;

private:
	LayoutManager* layout_manager = LayoutManager::GetInstance();

	void createTileMap();
	void isValid();

	void drawGrid(int tile_size, int x_tile_offset, int y_tile_offset) const;
	
	ProgramCallbacks editor_callbacks;
	static void syncToProgram();

	//Windows and UI Size Stuff ################################################################################################################################
	int window_height = 1080;
	int window_width = 1920;

	float General_Offset = 10.0f;

	Vector2 Anchor = Vector2(General_Offset, General_Offset);
	UI_Rectangle Canvas = {	Anchor.x,
							Anchor.y,
					1660,
					1030
	};

	float padding = 10.0f;

	Rectangle Slider = {	Anchor.x,
							Anchor.y +Canvas.height +padding,
							220,
							20

	};

	Rectangle Slider_Textbox = {	Anchor.x + Slider.width + 3 *padding,
									Slider.y,
								40,
								20
	};

	Vector2 Anchor_Right_Column = Vector2(General_Offset + Canvas.width + padding, General_Offset);

	float column_width = 240.0f;
	float buttons_height = 24.0f;

	Rectangle Button_Menu = {	Anchor_Right_Column.x,
								Anchor_Right_Column.y,
								column_width,
								buttons_height
	};

	Rectangle Toggle_Group = {	Anchor_Right_Column.x,
								Button_Menu.y + buttons_height + padding,
								column_width,
								3 * buttons_height
	};

	Rectangle Button_Generate{	Anchor_Right_Column.x,
							Toggle_Group.y + 4 * Toggle_Group.height + padding,
								column_width,
								buttons_height
	};

	float Label_height = 30;

	//References for UI Elements #############################################################################################################################
	float slider_value_float = 50.0f;
	int slider_value_int = static_cast<int>(slider_value_float);
	int old_slider_value_int = static_cast<int>(slider_value_float);

	int tile_map_height = max(10, (slider_value_int / 16) * 9);

	int toggle_group = 2;

	RenderTexture2D buffer = LoadRenderTexture(window_width, window_height);

};