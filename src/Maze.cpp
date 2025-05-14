#include "Maze.hpp"
#include "raymath.h"

#include <algorithm>
#include <random>

#define MAX_HEIGHT 150
#define MAX_WIDTH 150

#define MIN_HEIGHT 3
#define MIN_WIDTH 3

#define OFFSET 10

Maze::Maze()
{
	this->height = MIN_HEIGHT;
	this->width = MIN_WIDTH;
	createConnectedMaze();
}

Maze::Maze(int height, int width, int screenwidth, int screenheight)
{
	if (height > MAX_HEIGHT) {
		this->height = MAX_HEIGHT;
	}
	else if (height < MIN_HEIGHT) {
		this->height = MIN_HEIGHT;
	}
	else {
		this->height = height;
	}

	if (width > MAX_WIDTH) {
		this->width = MAX_WIDTH;
	}
	else if (width < MIN_WIDTH) {
		this->width = MIN_WIDTH;
	}
	else {
		this->width = width;
	}

	this->cellsize = min((screenwidth - OFFSET) / this->width, (screenheight - OFFSET) / this->height);

	createConnectedMaze();
}

Maze::~Maze()
{

}

Cell* Maze::getStart()
{
	return Start;
}

vector<Cell*> Maze::getGeneratedMaze()
{
	return GeneratedMaze;
}

void Maze::createConnectedMaze()
{
	Cell_List.clear();

	Cell_Grid = vector<vector<Cell*>>(width, vector<Cell*>(height, nullptr));

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			Cell* C = new Cell(Point(x, y), this->cellsize, OFFSET);

			Cell_List.push_back(C);
			Cell_Grid[x][y] = C;
		}
	}
	//connect with neighbors

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			Cell* currentCell = Cell_Grid[x][y];

			//set borders 
			currentCell->setNorth((y > 0) ? Cell_Grid[x][y -1] : nullptr);
			currentCell->setWest((x > 0) ? Cell_Grid[x-1][y] : nullptr);

			currentCell->setEast((x < width-1) ? Cell_Grid[x+1][y] : nullptr);
			currentCell->setSouth((y < height-1) ? Cell_Grid[x][y+1] : nullptr);

		}
	}
}

void Maze::createEmptyMaze()
{
	Cell_List.clear();

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			Cell* C = new Cell(Point(x, y));

			Cell_List.push_back(C);
		}
	}
}

void Maze::generateMaze()
{
	createConnectedMaze();
	deleteConnections();

	int rand = GetRandomValue(0, Cell_List.size() - 1);
	int rand2 = GetRandomValue(0, Cell_List.size() - 1); //(height*width) - 1;

	Cell_List[rand]->isStart = true;
	Start = Cell_List[rand];

	Cell_List[rand2]->isTarget = true;
	Target = Cell_List[rand2];

	record = Recorder(Cell_List);

	record.startRecording();

	RecursiveBacktracking(*Cell_List[rand]);

	record.stopRecording();

	//clear was_visited mark for coloring in pathfinding 
	for (auto cell : Cell_List) {
		cell->wasVisited = false;
		cell->isActive = false;
	}

	record.saveLastFrame(Cell_List);
	saveLastState();
}

void Maze::drawMaze()
{
	record.playLastFrame();
}

bool Maze::playRecording()
{
	return record.playRecording();
}

void Maze::loopRecording()
{
	record.loopRecording();
}

void Maze::stopRecording()
{
	record.stopPlaying();
}

void Maze::saveLastState()
{
	for (auto cell : Cell_List)
	{
		GeneratedMaze.push_back(new Cell(*cell));
	}
}

void Maze::drawCells()
{
	for (const auto& cell : Cell_List) {
		
		cell->drawCell();
	}
}

void Maze::deleteConnections()
{
	for (auto Cell : Cell_List) {
		Cell->setSouth(nullptr);
		Cell->setNorth(nullptr);
		Cell->setWest(nullptr);
		Cell->setEast(nullptr);
	}
}

void Maze::RecursiveBacktracking(Cell& cell)
{
	vector<int> directions;
	Point pos = cell.getPosition();

	cell.isActive = true;

	if (!cell.wasVisited) {

		//check for boundaries
		//create possible directions

		if (pos.getY() < this->height - 1) {
			//south possible -> 0
			directions.push_back(0);
		}
		if (pos.getY() > 0) {
			//north possible -> 1
			directions.push_back(1);
		}
		if (pos.getX() > 0) {
			//West possible -> 2
			directions.push_back(2);
		}
		if (pos.getX() < this->width - 1) {
			//East possible -> 3
			directions.push_back(3);
		}
	}

	// //mark as visited
	cell.wasVisited = true;

	while(!directions.empty()){

		//choose randomly and delete from list

		int r = GetRandomValue(0, directions.size() - 1);		
		int dir = directions.at(r);
		directions.erase(directions.begin()+ r);

		//set Pointer to choosen Cell
		//call function from new Cell again
		Cell* target_cell = nullptr;

		switch (dir) {
			case 0: //south
				target_cell = Cell_Grid[pos.getX()][pos.getY()+1];

				if (!target_cell->wasVisited) {
					cell.setSouth(target_cell);
					target_cell->setNorth(&cell);		
				}
				break;

			case 1: //north
				target_cell = Cell_Grid[pos.getX()][pos.getY()-1];

				if (!target_cell->wasVisited) {
					cell.setNorth(target_cell);
					target_cell->setSouth(&cell);					
				}
				break;

			case 2: //west
				target_cell = Cell_Grid[pos.getX()-1][pos.getY()];

				if (!target_cell->wasVisited) {
					cell.setWest(target_cell);
					target_cell->setEast(&cell);					
				}
				break;

			case 3: //east
				target_cell = Cell_Grid[pos.getX()+1][pos.getY()];

				if (!target_cell->wasVisited) {
					cell.setEast(target_cell);
					target_cell->setWest(&cell);					
				}
				break;

			default:				
				break;
				
		}

		

		if (target_cell) {
			record.recordStep(&cell);
			record.recordStep(&cell);
			cell.isActive = false;
			record.recordStep(&cell);
			RecursiveBacktracking(*target_cell);
		}
	}
}
