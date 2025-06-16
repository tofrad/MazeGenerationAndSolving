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

		void requestStateChange(ProgramState newState);

		void generatorRequest();

		void solverRequest();

		MenuState state = CLOSED;

		//GUI Variables-------------------------------------------------------

		/*TODO Dynamic GUI Size with Start and GetScreen Coords*/
		Vector2 GUI_Start;
		

		float dropdown_height = 40;
		float dropdown_length = 250;
		float dropdown_thick = 30;

		int Maze_GUI = 0;
		int Path_GUI = 0;

		bool MazeEdit = false;
		bool PathEdit = false;

		//Values from GUI Input-----------------------------------------------

		//Maze-------------------------------------
		int MazeSize;
		GenerationMethod MazeMethod;

		//Solver-----------------------------------
		SolvingMethod PathMethod;		

};

