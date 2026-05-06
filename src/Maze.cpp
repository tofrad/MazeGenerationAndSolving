#include "Maze.hpp"
#include "raymath.h"

#include <algorithm>
#include <random>
#include <ctime>

#define MAX_HEIGHT 249
#define MAX_WIDTH 249

#define MIN_HEIGHT 11
#define MIN_WIDTH 11

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

Maze::Maze(int w, int screenwidth, int screenheight, GenerationMethod method)
{

	if (w > MAX_WIDTH) {
		this->width = MAX_WIDTH;
	}
	else if (w < MIN_WIDTH) {
		this->width = MIN_WIDTH;
	}
	else {
		this->width = w;
	}
	if (this->width % 2 == 0) {
		this->width = this->width - 1;
	}

	int height = (this->width * 9) / 16;
	if (height % 2 == 0) {
		height = height - 1;
	}

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

	//this->height = (int)(usable_height / this->cellsize);
	//this->width = (int)(usable_width / this->cellsize);

	rand_gen.seed(time(0));
	generateMaze(method);
	
}

Maze::~Maze()
{

}

Maze::Maze(int screenwidth, int screenheight, const TileMap* custom_maze)
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

	record = Recorder(Cell_List);
}

void Maze::displayInitialFrame()
{
	record.playInitialGrid();
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

			if (x % 2 != 0 || y % 2 != 0) {
				C->makeWall();
			}
			C->setParent(C);
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

Recorder* Maze::getRecording()
{
	return &record;
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
			
			Cell* MiddleCell = connectCells(&cell, target);
			MiddleCell->wasVisited = true;
			MiddleCell->isActive = true;
			record.recordStep(MiddleCell);

			for (int i = 0; i < 5; i++) {
				record.recordStep(&cell);
			}

			cell.isActive = false;
			MiddleCell->isActive = false;
			record.recordStep(&cell);
			RecursiveBacktracking(*target);
	}
	}
	cell.isActive = false;
	record.recordStep(&cell);
	
}

void Maze::Kruskal()
{
	//store all valid walls in vector
	vector<Cell*> Walls;

	//every uneven number is a wall
	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			if (x % 2 != 0 || y % 2 != 0) {
				Walls.push_back(Cell_Grid[x][y]);
			}	
		}
	}

	// shuffle the Wall List
	shuffle(Walls.begin(), Walls.end(), rand_gen);
	
	for (auto wall : Walls) {

		vector<Cell*> Neighbors = getWalkableNeighborsFromWall(wall);

		if (Neighbors.size() == 2) {

			Cell* firstCell = Neighbors[0];
			Cell* secondCell = Neighbors[1];

			Cell* firstRoot = firstCell->findRoot();
			Cell* secondRoot = secondCell->findRoot();

			//different parents = different sets
			if (firstRoot != secondRoot) {
				//connect Cells and get former wall
				Cell* InBetween = connectCells(firstCell, secondCell);

				//set parents for joining sets
				secondRoot->setParent(firstRoot);

				//for recording
				firstCell->isActive = true;
				InBetween->isActive = true;
				secondCell->isActive = true;

				for (int i = 0; i < 2; i++) {
					record.recordStep(vector<Cell*>{firstCell, InBetween, secondCell});
				}

				//for recording
				firstCell->isActive = false;
				InBetween->isActive = false;
				secondCell->isActive = false;
				firstCell->wasVisited = true;
				InBetween->wasVisited = true;
				secondCell->wasVisited = true;
				record.recordStep(vector<Cell*>{firstCell, InBetween, secondCell});
			}
			else {
				// do nothing, cells already in same union
			}
		}
		else {
			//not enough walkable neighbors, just wall neighbors
			//nothing to do
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

					vector<Cell*> VisitedNeighbors;// = getVisitedNeighbors(cell);

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
	if (pos.getY() >= 2) {
		target_cell = Cell_Grid[X][Y - 2];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	//East possible
	if (pos.getX() < this->width - 2) {
		target_cell = Cell_Grid[X + 2][Y];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 2) {
		target_cell = Cell_Grid[X][Y + 2];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}	
	}

	//West possible
	if (pos.getX() >= 2) {
		target_cell = Cell_Grid[X - 2][Y];

		if (target_cell->wasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	return directions;
}

//needs to be called from a wall, does only one step in each direction
vector<Cell*> Maze::getWalkableNeighborsFromWall(Cell* cell) {

	vector<Cell*> directions;

	Point pos = cell->getPosition();
	int X = pos.getX();
	int Y = pos.getY();

	Cell* target_cell = nullptr;

	//north possible
	if (pos.getY() > 0) {
		target_cell = Cell_Grid[X][Y - 1];

		if(!target_cell->isWall){
			directions.push_back(target_cell);
		}
		
	}

	//East possible
	if (pos.getX() < this->width - 1) {
		target_cell = Cell_Grid[X + 1][Y];

		if (!target_cell->isWall) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 1) {
		target_cell = Cell_Grid[X][Y + 1];
		
		if (!target_cell->isWall) {
			directions.push_back(target_cell);
		}
	}

	//West possible
	if (pos.getX() > 0) {
		target_cell = Cell_Grid[X - 1][Y];

		if (!target_cell->isWall) {
			directions.push_back(target_cell);
		}
	}

	return directions;
}

Cell* Maze::connectCells(Cell* first, Cell* second)
{
	//get diff values to determin directions
	int diff_x = first->getPosition().getX() - second->getPosition().getX();
	int diff_y = first->getPosition().getY() - second->getPosition().getY();

	Cell* CellInBetween = nullptr;

	if (diff_x == 0) {
		CellInBetween = Cell_Grid[first->getPosition().getX()][first->getPosition().getY() - (diff_y/2)];
		CellInBetween->breakWall();
		//North
		if (diff_y > 0) {
			first->setNorth(CellInBetween);
			CellInBetween->setSouth(first);
			CellInBetween->setNorth(second);
			second->setSouth(CellInBetween);
		}
		//South
		if (diff_y < 0) {
			first->setSouth(CellInBetween);
			CellInBetween->setNorth(first);
			CellInBetween->setSouth(second);
			second->setNorth(CellInBetween);
		}
	}
	else {
		CellInBetween = Cell_Grid[first->getPosition().getX() - (diff_x/2)][first->getPosition().getY()];
		CellInBetween->breakWall();
		//West
		if (diff_x > 0) {
			first->setWest(CellInBetween);
			CellInBetween->setEast(first);
			CellInBetween->setWest(second);
			second->setEast(CellInBetween);
		}
		//East
		if (diff_x < 0) {
			first->setEast(CellInBetween);
			CellInBetween->setWest(first);
			CellInBetween->setEast(second);
			second->setWest(CellInBetween);
		}

	}
	return CellInBetween;
}

