#include "Recorder.hpp"

Recorder::Recorder()
{

}

Recorder::Recorder(const int& maze_height, const int& maze_width,const RecordType r_type)
{
	this->height = maze_height;
	this->width = maze_width;
	this->setRecordType(r_type);

	Texture_height = maze_height * cellsize;
	Texture_width = maze_width * cellsize;

	frame_texture = LoadRenderTexture(Texture_width, Texture_height);
	BeginTextureMode(frame_texture);
	ClearBackground(LIGHTGRAY);
	EndTextureMode();
}

Recorder::Recorder(const vector<Cell*>& cell_list, const int& maze_height, const int& maze_width, const RecordType r_type) : Recorder(maze_height, maze_width, r_type)
{
	for (const auto cell : cell_list)
	{
		InitialState.push_back(RecordCell(cell));
	}
}

Recorder::~Recorder()
{

}

RecordType Recorder::getRecordType() const
{
	return this->recording_type;
}

int Recorder::getStep() const
{
	return current_step;
}

int Recorder::getSize() const
{
	return length;
}

RenderTexture2D Recorder::getFrameTexture() const
{
	return frame_texture;
}

void Recorder::startRecording()
{
	history.clear();
	recording = true;

}

void Recorder::stopRecording()
{
	recording = false;
	length = history.size() - 1;
}

void Recorder::recordStep(const vector<Cell*>& modifiedCells)
{
	if (!recording)return;

	vector<RecordCell> temp;

	for (const auto cell : modifiedCells) {
		//record cells here
		temp.push_back(RecordCell(cell));
	}	

	history.push_back(temp);
}

void Recorder::saveInitialFrame(const vector<Cell*>& FirstCells)
{
	for ( const auto cell : FirstCells)
	{
		InitialState.push_back(RecordCell(cell));
	}
	playInitialGrid();
}

void Recorder::saveLastFrame(const vector<Cell*>& LastList)
{
	for ( const auto cell : LastList)
	{
		LastState.push_back(RecordCell(cell));
	}
}

bool Recorder::stepForward()
{
	if (current_step < history.size()-1)
	{
		for (auto record_cell : history[current_step]) {
			record_cell.drawCell(cellsize, record_cell.getCurrentColor(), Mode::FORWARD);
		}
		current_step++;
		return true;
	}
	else if(current_step == history.size()-1)
	{
		for (auto record_cell : history[current_step]) {
			record_cell.drawCell(cellsize, record_cell.getCurrentColor(), Mode::FORWARD);
		}
		return true;
	}

	return false;
}

void Recorder::playLastFrame()
{
	for (auto record_cell : LastState) {
		record_cell.drawCell(cellsize, record_cell.getCurrentColor(),Mode::FORWARD);
	}
	current_step = length;
}

void Recorder::playInitialGrid()
{
	BeginTextureMode(this->frame_texture);

	for (auto record_cell : InitialState) {
		record_cell.drawCell(cellsize, record_cell.getCurrentColor(),Mode::FORWARD);
	}
	EndTextureMode();
	current_step = 0;
}

bool Recorder::stepBackward()
{
	if (current_step == 0) {
		for (auto record_cell : history[current_step])
		{
			record_cell.drawCell(cellsize, record_cell.getCurrentColor(),Mode::BACKWARD);
		}
		return false;
	}
	for (auto record_cell : history[current_step]) {
		record_cell.drawCell(cellsize, record_cell.getCurrentColor(),Mode::BACKWARD);
	}
	current_step--;
	return true;
}

void Recorder::playStep(const int step)
{
	current_step = step;
}

void Recorder::setRecordSize(const int maze_height, const int maze_width)
{
	setHeight(maze_height);
	setWidth(maze_width);
	Texture_height = maze_height * cellsize;
	Texture_width = maze_width * cellsize;
	frame_texture = LoadRenderTexture(Texture_width, Texture_height);
}

void Recorder::setRecordType(const RecordType type)
{
	this->recording_type = type;
}

void Recorder::setHeight(const int maze_height)
{
	this->height = maze_height;
}

void Recorder::setWidth(const int maze_width)
{
	this->width = maze_width;
}
