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
	REC_BACKTRACKING,
	KRUSKAL,
	HUNTANDKILL,
	CUSTOM

} GenerationMethod;

//Pathsolver
typedef enum {
	SM_DFS,
	SM_BFS

} SolvingMethod;