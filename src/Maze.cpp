#include "Maze.hpp"
#include <algorithm>
#include <chrono>
#include <random>

#include <memory>
#include "Global_Config.hpp"

#define MAX_HEIGHT 249
#define MAX_WIDTH 249

#define MIN_HEIGHT 11
#define MIN_WIDTH 11


int start_x;
int start_y;

Maze::Maze()
{

}

Maze::Maze(const int w, const int h, const GenerationMethod method, Recorder* recorder, const int weight_cnt, const int max_weight )
{
	this->height = h;
	this->width = w;

	const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	rand_gen.seed(static_cast<unsigned int>(ms));

	generateMaze(method, recorder, weight_cnt, max_weight);
}

Maze::~Maze()
{
}

Maze::Maze(const TileMap* custom_maze, Recorder* recorder)
{
	this->width = custom_maze->size;
	this->height = custom_maze->height;

	createConnectedMaze();

	for (int x = 0; x < this->width; x++) {

		for (int y = 0; y < this->height; y++) {

			switch (custom_maze->TileArray[x][y]) {
			case 0:
				//cell clear
				break;

			case WALL_CHAR:
				//cell wall
				Cell_Grid[x][y]->makeWall();
				break;

			case START_CHAR:
				//cell start
				Cell_Grid[x][y]->setStart();
				Start = Cell_Grid[x][y];
				break;

			case TARGET_CHAR:
				//cell target
				Cell_Grid[x][y]->setTarget();
				Target = Cell_Grid[x][y];
				break;

			default:
				//cell with weight
				//weight in same char array, with offset to 3 bc of 3 cases start,target, wall
				const int weight = custom_maze->TileArray[x][y];
				// TODO
				// add global params
				if (weight >= 0 && weight <= 100)
				{
					Cell_Grid[x][y]->setWeight(weight);
				}

				break;
			}
		}
	}
	record = recorder;
	record->setRecordSize(this->height, this->width);
	record->saveInitialFrame(Cell_List);
}

void Maze::GetTileMapFromMaze(TileMap& custom) const
{

	for (int x = 0; x < this->width; x++)
	{
		for (int y = 0; y < this->height; y++)
		{
			const auto flags = Cell_Grid[x][y]->getMazeFlags_Next();
			if (flags->isStart )
			{
				//set start
				custom.TileArray[x][y] = START_CHAR;
				custom.Start = {x,y};
			}else if (flags->isTarget )
			{
				//set target
				custom.TileArray[x][y] = TARGET_CHAR;
				custom.Target = {x,y};
			}else if (flags->isWall )
			{
				//set wall
				custom.TileArray[x][y] = WALL_CHAR;
			}else
			{
				//normal cell
				custom.TileArray[x][y] = 0;
			}
		}
	}
}

Cell* Maze::getStart() const
{
	return Start;
}

vector<Cell*> Maze::getGeneratedMaze()
{
	return Cell_List;
}

int Maze::getHeight() const
{
	return this->height;
}

int Maze::getWidth() const
{
	return this->width;
}

void Maze::createConnectedMaze()
{
	createNoWallMaze();

	//connect with neighbors
	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			Cell* currentCell = Cell_Grid[x][y];

			//set bordering cells
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

			Cell* C = new Cell(Point(x, y));
			C->setId(cell_id_cnt);
			cell_id_cnt++;

			if (x % 2 != 0 || y % 2 != 0) {
				C->makeWall();
			}
			C->setParent(C);
			Cell_List.push_back(C);
			Cell_Grid[x][y] = C;
		}
	}
}

void Maze::createNoWallMaze()
{
	Cell_List.clear();

	Cell_Grid = vector<vector<Cell*>>(width, vector<Cell*>(height, nullptr));

	for (int x = 0; x < width; x++) {

		for (int y = 0; y < height; y++) {

			Cell* C = new Cell(Point(x, y));
			C->setId(cell_id_cnt);
			cell_id_cnt++;


			C->setParent(C);
			Cell_List.push_back(C);
			Cell_Grid[x][y] = C;
		}
	}
}

void Maze::resetMaze() const
{
	for (const auto cell : this->Cell_List) {
		cell->resetCell();
	}
}

