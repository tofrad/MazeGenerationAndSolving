#pragma once

#include "raylib.h"

#include "Maze.hpp"
#include "Pathsolver.hpp"
#include "Menu.hpp"

#include "State_Definitions.hpp"

class Program
{
	public:

		Program();
		~Program();

		void InitProgram();
		int Run();

		void setState(ProgramState next_state);
		ProgramState getState();

		void updateMaze(int size, GenerationMethod method);
		void updatePath(SolvingMethod method);

		


	private:

		ProgramState State = STOPPED;

		ProgramState LastState = MENU;

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

