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
	bool stopPlaying();

	bool getPlaystate();
	void setLooping(bool state);

	bool playRecording();
	void loopRecording();

	bool stepForward();
	void playLastFrame();

private:
	vector<Cell*> initialState;

	vector<Cell*> LastState;

	vector<vector<Cell*>> history;

	int current_step = 0;
	bool recording = false;
	bool isplaying = false;
	bool islooping = true;




};

