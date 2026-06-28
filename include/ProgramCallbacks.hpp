#pragma once
#include <functional>
#include "State_Definitions.hpp"

using ProgramstateCallback = std::function<void(ProgramState)>;
using GenerateCallback = std::function<void(int, GenerationMethod)>;
using SolveCallback = std::function<void(SolvingMethod)>;

using GetMazeGeneratorCallback = std::function<GenerationMethod()>;
using GetMazeSizeCallback = std::function<int()>;
using GetSolverCallback = std::function<SolvingMethod()>;



struct ProgramCallbacks {

	ProgramstateCallback onStateRequest = nullptr;

	GenerateCallback onGenerateRequest = nullptr;

	SolveCallback onSolveRequest = nullptr;

	GetMazeGeneratorCallback getGenerator = nullptr;
	GetMazeSizeCallback getMazeSize = nullptr;

	GetSolverCallback getSolver = nullptr;
};