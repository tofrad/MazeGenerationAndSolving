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


	private:

		Maze M;

		Pathsolver S;

		ProgramState state = STOPPED;

};

