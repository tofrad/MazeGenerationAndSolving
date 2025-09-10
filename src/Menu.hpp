#pragma once

#include "raylib.h"

#include "State_Definitions.hpp"

class Program;

typedef enum {
	OPEN,
	CLOSED,
	HOLD

} MenuState;

class Menu
{

	public:

		Menu();
		~Menu();

		void init(Program& P);

		void open();
		void close();

		void displayGUI();
		 

	private:

		Program* program = nullptr;

		void syncToProgram();

		void requestStateChange(ProgramState newState);

		void generatorRequest();

		void solverRequest();

		void WindowRequest();

		MenuState state = CLOSED;

		//GUI Variables-------------------------------------------------------

		/*TODO Dynamic GUI Size with Start and GetScreen Coords*/
		Vector2 GUI_Start;
		
		RenderTexture2D buffer;

		float dropdown_height = 40;
		float dropdown_length = 250;
		float dropdown_thick = 30;

		int Maze_GUI = 0;
		int Path_GUI = 0;
		int Window_GUI = 0;

		bool MazeEdit = false;
		bool PathEdit = false;
		bool WindowEdit = false;

		bool Generate_Button = false;
		bool Solve_Button = false;
		bool Exit_Button = false;

		//Values from GUI Input-----------------------------------------------

		//Maze-------------------------------------
		int MazeSize = 50;
		GenerationMethod MazeMethod;

		//Solver-----------------------------------
		SolvingMethod PathMethod;		

};

