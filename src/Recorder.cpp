#include "Recorder.hpp"
#include <cmath>
#include <algorithm>

#include "MazeRecordCell.hpp"
#include "PathRecordCell.hpp"

#include <iostream>
#include <ostream>

#include <variant>

using RecordCellVariant = std::variant<MazeRecordCell, PathRecordCell>;

Recorder::Recorder()
{

}

Recorder::Recorder(const int& maze_height, const int& maze_width,const RecordType r_type)
{
	init(maze_height,maze_width,r_type);
	BeginTextureMode(frame_texture);
	ClearBackground(LIGHTGRAY);
	EndTextureMode();
}

Recorder::Recorder(const vector<Cell*>& cell_list, const int& maze_height, const int& maze_width, const RecordType r_type) : Recorder(maze_height, maze_width, r_type)
{
	saveInitialFrame(cell_list);
}

Recorder::~Recorder()
{

}

void Recorder::init(const int& maze_height, const int& maze_width, const RecordType r_type)
{
	recording_type = NONE;

	length = 0;

	this->height = maze_height;
	this->width = maze_width;
	this->setRecordType(r_type);

	cellsize = 32;
	Texture_height = maze_height * cellsize;
	Texture_width = maze_width * cellsize;

	if (IsRenderTextureValid(frame_texture))
	{
		UnloadRenderTexture(frame_texture);
	}
	frame_texture = LoadRenderTexture(Texture_width, Texture_height);

	history.clear();
	history.shrink_to_fit();

	current_step = 0;
	cells_visited = 0;

	recording_type = r_type;

	recording = false;
	isplaying = false;
}

RecordType Recorder::getRecordType() const
{
	return this->recording_type;
}

int* Recorder::getStep()
{
	return &current_step;
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
	recording = true;
}

void Recorder::stopRecording()
{
	recording = false;
	length = static_cast<int>(history.size()) - 1;
}

void Recorder::recordStep(const vector<Cell*>& modifiedCells)
{
	if (!recording)return;

	vector<std::unique_ptr<Base_RecordCell>> temp;

	if (recording_type == MAZE)
	{
		for (auto cell : modifiedCells) {
			//record cells here
			temp.push_back(std::make_unique<MazeRecordCell>(cell));
		}
	}else
	{
		for (auto cell : modifiedCells) {
			//record cells here
			if (not(cell->getMazeFlags_Current()->isWall))
			{
				temp.push_back(std::make_unique<PathRecordCell>(cell));
			}
		}
	}

	history.push_back(std::move(temp));
}

void Recorder::recordStep(const vector<Cell*>& modifiedCells_first, const vector<Cell*>& modifiedCells_second)
{
	vector<Cell*> temp;
	temp.reserve(modifiedCells_first.size() + modifiedCells_second.size());
	temp.insert(temp.end(), modifiedCells_first.begin(), modifiedCells_first.end());
	temp.insert(temp.end(), modifiedCells_second.begin(), modifiedCells_second.end());
	recordStep(temp);
}

void Recorder::saveInitialFrame(const vector<Cell*>& FirstCells)
{
	vector<unique_ptr<Base_RecordCell>> temp;

	if (recording_type == MAZE)
	{
		for (auto cell : FirstCells) {
			//record cells here
			temp.push_back(std::make_unique<MazeRecordCell>(cell));
		}
	}else
	{
		//no need for recording path cells as original view has no path action
		for (auto cell : FirstCells) {
			//record cells here
			temp.push_back(std::make_unique<MazeRecordCell>(cell));
		}
	}
	history.push_back(std::move(temp));
	playInitialGrid();
}

void Recorder::saveLastFrame(const vector<Cell*>& LastList)
{
	startRecording();
	vector<unique_ptr<Base_RecordCell>> temp;

	if (recording_type == MAZE)
	{
		for (auto cell : LastList) {
			//record cells here
			temp.push_back(std::make_unique<MazeRecordCell>(cell));
		}
	}else
	{
		for (auto cell : LastList) {
			// record cells here
			 if (cell->getMazeFlags_Current()->isWall)
			 {
			 	temp.push_back(std::make_unique<MazeRecordCell>(cell));
			 }else
			 {
				temp.push_back(std::make_unique<PathRecordCell>(cell));
			}
		}
	}
	history.push_back(std::move(temp));
	stopRecording();
}

bool Recorder::stepForward()
{
	if (!history.empty())
	{
		BeginTextureMode(this->frame_texture);

		current_step++;

		if (current_step < length)
		{
			for (const auto& record_cell : history[current_step]) {
				record_cell->draw(cellsize, Direction::FORWARD);
			}
			// current_step++;
			EndTextureMode();
			return true;
		}
		current_step = length;
		EndTextureMode();
		return false;
	}
	return false;
}

void Recorder::playLastFrame()
{

	BeginTextureMode(this->frame_texture);

	for (const auto& record_cell : history.back()) {
		record_cell->draw(cellsize, Direction::FORWARD);
	}
	current_step = length;
	EndTextureMode();

}

void Recorder::playInitialGrid()
{
	BeginTextureMode(this->frame_texture);
	ClearBackground(LIGHTGRAY);
	for (const auto& record_cell : history.front()) {
		record_cell->draw(cellsize, Direction::FORWARD);
	}
	EndTextureMode();
	current_step = 0;
}


bool Recorder::stepBackward()
{
	if (!history.empty())
	{
		BeginTextureMode(this->frame_texture);

		if (current_step >= 1) {
			//revert current step
			for (const auto& record_cell : history[current_step])
			{
				record_cell->draw(cellsize,Direction::BACKWARD);
			}
			
			current_step--;
			EndTextureMode();
			return true;
		}
		EndTextureMode();
		return false;
	}
	return false;
}

void Recorder::playStep(const int step)
{
	const int first_dist = step;
	const int last_dist = std::abs(length - step);
	const int current_dist = std::abs(current_step - step);

	const int shortest_dist = std::min({first_dist, last_dist, current_dist});

	if (shortest_dist == first_dist)
	{
		this->playInitialGrid();
		draw_till_step(step);

	}else if (shortest_dist == last_dist)
	{
		this->playLastFrame();
		draw_till_step(step);
	}else
	{
		draw_till_step(step);
	}
}

void Recorder::draw_till_step( const int step)
{
	while (current_step != step)
	{
		if (current_step < step)
		{
			this->stepForward();
		}else
		{
			this->stepBackward();
		}
	}
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
