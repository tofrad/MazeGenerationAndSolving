#pragma once

#include "Maze.hpp"
#include "Pathsolver.hpp"

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

		ProgramState State = STOPPED;


	private:

		int screenWidth = 1920;
		int screenHeight = 1080;

		int MazeSize = 50;

		Maze M;

		Pathsolver S;

		int FrameRate = 60;

		float scale = 1.0f;

		//buffer for displaying program to window
		RenderTexture2D buffer;

		Rectangle source;

		void setState(ProgramState next_state);

};

