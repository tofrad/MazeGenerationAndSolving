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

	ProgramCallbacks callbacks;

	void syncToProgram();

	vector<vector<char>> tile_map;
	void createTileMap();

	int tile_map_height = 0;

	int window_height = 1080;
	int window_width = 1920;

	float Border_Offset = 10;

	float slider_value_float = 50.0f;
	int slider_value_int = static_cast<int>(slider_value_float);
	int old_slider_value_int = static_cast<int>(slider_value_float);

	int toggle_group = 2;

	RenderTexture2D buffer;

};