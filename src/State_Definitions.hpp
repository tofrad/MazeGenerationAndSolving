#pragma once

#include "Point.hpp"

#include <vector>

//Programhandler
typedef enum {
	STOPPED,
	MENU,
	EDITING,
	PLAY_MAZE,
	PLAY_PATH

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

typedef enum {
	UHD = 0,	//2560x1440
	FHD = 1,	//1920x1080
	WSXGA = 2,	//1600x900
	SMALL = 3	//960x540

} Screensize;

struct TileMap{

	int size = 0;
	int height = 0;
	Point Start{ -1,-1 };
	Point Target{ -1,-1 };
	std::vector<std::vector<char>> TileArray;

};