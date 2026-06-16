#pragma once

#include "Cell.hpp"
#include "RecordCell.hpp"
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
	void saveInitialFrame(const vector<Cell*>& FirstCells);
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

	void setHeight(int maze_height);
	void setWidth(int maze_width);

private:

	int size = 0;

	int height = 0;
	int width = 0;

	int cellsize = 10;
	int Texture_width = 960;
	int Texture_height = 540;
	RenderTexture2D last_frame = LoadRenderTexture(Texture_width, Texture_height);

	vector<RecordCell> InitialState;

	vector<RecordCell> LastState;

	vector<vector<RecordCell>> history;

	int current_step = 0;
	bool recording = false;
	bool isplaying = false;
	bool islooping = true;

};

