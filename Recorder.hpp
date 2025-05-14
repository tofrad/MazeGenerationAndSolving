#pragma once

#include "Cell.hpp"

#include <vector>

using namespace std;

class Recorder
{

public:
	Recorder();
	Recorder(const vector<Cell*> cell_list);
	~Recorder();

	void startRecording();
	void stopRecording();

	void recordStep(Cell* modifiedCell);
	void saveLastFrame(vector<Cell*> LastList);

	void startPlaying();
	void stepForward();

	void stopPlaying();
	void playLastFrame();

private:
	vector<Cell*> initialState;

	vector<Cell*> LastState;

	vector<vector<Cell*>> history;


	int current_step = 0;
	bool recording = false;
	bool playing = false;

};

