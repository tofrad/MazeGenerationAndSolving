#include "Recorder.hpp"

Recorder::Recorder()
{

}

Recorder::Recorder(const int& maze_height, const int& maze_width)
{
	this->height = maze_height;
	this->width = maze_width;

	cellsize = Texture_width / maze_width;

	BeginTextureMode(last_frame);
	ClearBackground(LIGHTGRAY);
	EndTextureMode();
}

Recorder::Recorder(const vector<Cell*>& cell_list, const int& maze_height, const int& maze_width) : Recorder(maze_height, maze_width)
{
	for (const auto cell : cell_list)
	{
		InitialState.push_back(RecordCell(*cell));
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

	vector<RecordCell> temp;
	modifiedCell->updateColor();
	temp.push_back(RecordCell(*modifiedCell));

	history.push_back(temp);
}

void Recorder::recordStep(const vector<Cell*>& modifiedCells)
{
	if (!recording)return;

	vector<RecordCell> temp;

	for (const auto cell : modifiedCells) {
		//record cells here
		cell->updateColor();
		temp.push_back(RecordCell(*cell));
	}	

	history.push_back(temp);
}

void Recorder::saveInitialFrame(const vector<Cell*>& FirstCells)
{
	for (const auto cell : FirstCells)
	{
		InitialState.push_back(RecordCell(*cell));
	}
}

void Recorder::saveLastFrame(const vector<Cell*>& LastList)
{
	for (const auto cell : LastList)
	{
		LastState.push_back(RecordCell(*cell));
	}

}

void Recorder::startPlaying()
{
	BeginTextureMode(last_frame);
	if (!isplaying && islooping) {

		current_step = 0;
		isplaying = true;

		for (auto cell : InitialState) {
			cell.drawCell(cellsize, cell.getColor());
		} 
	}
	EndTextureMode();
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
			cell.drawCell(cellsize, cell.getColor());
		}
		current_step++;
		return true;
	}

	return true;

}

void Recorder::playLastFrame() const
{
	for (auto cell : LastState) {
		cell.drawCell(cellsize, cell.getColor());
	}
}

void Recorder::playInitialGrid() const
{
	for (auto cell : InitialState) {
		cell.drawCell(cellsize, cell.getColor());
	}

}

void Recorder::playStep(const int step)
{
	current_step = step;

}

void Recorder::setHeight(const int maze_height)
{
	this->height = maze_height;
}

void Recorder::setWidth(const int maze_width)
{
	this->width = maze_width;
}

