#include "Recorder.hpp"

Recorder::Recorder()
{

}

Recorder::Recorder(const vector<Cell*>& cell_list)
{
	for (const auto cell : cell_list)
	{
		initialState.push_back(cell->get_Base_copy()); 
	}
}

Recorder::~Recorder()
{

}

int Recorder::getStep() const
{
	return current_step;
}

int Recorder::getSize() const
{
	return size;
}

void Recorder::startRecording()
{
	history.clear();
	recording = true;

}

void Recorder::stopRecording()
{
	recording = false;
	size = history.size() - 1;
}

void Recorder::recordStep(Cell* modifiedCell)
{
	if (!recording)return;

	vector<Base_Cell> temp;
	modifiedCell->updateColor();
	temp.push_back(modifiedCell->get_Base_copy());

	history.push_back(temp);
}

void Recorder::recordStep(const vector<Cell*>& modifiedCells)
{
	if (!recording)return;

	vector<Base_Cell> temp;

	for (const auto cell : modifiedCells) {

		cell->updateColor();
		temp.push_back(cell->get_Base_copy()); 
	}	

	history.push_back(temp);
}

void Recorder::saveLastFrame(const vector<Cell*>& LastList)
{
	for (const auto cell : LastList)
	{
		LastState.push_back(cell->get_Base_copy());
	}

}

void Recorder::startPlaying()
{
	if (!isplaying && islooping) {

		current_step = 0;
		isplaying = true;

		for (auto cell : initialState) {
			cell.drawCell(); 
		} 
	}
}

void Recorder::stopPlaying()
{
	isplaying = false;
}

bool Recorder::getPlaystate() const
{
	return isplaying;
}

void Recorder::setLooping(const bool state)
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
	const bool stillPlaying = stepForward();

	setLooping(true);

	if (not stillPlaying) {
		startPlaying();
	}
}

bool Recorder::stepForward()
{

	if (current_step >= history.size()) {
		stopPlaying();
		return false;
	}
	if (isplaying)
	{
		for (auto cell : history[current_step]) {
			cell.drawCell(); 
		}
		current_step++;
		return true;
	}

	return true;

}

void Recorder::playLastFrame() const
{
	for (auto cell : LastState) {
		cell.drawCell(); 
	}
}

void Recorder::playInitialGrid() const
{
	for (auto cell : initialState) {
		cell.drawEmptyCell();
	}

}

void Recorder::playStep(const int step)
{
	current_step = step;

}
