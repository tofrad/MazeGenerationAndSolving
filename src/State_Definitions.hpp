#pragma once

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
	UHD,	//2560x1440
	FHD,	//1920x1080
	WSXGA,	//1600x900
	SMALL	//960x540

} Screensize;