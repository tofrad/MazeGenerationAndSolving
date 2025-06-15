#pragma once

#include "Maze.hpp"
#include "Pathsolver.hpp"
#include "Menu.hpp"

#include "raylib.h"

typedef enum {
	IDLE,
	STOPPED,
	MENU,
	EDITING,
	PLAY_MAZE,
	PLAY_PATH

} ProgramState;

class Program
{
	public:

		Program();
		~Program();

		void InitProgram();
		int Run();

		void setState(ProgramState next_state);
		ProgramState State = STOPPED;


	private:

		int screenWidth = 1920;
		int screenHeight = 1080;

		int MazeSize = 50;

		Maze M;

		Pathsolver S;

		Menu menu;  

		int FrameRate = 60;

		float scale = 1.0f;

		//buffer for displaying program to window
		RenderTexture2D buffer;

		//buffer to save last maze frame
		RenderTexture2D last_maze_buffer;

		//buffer to save last path frame
		RenderTexture2D last_path_buffer;

		Rectangle source;

		void saveLastFrame();
		void getLastMazeFrame();
		void getLastPathFrame();

		

};

