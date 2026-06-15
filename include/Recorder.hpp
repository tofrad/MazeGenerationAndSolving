#pragma once

#include "Cell.hpp"

#include <vector>

using namespace std;

class Recorder
{

public:
	Recorder();
	Recorder(const int& maze_height, const int& maze_width);
	explicit Recorder(const vector<Cell*>& cell_list, const int& maze_height, const int& maze_width);
	~Recorder();

	int getStep() const;
	int getSize() const;

	void startRecording();
	void stopRecording();

	void recordStep(Cell* modifiedCell);
	void recordStep(const vector<Cell*>& modifiedCells);
	void saveLastFrame(const vector<Cell*>& LastList);

	void startPlaying();
	void stopPlaying();

	bool getPlaystate() const;
	void setLooping(bool state);

	bool playRecording();
	void loopRecording();

	bool stepForward();
	void playLastFrame() const;

	void playInitialGrid() const;

	void playStep(int step);


private:

	int size = 0;

	int height = 0;
	int width = 0;

	vector<Base_Cell> initialState;

	vector<Base_Cell> LastState;

	vector<vector<Base_Cell>> history;

	int current_step = 0;
	bool recording = false;
	bool isplaying = false;
	bool islooping = true;




};

