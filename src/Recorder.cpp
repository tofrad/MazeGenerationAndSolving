#include "Recorder.hpp"

Recorder::Recorder()
{
	
}

Recorder::Recorder(const vector<Cell*> cell_list)
{
	for (auto cell : cell_list)
	{
		initialState.push_back(new Cell(*cell));
	}
}

Recorder::~Recorder()
{

}

void Recorder::startRecording()
{
	history.clear();
	current_step = 0;
	recording = true;

}

void Recorder::stopRecording()
{
	recording = false;

}

void Recorder::recordStep(Cell* modifiedCell)
{
	if (!recording)return;

	vector<Cell*> temp;
	temp.push_back(new Cell(*modifiedCell));

	history.push_back(temp);
	current_step++;
}

void Recorder::saveLastFrame(vector<Cell*> LastList)
{
	for (auto cell : LastList)
	{
		LastState.push_back(new Cell(*cell));
	}

}

void Recorder::startPlaying()
{
	if (!playing) {

		current_step = 0;
		playing = true;

		for (auto cell : initialState) {
			cell->drawCell();
		}
	}
}

void Recorder::stepForward()
{
	if (playing)
	{
		for (auto cell : history[current_step]) {
			cell->drawCell();
		}
		current_step++;
	}
	if (current_step >= history.size()) {
		playing = false;
		return;
	}
}

void Recorder::stopPlaying()
{

}

void Recorder::playLastFrame()
{
	for (auto cell : LastState) {
		cell->drawCell();
	}
}
