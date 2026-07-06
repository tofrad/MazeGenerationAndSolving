#pragma once

#include "Point.hpp"
#include "raylib.h"
#include <vector>

//Program-handler
typedef enum {
	STOPPED,
	MENU,
	EDITING,
	PLAYER

} ProgramState;

//Mazegenerator
typedef enum {
	REC_BACKTRACKING = 0,
	KRUSKAL = 1,
	HUNTANDKILL = 2,
	CUSTOM = 3

} GenerationMethod;

//Pathsolver
typedef enum {

	SM_BFS = 0,
	SM_DFS = 1

} SolvingMethod;

enum Direction
{
	FORWARD,
	BACKWARD
};

struct TileMap{

	int size = 0;
	int height = 0;
	Point Start{ -1,-1 };
	Point Target{ -1,-1 };

	std::vector<std::vector<char>> TileArray;

	bool isValid = false;
	bool isSolvable = false;

};
