#pragma once
#include <functional>
#include "State_Definitions.hpp"

class Recorder;

using ProgramstateCallback = std::function<void(ProgramState)>;
using GenerateCallback = std::function<void(int, GenerationMethod)>;
using SolveCallback = std::function<void(SolvingMethod)>;
using CustomGenerateCallback = std::function<void(TileMap&, GenerationMethod)>;

using GetMazeGeneratorCallback = std::function<GenerationMethod()>;
using GetMazeSizeCallback = std::function<int()>;
using GetSolverCallback = std::function<SolvingMethod()>;

using GetGeneratorRecording = std::function<Recorder*()>;
using GetSolveRecording = std::function<Recorder*()>;

struct ProgramCallbacks {

	ProgramstateCallback onStateRequest = nullptr;

	GenerateCallback onGenerateRequest = nullptr;

	SolveCallback onSolveRequest = nullptr;

	CustomGenerateCallback onCustomGenerateRequest = nullptr;

	GetMazeGeneratorCallback getGenerator = nullptr;
	GetMazeSizeCallback getMazeSize = nullptr;
	GetSolverCallback getSolver = nullptr;

	GetGeneratorRecording getGeneratorRecording = nullptr;
	GetSolveRecording getSolveRecording = nullptr;
};