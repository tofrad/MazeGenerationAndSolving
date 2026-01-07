#pragma once
#include <functional>

#include "raylib.h"

#include "State_Definitions.hpp"

using ProgramstateCallback = std::function<void(ProgramState)>;

using GenerateCallback = std::function<void(int, GenerationMethod)>;

using SolveCallback = std::function<void(SolvingMethod)>;

using WindowChangeCallback = std::function<void(Screensize)>;


using GetMazeGeneratorCallback = std::function<GenerationMethod()>;
using GetMazeSizeCallback = std::function<int()>;

using GetSolverCallback = std::function<SolvingMethod()>;

using GetWindowSizeCallback = std::function<Screensize()>;


struct ProgramCallbacks {

	ProgramstateCallback onStateRequest = nullptr;

	GenerateCallback onGenerateRequest = nullptr;

	SolveCallback onSolveRequest = nullptr;

	WindowChangeCallback onWindowRequest = nullptr;

	GetMazeGeneratorCallback getGenerator = nullptr;
	GetMazeSizeCallback getMazeSize = nullptr;

	GetSolverCallback getSolver = nullptr;

	GetWindowSizeCallback getWindowSize = nullptr;

};