void Maze::generateMaze(const GenerationMethod method, Recorder* recorder, const int weight_cnt, const int max_weight)
{
	createEmptyMaze();

	constexpr int rand = 0; // GetRandomValue(0, Cell_List.size() - 1);
	const int rand2 = (height * width) - 1;// GetRandomValue(0, Cell_List.size() - 1); //(height*width) - 1;

	Cell_List[rand]->setStart();
	Start = Cell_List[rand];

	Cell_List[rand2]->setTarget();
	Target = Cell_List[rand2];

	std::unique_ptr<Recorder> tempRecorder;
	if (recorder == nullptr)
	{
		tempRecorder = std::make_unique<Recorder>();
		recorder = tempRecorder.get();
	}
	//The address of the local variable tempRecorder may escape the function ->case doesn't matter just a temp mock object
	record = recorder;
	record->startRecording();
	record->saveInitialFrame(Cell_List);

	switch (method) {

	case REC_BACKTRACKING:
		RecursiveBacktracking(Start, 0);
		break;

	case KRUSKAL:
		Kruskal();
		break;

	case HUNTANDKILL:
		HuntAndKill();
		break;

	case CUSTOM:
		//Cell_List[rand]->next_flags.isStart = false;
		//Start = nullptr;

		//Cell_List[rand2]->next_flags.isTarget = false;
		//Target = nullptr;
		break;

	default:
		HuntAndKill();
		break;

	}

	if (weight_cnt > 0)
	{
		setWeights(weight_cnt, max_weight);
	}

	record->stopRecording();
	record->saveLastFrame(Cell_List);

	//clear was_visited mark for coloring in pathfinding
	// for (const auto cell : Cell_List) {
	// 	cell->maze_next_flags.Maze_CellWasVisited = false;
	// 	cell->maze_next_flags.isActive = false;
	// }
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

void Maze::RecursiveBacktracking(Cell* cell, const uint step)
{
	vector<Cell*> directions;
	Point pos = cell->getPosition();

	if (!cell->maze_next_flags.Maze_CellWasVisited) {
		directions = getUnvisitedNeighbors(cell);
	}
	// //mark as visited
	cell->maze_next_flags.Maze_CellWasVisited = true;

	ranges::shuffle(directions, rand_gen);

	if (directions.size() == 0)
	{
		//record active once, is dead end,
		record->recordStep({cell});
		cell->maze_next_flags.isActive = false;
		record->recordStep({cell});
	}
	else
	{
		int cell_cnt = 0;
		for (const auto target : directions) {

			cell_cnt++;

			if (!target->maze_next_flags.Maze_CellWasVisited) {

				Cell* MiddleCell = connectCells(cell, target);
				MiddleCell->maze_next_flags.Maze_CellWasVisited = true;
				MiddleCell->maze_next_flags.isActive = true;
				target->maze_next_flags.isActive = true;

				//record current cells as active
				record->recordStep({cell, MiddleCell, target});

				RecursiveBacktracking(target, step + 1);

				//if last cell in iteration clear active and record, also check neighbors again
				//because they could have been visited in the meantime
				if (cell_cnt == directions.size() || getUnvisitedNeighbors(cell).size() == 0)
				{
					cell->maze_next_flags.isActive = false;
					MiddleCell->maze_next_flags.isActive = false;
					target->maze_next_flags.isActive = false;
					record->recordStep({cell, MiddleCell, target});
				}
				else
				{
					MiddleCell->maze_next_flags.isActive = false;
					record->recordStep({MiddleCell});
				}
			}
		}
	}
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
				firstCell->maze_next_flags.isActive = true;
				InBetween->maze_next_flags.isActive = true;
				secondCell->maze_next_flags.isActive = true;

				record->recordStep(vector<Cell*>{firstCell, InBetween, secondCell});

				//for recording
				firstCell->maze_next_flags.isActive = false;
				InBetween->maze_next_flags.isActive = false;
				secondCell->maze_next_flags.isActive = false;
				firstCell->maze_next_flags.Maze_CellWasVisited = true;
				InBetween->maze_next_flags.Maze_CellWasVisited = true;
				secondCell->maze_next_flags.Maze_CellWasVisited = true;
				record->recordStep(vector<Cell*>{firstCell, InBetween, secondCell});
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
	current_cell->maze_next_flags.Maze_CellWasVisited = true;
	vector<Cell*> carvedPath;

	while(true){

		vector<Cell*> neighbor_list = getUnvisitedNeighbors(current_cell);

		current_cell->maze_next_flags.Maze_CellWasVisited = true;
		bool found_new_cell = false;

		//get next cell from neighbors
		if (!neighbor_list.empty()) {

			if (neighbor_list.size() )
			ranges::shuffle(neighbor_list, rand_gen);

			Cell* next_cell = neighbor_list[0];

			current_cell->maze_next_flags.isActive = true;
			next_cell->maze_next_flags.isActive = true;

			Cell* InBetween = connectCells(current_cell, next_cell);
			InBetween->maze_next_flags.Maze_CellWasVisited = true;
			InBetween->maze_next_flags.isActive = true;

			record->recordStep({ current_cell, InBetween, next_cell});

			carvedPath.push_back(current_cell);
			carvedPath.push_back(InBetween);

			current_cell->maze_next_flags.isActive = false;
			InBetween->maze_next_flags.isActive = false;

			current_cell = next_cell;
			continue;
		}
		//current cell is an active cell in a dead end
		current_cell->maze_next_flags.isActive = false;
		carvedPath.push_back(current_cell);

		//record and clear carved path
		record->recordStep({carvedPath});
		carvedPath.clear();

		//mark visited in recording
		found_new_cell = false;
		// iterate through all cells till found unvisited with visited Neighbors for new starting point
		for (const auto cell : Cell_List) {

			if (cell->maze_next_flags.Maze_CellWasVisited == false && cell->maze_next_flags.isWall == false) {

				vector<Cell*> VisitedNeighbors = getVisitedNeighbors(cell);

				if (!VisitedNeighbors.empty()) {

					ranges::shuffle(VisitedNeighbors, rand_gen);

					Cell* neighbor = VisitedNeighbors[0];

					neighbor->maze_next_flags.isActive = true;

					Cell* InBetween = connectCells(cell, neighbor);
					InBetween->maze_next_flags.Maze_CellWasVisited = true;
					InBetween->maze_next_flags.isActive = true;

					current_cell = cell;

					found_new_cell = true; //mark to signal found cell
					current_cell->maze_next_flags.isActive = true;
					record->recordStep({ neighbor, InBetween, current_cell });
					InBetween->maze_next_flags.isActive = false;
					neighbor->maze_next_flags.isActive = false;
					carvedPath.push_back(neighbor);
					carvedPath.push_back(InBetween);

					break;
				}
			}
		}
		//no valid cell is left , abort
		if (found_new_cell == false) {
			break;
		}
	}
}

void Maze::setWeights(const int weight_count, const int max_weight)
{
	int cells_changed = 0;
	std::uniform_int_distribution<> distribution(0, Cell_List.size() - 1);

	std::vector<bool> used(Cell_List.size(), false);

	while (cells_changed < weight_count)
	{
		const size_t pos = distribution(rand_gen);

		//index was not used prior
		if (not used[pos])
		{
			//add weight to cell via member
			if (not Cell_List[pos]->getMazeFlags_Next()->isWall)
			{
				const auto current_cell = Cell_List[pos];

				current_cell->addWeight(max_weight);

				//helper to check neighboring cells already visited THIS iteration
				std::vector<bool> recursion_used(Cell_List.size(), false);
				recursion_used[pos] = true;

				//get initial neighbors of first cell
				std::vector<Cell*> adjCells;

				adjCells.push_back(current_cell->getNorth());
				adjCells.push_back(current_cell->getEast());
				adjCells.push_back(current_cell->getSouth());
				adjCells.push_back(current_cell->getWest());

				//advance through neighbors till weight is 0, decreasing it every distance
				for (int w = max_weight - 1; w > 0; --w)
				{
					//save last adjCells in helper to iterate trough
					std::vector<Cell*> temp_adjCells = adjCells;

					//clear for next iteration
					adjCells.clear();

					//get through every neighbor add weight and get its neighbors
					for (const auto cell: temp_adjCells)
					{
						//if real and not called during this iteration, handle it
						if (Cell_isVisitable(cell) && not recursion_used[cell->getCellID()])
						{
							cell->addWeight(w);
							recursion_used[cell->getCellID()] = true;

							adjCells.push_back(cell->getNorth());
							adjCells.push_back(cell->getEast());
							adjCells.push_back(cell->getSouth());
							adjCells.push_back(cell->getWest());
						}
					}
				}

				//save original cell as already added
				used[pos] = true;
				cells_changed++;

			}
			//pos cell was wall, add it to used to save effort
			used[pos] = true;
		}
	}
}

bool Maze::Cell_isVisitable(const Cell* cell)
{
	if (cell != nullptr && !cell->maze_next_flags.isWall) {
		return true;
	}
	return false;
}

vector<Cell*> Maze::getUnvisitedNeighbors(const Cell* cell) const
{

	vector<Cell*> directions;
	const Point pos = cell->getPosition();
	const int X = pos.getX();
	const int Y = pos.getY();

	Cell* target_cell = nullptr;
	
	//north possible
	if (pos.getY() >= 2) {
		target_cell = Cell_Grid[X][Y - 2];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	//East possible
	if (pos.getX() < this->width - 2) {
		target_cell = Cell_Grid[X + 2][Y];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == false) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 2) {
		target_cell = Cell_Grid[X][Y + 2];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == false) {
			directions.push_back(target_cell);
		}	
	}

	//West possible
	if (pos.getX() >= 2) {
		target_cell = Cell_Grid[X - 2][Y];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == false) {
			directions.push_back(target_cell);
		}
	}
	return directions;
}

