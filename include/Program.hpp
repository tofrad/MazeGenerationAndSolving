#pragma once
#define RAYGUI_IMPLEMENTATION

#include "../lib/raylib/include/raylib.h"
#include "Maze.hpp"
#include "Pathsolver.hpp"
#include "ProgramCallbacks.hpp"

#define MAX_HEIGHT 249
#define MAX_WIDTH 249

#define MIN_HEIGHT 11
#define MIN_WIDTH 11

class Menu;
class Editor;
class Player;

class Program
{
	public:

		Program();
		~Program();

		void InitProgram();
		int Run();

		ProgramState getState() const;

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

		int MazeWidth = 40;
		int MazeHeight = 10;

		Recorder Gen_Recorder;
		Maze M;

		Recorder Solve_Recorder;
		Pathsolver S;

		Menu* menu;

		Editor* editor;

		Player* player;

		int FrameRate = 30;

		float scale = 1.0f;

		void centerWindow() const;

		//buffer for displaying program to window
		RenderTexture2D buffer;

		//buffer to save last maze frame
		RenderTexture2D last_maze_buffer;

		//buffer to save last path frame
		RenderTexture2D last_path_buffer;

		Rectangle source;

		void getLastMazeFrame() const;
		void getLastPathFrame() const;

		void CalculateMazeParams(const int width);

};

