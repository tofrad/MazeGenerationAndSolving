#pragma once

#include "raylib.h"

#include "ProgramCallbacks.hpp"

#include "Recorder.hpp"

class Program;


typedef enum {
	OPEN,
	CLOSED

}PlayerState;

class Player {

	public:

		Player();
		~Player();

		//void init(ProgramCallbacks& callbacks);

		void open();
		void close();

		void displayPlayerGUI();


	private:

		//ProgramCallbacks callbacks;

		PlayerState state = CLOSED;

		Recorder Record_Object;

		//GUI Variables-------------------------------------------------------

		
		//--------------------------------------------------------------------

};