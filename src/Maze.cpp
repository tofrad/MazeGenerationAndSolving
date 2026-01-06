#include "Maze.hpp"
#include "raymath.h"

#include <algorithm>
#include <random>
#include <ctime>

#define MAX_HEIGHT 150
#define MAX_WIDTH 150

#define MIN_HEIGHT 3
#define MIN_WIDTH 3

#define OFFSET 30

int usable_height;
int usable_width;

int start_x;
int start_y;

Maze::Maze()
{
	this->height = MIN_HEIGHT;
	this->width = MIN_WIDTH;
	rand_gen.seed(time(0));
	createConnectedMaze();
}

Maze::Maze(int width, int screenwidth, int screenheight, GenerationMethod method)
{

	if (width > MAX_WIDTH) {
		this->width = MAX_WIDTH;
	}
	else if (width < MIN_WIDTH) {
		this->width = MIN_WIDTH;
	}
	else {
		this->width = width;
	}

	int height = (this->width * 9) / 16;

	if (height > MAX_HEIGHT) {
		this->height = MAX_HEIGHT;
	}
	else if (height < MIN_HEIGHT) {
		this->height = MIN_HEIGHT;
	}
	else {
		this->height = height;
	}

	usable_height = screenheight - (OFFSET);
	usable_width = screenwidth - (OFFSET);

	this->cellsize = min(usable_width / this->width, usable_height / this->height);

	this->height = (int)(usable_height / this->cellsize);
	this->width = (int)(usable_width / this->cellsize);

	rand_gen.seed(time(0));
	generateMaze(method);
	
}

Maze::~Maze()
{

}

Maze::Maze(int screenwidth, int screenheight, TileMap* custom_maze)
{
	this->width = custom_maze->size;
	this->height = custom_maze->height;

	usable_height = screenheight - (OFFSET);
	usable_width = screenwidth - (OFFSET);

	this->cellsize = min(usable_width / this->width, usable_height / this->height);
	
	createConnectedMaze();

	for (int x = 0; x < this->width; x++) {

		for (int y = 0; y < this->height; y++) {

			switch (custom_maze->TileArray[x][y]) {

				case 0:
					//cell clear
					break;

				case 1:
					//cell wall
					Cell_Grid[x][y]->makeWall();
					break;

				case 2:
					//cell start
					Cell_Grid[x][y]->isStart = true;
					Start = Cell_Grid[x][y];
					break;

				case 3:
					//cell target

					Cell_Grid[x][y]->isTarget = true;
					Target = Cell_Grid[x][y];
					break;

				default:
					//cell clear
					break;
			}


		}

	}
	int x = 0;
	//Maze(custom_maze.size, custom_maze.)
}

Cell* Maze::getStart()
{
	return Start;
}

vector<Cell*> Maze::getGeneratedMaze()
{
	return Cell_List;
}

int Maze::getCellsize()
{
	return this->cellsize;
}

int Maze::getHeight()
{
	return this->height;
}

int Maze::getWidth()
{
	return this->width;
}

void Maze::createConnectedMaze()
{
	createEmptyMaze();

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

	Cell_Grid = vector<vector<Cell*>>(width, vector<Cell*>(height, nullptr));

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			start_x = OFFSET / 2 + (usable_width - (width * cellsize)) / 2;
			start_y = OFFSET / 2 + (usable_height - (height * cellsize)) / 2;
			Vector2 Offset{ start_x, start_y };
			Cell* C = new Cell(Point(x, y), cellsize, Offset);

			Cell_List.push_back(C);
			Cell_Grid[x][y] = C;
		}
	}


}

void Maze::generateMaze(GenerationMethod method)
{
	createEmptyMaze();

	int rand = 0; // GetRandomValue(0, Cell_List.size() - 1); 
	int rand2 = (height * width) - 1;// GetRandomValue(0, Cell_List.size() - 1); //(height*width) - 1; 

	Cell_List[rand]->isStart = true; 
	Start = Cell_List[rand]; 

	Cell_List[rand2]->isTarget = true; 
	Target = Cell_List[rand2];


	record = Recorder(Cell_List);

	record.startRecording();

	switch (method) {

		case REC_BACKTRACKING:
			RecursiveBacktracking(*Start);
			break;

		case KRUSKAL:
			Kruskal();
			break;

		case HUNTANDKILL:
			HuntAndKill();
			break;

		case CUSTOM:
			//TODO
			// error logging
			//shouldnt land here 

			//Cell_List[rand]->isStart = false;
			//Start = nullptr;

			//Cell_List[rand2]->isTarget = false;
			//Target = nullptr;
			break;

		default:
			HuntAndKill();
			break;

	}

	record.stopRecording();

	//clear was_visited mark for coloring in pathfinding 
	for (auto cell : Cell_List) {
		cell->wasVisited = false;
		cell->isActive = false;
	}

	record.saveLastFrame(Cell_List);
}

