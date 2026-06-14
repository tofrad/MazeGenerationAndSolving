#include "Maze.hpp"

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
	rand_gen.seed(time(nullptr));
	createConnectedMaze();
}

Maze::Maze(const int w, const int screenwidth, const int screenheight, const GenerationMethod method)
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

	int temp_height = (this->width * 9) / 16;
	if (temp_height % 2 == 0) {
		temp_height = temp_height - 1;
	}

	if (temp_height > MAX_HEIGHT) {
		this->height = MAX_HEIGHT;
	}
	else if (temp_height < MIN_HEIGHT) {
		this->height = MIN_HEIGHT;
	}
	else {
		this->height = temp_height;
	}

	usable_height = screenheight - (OFFSET);
	usable_width = screenwidth - (OFFSET);

	this->cellsize = min(usable_width / this->width, usable_height / this->height);

	//this->height = (int)(usable_height / this->cellsize);
	//this->width = (int)(usable_width / this->cellsize);

	rand_gen.seed(time(nullptr));
	generateMaze(method);
	
}

Maze::~Maze()
{

}

Maze::Maze(const int screenwidth, const int screenheight, const TileMap* custom_maze)
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

Cell* Maze::getStart() const
{
	return Start;
}

vector<Cell*> Maze::getGeneratedMaze()
{
	return Cell_List;
}

int Maze::getCellsize() const
{
	return this->cellsize;
}

int Maze::getHeight() const
{
	return this->height;
}

int Maze::getWidth() const
{
	return this->width;
}

void Maze::createEmptyMaze()
{
	Cell_List.clear();

	Cell_Grid = vector<vector<Cell*>>(width, vector<Cell*>(height, nullptr));

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			start_x = OFFSET / 2 + (usable_width - (width * cellsize)) / 2;
			start_y = OFFSET / 2 + (usable_height - (height * cellsize)) / 2;
			const Vector2 LocalOffset{ static_cast<float>(start_x), static_cast<float>(start_y) };
			Cell* C = new Cell(Point(x, y), cellsize, LocalOffset);

			if (x % 2 != 0 || y % 2 != 0) {
				C->makeWall();
			}
			C->setParent(C);
			Cell_List.push_back(C);
			Cell_Grid[x][y] = C;
		}
	}


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

void Maze::resetMaze() const
{
	for (const auto cell : this->Cell_List) {
		cell->resetCell();
	}

}

void Maze::drawMaze() const
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

void Maze::displayInitialFrame() const
{
	record.playInitialGrid();
}

void Maze::generateMaze(const GenerationMethod method)
{
	createEmptyMaze();

	constexpr int rand = 0; // GetRandomValue(0, Cell_List.size() - 1);
	const int rand2 = (height * width) - 1;// GetRandomValue(0, Cell_List.size() - 1); //(height*width) - 1;

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
		//shouldn't land here

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
	for (const auto cell : Cell_List) {
		cell->wasVisited = false;
		cell->isActive = false;
	}

	record.saveLastFrame(Cell_List);
}

void Maze::drawCells() const
{
	for (const auto& cell : Cell_List) {
		
		cell->drawCell();
	}
}

