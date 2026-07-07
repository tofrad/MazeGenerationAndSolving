#pragma once

#include "Cell.hpp"

#include <vector>
#include <memory>

#include"raylib.h"

using namespace std;

enum RecordType
{
	MAZE,
	PATH,
	NONE
};

class Base_RecordCell;

class Recorder
{

public:
	Recorder();
	Recorder(const int& maze_height, const int& maze_width, RecordType r_type);
	explicit Recorder(const vector<Cell*>& cell_list, const int& maze_height, const int& maze_width, RecordType r_type);

	Recorder(const Recorder&) = delete;
	Recorder& operator=(const Recorder&) = delete;

	~Recorder();

	void init(const int& maze_height, const int& maze_width, RecordType r_type);

	RecordType getRecordType() const;
	int* getStep();
	int getVisitedCnt() const;
	int getSize() const;
	RenderTexture2D getFrameTexture() const;

	void startRecording();
	void stopRecording();

	void recordStep(const vector<Cell*>& modifiedCells);
	void saveInitialFrame(const vector<Cell*>& FirstCells);
	void saveLastFrame(const vector<Cell*>& LastList);

	bool stepForward();

	void playLastFrame();
	void playInitialGrid();

	bool stepBackward();

	void playStep(int step);
	void draw_till_step(int step);
	//reloads texture
	void setRecordSize(int maze_height, int maze_width);

private:

	RecordType recording_type = NONE;
	void setRecordType(RecordType type);

	int length = 0;

	int height = 0;
	int width = 0;

	int cellsize = 32;
	int Texture_width = 960;
	int Texture_height = 540;
	RenderTexture2D frame_texture{};

	vector<std::unique_ptr<Base_RecordCell>>  InitialState;

	vector<std::unique_ptr<Base_RecordCell>> LastState;

	vector<vector<std::unique_ptr<Base_RecordCell>>> history;

	int current_step = 0;
	int cells_visited = 0;

	bool recording = false;
	bool isplaying = false;


	void setHeight(int maze_height);
	void setWidth(int maze_width);
};

