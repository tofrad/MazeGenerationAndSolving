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
	void recordStep(vector<Cell*> modifiedCells);
	void saveLastFrame(vector<Cell*> LastList);

	void startPlaying();
	bool stopPlaying();

	bool getPlaystate();
	void setLooping(bool state);

	bool playRecording();
	void loopRecording();

	bool stepForward();
	void playLastFrame();

	void playStep(int step);

private:

	int size = 0;

	vector<Base_Cell> initialState;

	vector<Base_Cell> LastState;

	vector<vector<Base_Cell>> history;

	int current_step = 0;
	bool recording = false;
	bool isplaying = false;
	bool islooping = true;




};