void Maze::resetMaze()
{
	for (auto cell : this->Cell_List) {
		cell->resetCell();
	}

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
	vector<Cell*> directions;
	Point pos = cell.getPosition();

	cell.isActive = true;

	if (!cell.wasVisited) {
		directions = getUnvisitedNeighbors(&cell);
		
	}
	// //mark as visited
	cell.wasVisited = true;

	shuffle(directions.begin(), directions.end(), rand_gen);
	
	for (auto target : directions) {

		if (!target->wasVisited) {

			connectCells(&cell, target);

			for (int i = 0; i < 5; i++) {
				record.recordStep(&cell);
			}

			cell.isActive = false;
			record.recordStep(&cell);
			RecursiveBacktracking(*target);
	}
	}
	cell.isActive = false;
	record.recordStep(&cell);
	
}

void Maze::Kruskal()
{
	//take all Connections by interating through cell grid after fully connected generation
	// set Parent Cell to itslef in every Cell
	// add only existing connections to South and East to prevent duplicates
	//------>>> vector<pair<Cell*,Cell*>> Walls -> currentCell, Cell.East
	//											-> currentCell, Cell.South
	vector<pair<Cell*, Cell*>> Walls;

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			Cell* currentCell = Cell_Grid[x][y];
			currentCell->setParent(currentCell);

			//add East Wall if not border
			if (x < width - 1) {
				Cell* EastCell = Cell_Grid[x+1][y];
				EastCell->setParent(EastCell);
				Walls.push_back(make_pair(currentCell, EastCell));
			}
			//add South Wall if not border
			if (y < height - 1) {
				Cell* WestCell = Cell_Grid[x][y+1];
				WestCell->setParent(WestCell);
				Walls.push_back(make_pair(currentCell, WestCell));
			}
		}
	}
	// shuffle the Wall List
	shuffle(Walls.begin(), Walls.end(), rand_gen);
	
	// do a union find by checking Parents
	// if same parents -> do nothing 
	// if different parents -> overwrite Parent from one Cell with other Parent to join the Sets
	// check if its South Connecting or East Connecting, by using th Cell Grid or Coordinates 
	// if Cell.x == Cell2.x --> X cord. same --> Cells above each other 
	//												-> Cell1.South = Cell2;
	//												-> Cell2.North = Cell1;
	// if Cell.y == Cell2.y --> Y Cord. same --> Cells next to each other
	//												-> Cell1.East = cell2;
	//												-> Cell2.west = cell1
	for (auto wall : Walls) {
		Cell* firstCell = wall.first;
		Cell* secondCell = wall.second;

		if (firstCell->getParent() != secondCell->getParent()) {

			secondCell->setParent(firstCell->getParent());

			uniteGroupByParents(secondCell);

			firstCell->isActive = true;
			secondCell->isActive = true;

			for (int i = 0; i < 7; i++) {
				record.recordStep(vector<Cell*>{firstCell, secondCell});
			}
			if (firstCell->getPosition().getX() == secondCell->getPosition().getX()) {
				firstCell->setSouth(secondCell);
				secondCell->setNorth(firstCell);
			}
			if (firstCell->getPosition().getY() == secondCell->getPosition().getY()) {
				firstCell->setEast(secondCell);
				secondCell->setWest(firstCell);
			}
			firstCell->isActive = false;
			secondCell->isActive = false;
			firstCell->wasVisited = true;
			secondCell->wasVisited = true;
			record.recordStep(vector<Cell*>{firstCell, secondCell});
		}
		else {
			// do nothing, cells already in same union
		}
	}

	// Delete all Parent Cells to Null
	for (auto cell : Cell_List) {
		cell->setParent(nullptr);
	}
	
	// For  Recording highlight Cells as Active and record 273 times
	// Delete active flag and record once
	// 
	// Delete Pair from Walls and repeat with for loop through walls
	// 
}