vector<Cell*> Maze::getVisitedNeighbors(const Cell* cell) const
{
	vector<Cell*> directions;
	const Point pos = cell->getPosition();
	const int X = pos.getX();
	const int Y = pos.getY();

	Cell* target_cell = nullptr;

	//north possible
	if (pos.getY() >= 2) {
		target_cell = Cell_Grid[X][Y - 2];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	//East possible
	if (pos.getX() < this->width - 2) {
		target_cell = Cell_Grid[X + 2][Y];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 2) {
		target_cell = Cell_Grid[X][Y + 2];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == true) {
			directions.push_back(target_cell);
		}
	}

	//West possible
	if (pos.getX() >= 2) {
		target_cell = Cell_Grid[X - 2][Y];

		if (target_cell->maze_next_flags.Maze_CellWasVisited == true) {
			directions.push_back(target_cell);
		}
	}
	return directions;
}

//needs to be called from a wall, does only one step in each direction
vector<Cell*> Maze::getWalkableNeighborsFromWall(const Cell* cell) const
{
	vector<Cell*> directions;
	const Point pos = cell->getPosition();
	const int X = pos.getX();
	const int Y = pos.getY();

	Cell* target_cell = nullptr;

	//north possible
	if (pos.getY() > 0) {
		target_cell = Cell_Grid[X][Y - 1];

		if(!target_cell->maze_next_flags.isWall){
			directions.push_back(target_cell);
		}
		
	}

	//East possible
	if (pos.getX() < this->width - 1) {
		target_cell = Cell_Grid[X + 1][Y];

		if (!target_cell->maze_next_flags.isWall) {
			directions.push_back(target_cell);
		}
	}

	//south possible
	if (pos.getY() < this->height - 1) {
		target_cell = Cell_Grid[X][Y + 1];
		
		if (!target_cell->maze_next_flags.isWall) {
			directions.push_back(target_cell);
		}
	}

	//West possible
	if (pos.getX() > 0) {
		target_cell = Cell_Grid[X - 1][Y];

		if (!target_cell->maze_next_flags.isWall) {
			directions.push_back(target_cell);
		}
	}
	return directions;
}

Cell* Maze::connectCells(Cell* first, Cell* second) const
{
	const int x1 = first->getPosition().getX();
	const int y1 = first->getPosition().getY();
	const int x2 = second->getPosition().getX();
	const int y2 = second->getPosition().getY();

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