void Maze::deleteConnections() const
{
	for (const auto Cell : Cell_List) {
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

	ranges::shuffle(directions, rand_gen);
	
	for (const auto target : directions) {

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
	ranges::shuffle(Walls, rand_gen);
	
	for (const auto wall : Walls) {

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
				Cell* InBetweenRoot = InBetween->findRoot();

				//set parents for joining sets
				secondRoot->setParent(firstRoot);
				InBetweenRoot->setParent(firstRoot);

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
	for (const auto cell : Cell_List) {
		cell->setParent(nullptr);
	}
}

void Maze::HuntAndKill()
{
	Cell* current_cell = Start;
	current_cell->wasVisited = true;

	while(true){

		vector<Cell*> neighbor_list = getUnvisitedNeighbors(current_cell);
		current_cell->wasVisited = true;
		bool found_new_cell = false;
		//get next cell
		if (!neighbor_list.empty()) {
			
			ranges::shuffle(neighbor_list, rand_gen);

			Cell* next_cell = neighbor_list[0];

			current_cell->isActive = true;
			for (int i = 0; i < 2; i++) {
				record.recordStep(current_cell);
			}

			Cell* InBetween = connectCells(current_cell, next_cell);
			InBetween->wasVisited = true;

			current_cell->isActive = false;
			record.recordStep({ current_cell, InBetween});

			current_cell = next_cell;
			found_new_cell = true; //mark to signal found cell
			continue;
		}
		else {
			record.recordStep(current_cell);//mark visited in recording
			found_new_cell = false;
			// iterate through all cells till found unvisited with visited Neighbors for new starting point	
			for (const auto cell : Cell_List) {

				if (cell->wasVisited == false && cell->isWall == false) {

					vector<Cell*> VisitedNeighbors = getVisitedNeighbors(cell);

					if (!VisitedNeighbors.empty()) {

						ranges::shuffle(VisitedNeighbors, rand_gen);

						Cell* neighbor = VisitedNeighbors[0];

						neighbor->isActive = true;
						for (int i = 0; i < 2; i++) {
							record.recordStep(neighbor);
						}

						Cell* InBetween = connectCells(cell, neighbor);
						InBetween->wasVisited = true;

						current_cell = cell;

						found_new_cell = true; //mark to signal found cell

						neighbor->isActive = false;
						record.recordStep({ neighbor, InBetween });

						break;
					}
				}
			}
		}

		//no valid cell is left , abort
		if (found_new_cell == false) {
			break;
		}
	}

}

vector<Cell*> Maze::getUnvisitedNeighbors(Cell* cell) const
{

	vector<Cell*> directions;
	

	Point pos = cell->getPosition();
	const int X = pos.getX();
	const int Y = pos.getY();

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

vector<Cell*> Maze::getVisitedNeighbors(const Cell* cell) const
{

	vector<Cell*> directions;


	Point pos = cell->getPosition();
	const int X = pos.getX();
	const int Y = pos.getY();

	Cell* target_cell = nullptr;

	//north possible
	if (pos.getY() >= 2) {
		target_cell = Cell_Grid[X][Y - 2];

		if (target_cell->wasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	//East possible
	if (pos.getX() < this->width - 2) {
		target_cell = Cell_Grid[X + 2][Y];

		if (target_cell->wasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 2) {
		target_cell = Cell_Grid[X][Y + 2];

		if (target_cell->wasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	//West possible
	if (pos.getX() >= 2) {
		target_cell = Cell_Grid[X - 2][Y];

		if (target_cell->wasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	return directions;
}

//needs to be called from a wall, does only one step in each direction
vector<Cell*> Maze::getWalkableNeighborsFromWall(const Cell* cell) const
{

	vector<Cell*> directions;

	Point pos = cell->getPosition();
	const int X = pos.getX();
	const int Y = pos.getY();

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

Cell* Maze::connectCells(Cell* first, Cell* second) const
{
	int x1 = first->getPosition().getX();
	int y1 = first->getPosition().getY();
	int x2 = second->getPosition().getX();
	int y2 = second->getPosition().getY();

	//get diff values to determine directions
	int diff_x = first->getPosition().getX() - second->getPosition().getX();
	int diff_y = first->getPosition().getY() - second->getPosition().getY();

	Cell* CellInBetween = nullptr;

	if (x1 == x2) {
		const int midY = (y1 + y2) / 2;
		CellInBetween = Cell_Grid[x1][midY];
		CellInBetween->breakWall();
		
		if (y1 < y2) { //first north of second
			first->setSouth(CellInBetween);
			CellInBetween->setNorth(first);
			CellInBetween->setSouth(second);
			second->setNorth(CellInBetween);
		}
		else {
			first->setNorth(CellInBetween);
			CellInBetween->setSouth(first);
			CellInBetween->setNorth(second);
			second->setSouth(CellInBetween);
		}
	}
	else {
		const int midX = (x1 + x2) / 2;
		CellInBetween = Cell_Grid[midX][y1];
		CellInBetween->breakWall();

		if (x1 < x2) { //first west of second
			first->setEast(CellInBetween);
			CellInBetween->setWest(first);
			CellInBetween->setEast(second);
			second->setWest(CellInBetween);
		}
		else {
			first->setWest(CellInBetween);
			CellInBetween->setEast(first);
			CellInBetween->setWest(second);
			second->setEast(CellInBetween);
		}

	}
	return CellInBetween;
}

