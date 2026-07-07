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

	InitialState.clear();
	InitialState.shrink_to_fit();
	LastState.clear();
	LastState.shrink_to_fit();
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
	history.clear();
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
			if (not(cell->getMazeFlags_Current()->isStart ||
					cell->getMazeFlags_Current()->isWall ||
					cell->getMazeFlags_Current()->isTarget))
			{
				temp.push_back(std::make_unique<PathRecordCell>(cell));
			}
		}
	}

	history.push_back(std::move(temp));
}

void Recorder::saveInitialFrame(const vector<Cell*>& FirstCells)
{
	if (recording_type == MAZE)
	{
		for (auto cell : FirstCells) {
			//record cells here
			InitialState.push_back(std::make_unique<MazeRecordCell>(cell));
		}
	}else
	{
		for (auto cell : FirstCells) {
			//record cells here
			if (cell->getMazeFlags_Current()->isStart ||
				cell->getMazeFlags_Current()->isWall ||
				cell->getMazeFlags_Current()->isTarget)
			{
				InitialState.push_back(std::make_unique<MazeRecordCell>(cell));

			}else
			{
				InitialState.push_back(std::make_unique<PathRecordCell>(cell));
			}
		}
	}
	playInitialGrid();
}

void Recorder::saveLastFrame(const vector<Cell*>& LastList)
{
	if (recording_type == MAZE)
	{
		for (auto cell : LastList) {
			//record cells here
			LastState.push_back(std::make_unique<MazeRecordCell>(cell));
		}
	}else
	{
		for (auto cell : LastList) {
			//record cells here
			if (cell->getMazeFlags_Current()->isStart ||
				cell->getMazeFlags_Current()->isWall ||
				cell->getMazeFlags_Current()->isTarget)
			{
				LastState.push_back(std::make_unique<MazeRecordCell>(cell));
			}else
			{
				LastState.push_back(std::make_unique<PathRecordCell>(cell));
			}
		}
	}
}

bool Recorder::stepForward()
{
	if (!history.empty())
	{
		BeginTextureMode(this->frame_texture);

		if (current_step <= length)
		{
			for (const auto& record_cell : history[current_step]) {
				record_cell->draw(cellsize, Direction::FORWARD);
			}

			if(current_step == length)
			{
				EndTextureMode();
				return false;
			}
			current_step++;

			EndTextureMode();
			return true;

		}
		EndTextureMode();
		return true;
	}
	return false;
}

void Recorder::playLastFrame()
{
	if (!LastState.empty())
	{
		BeginTextureMode(this->frame_texture);

		for (const auto& record_cell : LastState) {
			record_cell->draw(cellsize, Direction::FORWARD);
		}
		current_step = length;
		EndTextureMode();
	}
}

void Recorder::playInitialGrid()
{
	if (!InitialState.empty())
	{
		BeginTextureMode(this->frame_texture);

		for (const auto& record_cell : InitialState) {
			record_cell->draw(cellsize, Direction::FORWARD);
		}
		EndTextureMode();
		current_step = 0;
	}
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
			for (const auto& record_cell : history[current_step-1])
			{
				record_cell->draw(cellsize,Direction::BACKWARD);
			}

			if (current_step - 1 == 0)
			{
				current_step--;
				EndTextureMode();
				return false;
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