void Maze::HuntAndKill()
{
	Cell* current_cell = Start;
	current_cell->wasVisited = true;

	while(true){

		vector<Cell*> neighbor_list = getUnvisitedNeighbors(current_cell);
		shuffle(neighbor_list.begin(), neighbor_list.end(), rand_gen);

		Cell* next_cell = nullptr;

		//counter for iterating cell list in search for new cell
		int cell_cnt = 0;

		//get next cell
		if (!neighbor_list.empty()) {
			
			next_cell = neighbor_list[0];

			current_cell->isActive = true;
			for (int i = 0; i < 5; i++) {
				record.recordStep(current_cell);
			}

			connectCells(current_cell, next_cell);

			current_cell->isActive = false;
			current_cell->wasVisited = true;
			record.recordStep(current_cell);

			current_cell = next_cell;
		}
		else {
			for (auto cell : Cell_List) {

				if(cell->wasVisited == false){

					vector<Cell*> VisitedNeighbors = getVisitedNeighbors(cell);

					if (!VisitedNeighbors.empty()) {

						shuffle(VisitedNeighbors.begin(), VisitedNeighbors.end(), rand_gen);

						next_cell = VisitedNeighbors[0];

						cell->isActive = true;
						cell->wasVisited = true;
						for (int i = 0; i < 5; i++) {
							record.recordStep(cell);
						}

						connectCells(cell, next_cell);
						current_cell = next_cell;

						cell->isActive = false;
						record.recordStep(cell);

						break;
					}
				}
			}
		}
		//no valid cell is left , abort
		if (next_cell == nullptr) {
			break;
		}
	}

}

void Maze::uniteGroupByParents(Cell* start)
{
	vector<Cell*> neighbors;
	if (start->getSouth() != nullptr) {
		neighbors.push_back(start->getSouth());
	}
	if (start->getNorth() != nullptr) {
		neighbors.push_back(start->getNorth());
	}
	if (start->getWest() != nullptr) {
		neighbors.push_back(start->getWest());
	}
	if (start->getEast() != nullptr) {
		neighbors.push_back(start->getEast());
	}
	for (auto neighbor : neighbors) {

		if (neighbor->getParent() != start->getParent()) {

			neighbor->setParent(start->getParent());
			uniteGroupByParents(neighbor);
		}
	}
}

vector<Cell*> Maze::getUnvisitedNeighbors(Cell* cell) {

	vector<Cell*> directions;
	

	Point pos = cell->getPosition();
	int X = pos.getX();
	int Y = pos.getY();

	Cell* target_cell = nullptr;
	
	//north possible
	if (pos.getY() > 0) {
		target_cell = Cell_Grid[X][Y - 1];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	//East possible
	if (pos.getX() < this->width - 1) {
		target_cell = Cell_Grid[X + 1][Y];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 1) {
		target_cell = Cell_Grid[X][Y + 1];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}	
	}

	//West possible
	if (pos.getX() > 0) {
		target_cell = Cell_Grid[X - 1][Y];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	return directions;
}

vector<Cell*> Maze::getVisitedNeighbors(Cell* cell) {

	vector<Cell*> directions;

	Point pos = cell->getPosition();
	int X = pos.getX();
	int Y = pos.getY();

	Cell* target_cell = nullptr;

	//north possible
	if (pos.getY() > 0) {
		target_cell = Cell_Grid[X][Y - 1];

		if (target_cell->wasVisited) {
			directions.push_back(target_cell);
		}
	}

	//East possible
	if (pos.getX() < this->width - 1) {
		target_cell = Cell_Grid[X + 1][Y];

		if (target_cell->wasVisited) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 1) {
		target_cell = Cell_Grid[X][Y + 1];

		if (target_cell->wasVisited) {
			directions.push_back(target_cell);
		}
	}

	//West possible
	if (pos.getX() > 0) {
		target_cell = Cell_Grid[X - 1][Y];

		if (target_cell->wasVisited) {
			directions.push_back(target_cell);
		}
	}


	return directions;
}

void Maze::connectCells(Cell* first, Cell* second)
{
	int diff_x = first->getPosition().getX() - second->getPosition().getX();
	int diff_y = first->getPosition().getY() - second->getPosition().getY();

	if (diff_x == 0) {
		//North
		if (diff_y == 1) {
			first->setNorth(second);
			second->setSouth(first);
		}
		//South
		if (diff_y == -1) {
			first->setSouth(second);
			second->setNorth(first);
		}
	}
	else {
		//West
		if (diff_x == 1) {
			first->setWest(second);
			second->setEast(first);
		}
		//East
		if (diff_x == -1) {
			first->setEast(second);
			second->setWest(first);
		}

	}
}

