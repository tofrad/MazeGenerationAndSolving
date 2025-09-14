#pragma once

#include <vector>

#include "raylib.h"

#include "ProgramCallbacks.hpp"

using namespace std;

class Program;

class Editor
{

public:

	Editor();
	~Editor();

	void init(ProgramCallbacks& callbacks);

	void open();
	void close();

	void displayEditor();


private:

	TileMap CustomMaze;
	void createTileMap();

	void drawGrid(int tile_size);
	
	ProgramCallbacks callbacks;
	void syncToProgram();

	//Windows and UI Size Stuff ################################################################################################################################
	int window_height = 1080;
	int window_width = 1920;

	void calculateLandmarks();

	UI_Rectangle AboveCanvas;
	UI_Rectangle Canvas;
	UI_Rectangle BelowCanvas;

	UI_Rectangle TopRight;
	UI_Rectangle RightToCanvas;
	UI_Rectangle BottomRight;

	float General_Offset = 10; //10 px Offset

	//windows splited into parts ( 7 parts Canvas, 1 Part Right Column, with Offset from Borders and between) 
	float WindowSection = (window_width - (3 * General_Offset)) / 8;


	float Label_height = 30;

	//References for UI Elements #############################################################################################################################
	float slider_value_float = 50.0f;
	int slider_value_int = static_cast<int>(slider_value_float);
	int old_slider_value_int = static_cast<int>(slider_value_float);

	int tile_map_height = max(10, (slider_value_int / 16) * 9);

	int toggle_group = 2;

	RenderTexture2D buffer;

};