#pragma once

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

		int MazeWidth = 60;
		int MazeHeight = 10;

		Recorder Gen_Recorder;
		Maze M;

		Recorder Solve_Recorder;
		Pathsolver S;

		Menu* menu;

		Editor* editor;

		Player* player;

		int FrameRate = 30;

		void centerWindow() const;

		void CalculateMazeParams(const int width);

};

