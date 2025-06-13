#pragma once

#include "Maze.hpp"
#include "Pathsolver.hpp"

#include "raylib.h"

typedef enum {
	RUNNING,
	STOPPED,
	MENU,
	EDITING

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

		int MazeSize = 100;

		Maze M;

		Pathsolver S;

		int FrameRate = 60;

		float scale = 1.0f;

		//buffer for displaying program to window
		RenderTexture2D buffer;

		//source to save the displayed maze in 
		Rectangle source;

};

