#pragma once


class Program;

class Menu
{

	public:

		Menu();
		~Menu();

		void open(Program& P);

	private:

		float dropdown_height = 40;
		float dropdown_length = 250;
		float dropdown_thick = 30;

		int Maze_GUI = 0;
		int Path_GUI = 0;

		bool MazeEdit = false;
		bool PathEdit = false;

		void close();
		void displayGUI();

};

