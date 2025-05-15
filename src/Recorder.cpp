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
}

void Recorder::recordStep(vector<Cell*> modifiedCells)
{
	if (!recording)return;

	vector<Cell*> temp;

	for (auto cell : modifiedCells) {

		temp.push_back(new Cell(*cell));
	}	

	history.push_back(temp);
}

void Recorder::saveLastFrame(vector<Cell*> LastList)
{
	for (auto cell : LastList)
	{
		LastState.push_back(new Cell(*cell));
	}

}

//########################################################################################################################################

void Recorder::startPlaying()
{
	if (!isplaying && islooping) {

		current_step = 0;
		isplaying = true;

		for (auto cell : initialState) {
			cell->drawCell();
		}
	}
}

bool Recorder::stopPlaying()
{
	return isplaying = false;
}

bool Recorder::getPlaystate()
{
	return isplaying;
}

void Recorder::setLooping(bool state)
{
	islooping = state;
}

bool Recorder::playRecording()
{
	startPlaying();
	setLooping(false);
	return stepForward();
}

void Recorder::loopRecording()
{
	bool stillPlaying = stepForward();

	setLooping(true);

	if (not stillPlaying) {
		startPlaying();
	}
}

bool Recorder::stepForward()
{
	if (current_step >= history.size()) {
		return stopPlaying();
	}
	if (isplaying)
	{
		for (auto cell : history[current_step]) {
			cell->drawCell();
		}
		current_step++;
		return true;
	}

}

void Recorder::playLastFrame()
{
	for (auto cell : LastState) {
		cell->drawCell();
	}
}
