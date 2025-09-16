#pragma once
#define RAYGUI_IMPLEMENTATION

#include "raylib.h"

#include "Maze.hpp"
#include "Pathsolver.hpp"

#include "ProgramCallbacks.hpp"

class Menu;
class Editor;

class Program
{
	public:

		Program();
		~Program();

		void InitProgram();
		int Run();

		ProgramState getState();

		void handleStateRequest(ProgramState state);

		void handleGeneratorRequest(int size, GenerationMethod method);
		void handleSolveRequest(SolvingMethod method);

		void handleWindowChange(Screensize size);

	private:

		void setState(ProgramState next_state);

		ProgramCallbacks createCallbacks();

		Screensize Windowsize = FHD;

		GenerationMethod Generator = REC_BACKTRACKING;

		SolvingMethod Solver = SM_BFS;

		ProgramState State = STOPPED;

		ProgramState LastState = MENU;

		int screenWidth = 1920;
		int screenHeight = 1080;

		int buffer_width = 1920;
		int buffer_height = 1080;

		int MazeSize = 50;

		Maze M;

		Pathsolver S;

		Menu* menu;

		Editor* editor;

		int FrameRate = 120;

		float scale = 1.0f;

		void centerWindow();

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

