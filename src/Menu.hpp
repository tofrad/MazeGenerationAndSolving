#pragma once


class Program;

typedef enum {
	OPEN,
	CLOSED,
	HOLD

} MenuState;

class Menu
{

	public:

		Menu();
		~Menu();

		void init(Program& P);

		void open();
		void close();

		void displayGUI();

	private:

		Program* program;

		MenuState state = CLOSED;

		float dropdown_height = 40;
		float dropdown_length = 250;
		float dropdown_thick = 30;

		int Maze_GUI = 0;
		int Path_GUI = 0;

		bool MazeEdit = false;
		bool PathEdit = false;

		
		

};

