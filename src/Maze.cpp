#include "Maze.hpp"
#include "raymath.h"

#include <algorithm>
#include <random>
#include <ctime>

#define MAX_HEIGHT 100
#define MAX_WIDTH 100

#define MIN_HEIGHT 3
#define MIN_WIDTH 3

#define OFFSET 10
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

Maze::Maze(int width, int height, int screenwidth, int screenheight)
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
	usable_height = screenheight - (OFFSET);
	usable_width = screenwidth - (OFFSET);

	this->cellsize = min(usable_width / this->width, usable_height / this->height);

	this->height = (int)(usable_height / this->cellsize);
	/*this->width = (int)(usable_width / this->cellsize);*/

	rand_gen.seed(time(0));
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

			start_x = OFFSET + (usable_width - (width * cellsize)) / 2;
			start_y = OFFSET + (usable_height - (height * cellsize)) / 2;
			Vector2 Offset{start_x, start_y};
			Cell* C = new Cell(Point(x, y), cellsize, Offset);

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

	int rand = 0; // GetRandomValue(0, Cell_List.size() - 1);
	int rand2 = (height * width) - 1;// GetRandomValue(0, Cell_List.size() - 1); //(height*width) - 1;

	Cell_List[rand]->isStart = true;
	Start = Cell_List[rand];

	Cell_List[rand2]->isTarget = true;
	Target = Cell_List[rand2];

	record = Recorder(Cell_List);

	record.startRecording();

	//Kruskal();
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
	vector<Cell*> directions;
	Point pos = cell.getPosition();

	cell.isActive = true;

	if (!cell.wasVisited) {
		auto result = getUnvisitedNeighbors(&cell);
		directions = result.second;
	}
	// //mark as visited
	cell.wasVisited = true;

	vector<int> dir_idx{ 0,1,2,3 };

	shuffle(dir_idx.begin(), dir_idx.end(), rand_gen);
	
	for (auto idx : dir_idx) {

		//int idx = dir_idx[i];
		Cell* target = directions.at(idx);

		if (target != nullptr) {

			//target could have been visited by prev funtion call in between
			if (!target->wasVisited) {
				switch (idx) {
					case 0: //north
						cell.setNorth(target);
						target->setSouth(&cell);
						break;

					case 1: //east
						cell.setEast(target);
						target->setWest(&cell);
						break;

					case 2: //south
						cell.setSouth(target);
						target->setNorth(&cell);
						break;

					case 3: //west
						cell.setWest(target);
						target->setEast(&cell);
						break;

					default:
						break;
				}
				for (int i = 0; i < 5; i++) {
					record.recordStep(&cell);
				}
				cell.isActive = false;
				record.recordStep(&cell);		

				RecursiveBacktracking(*target);
			}
		}
		else {
			//target is null do nothing
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

	for (auto cell : Cell_List) {

		if (cell->wasVisited == false) {


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

pair<int, vector<Cell*>> Maze::getUnvisitedNeighbors(Cell* cell) {

	vector<Cell*> directions{4, nullptr};
	int cnt = 0;

	Point pos = cell->getPosition();
	int X = pos.getX();
	int Y = pos.getY();

	Cell* target_cell = nullptr;
	
	//north possible
	if (pos.getY() > 0) {
		target_cell = Cell_Grid[X][Y - 1];

		if (target_cell->wasVisited == false) {
			directions[0] = target_cell;
			cnt++;
		}
	}

	//East possible
	if (pos.getX() < this->width - 1) {
		target_cell = Cell_Grid[X + 1][Y];

		if (target_cell->wasVisited == false) {
			directions[1] = target_cell;
			cnt++;
		}
	}

	//south possible
	if (pos.getY() < this->height - 1) {
		target_cell = Cell_Grid[X][Y + 1];

		if (target_cell->wasVisited == false) {
			directions[2] = target_cell;
			cnt++;
		}	
	}

	//West possible
	if (pos.getX() > 0) {
		target_cell = Cell_Grid[X - 1][Y];

		if (target_cell->wasVisited == false) {
			directions[3] = target_cell;
			cnt++;
		}
	}


	return make_pair(cnt ,directions);
}

pair<int, vector<Cell*>> Maze::getVisitedNeighbors(Cell* cell) {

	vector<Cell*> directions{ 4, nullptr };
	int cnt = 0;

	Point pos = cell->getPosition();
	int X = pos.getX();
	int Y = pos.getY();

	Cell* target_cell = nullptr;

	//north possible
	if (pos.getY() > 0) {
		target_cell = Cell_Grid[X][Y - 1];

		if (target_cell->wasVisited) {
			directions[0] = target_cell;
			cnt++;
		}
	}

	//East possible
	if (pos.getX() < this->width - 1) {
		target_cell = Cell_Grid[X + 1][Y];

		if (target_cell->wasVisited) {
			directions[1] = target_cell;
			cnt++;
		}
	}

	//south possible
	if (pos.getY() < this->height - 1) {
		target_cell = Cell_Grid[X][Y + 1];

		if (target_cell->wasVisited) {
			directions[2] = target_cell;
			cnt++;
		}
	}

	//West possible
	if (pos.getX() > 0) {
		target_cell = Cell_Grid[X - 1][Y];

		if (target_cell->wasVisited) {
			directions[3] = target_cell;
			cnt++;
		}
	}


	return make_pair(cnt, directions);
}
