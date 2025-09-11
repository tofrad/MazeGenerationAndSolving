#pragma once

#include "raylib.h"

#include "State_Definitions.hpp"

class Program;

class Editor
{

public:

	Editor();
	~Editor();

	void init(Program& P);

	void open();
	void close();

	void displayEditor();

private:

	Program* program = nullptr;

	void syncToProgram();

	RenderTexture2D buffer;

};