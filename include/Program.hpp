#pragma once

#include "Maze.hpp"
#include "Pathsolver.hpp"
#include "ProgramCallbacks.hpp"

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

		void handleGeneratorRequest(int size, GenerationMethod method, int weight_cnt, int max_weight);
		void handleSolveRequest(SolvingMethod method);
		static void handleCustomGenerateRequest(TileMap& custom, GenerationMethod GenMethod);

	private:

		void setState(ProgramState next_state);

		ProgramCallbacks createCallbacks();

		GenerationMethod Generator = REC_BACKTRACKING;

		SolvingMethod Solver = SM_DFS;

		ProgramState State = STOPPED;

		ProgramState LastState = MENU;

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

		void CalculateMazeParams(const int width);
};

