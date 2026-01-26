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

		void init(ProgramCallbacks& callbacks);

		void open();
		void close();

		void displayPlayerGUI();


	private:

		ProgramCallbacks callbacks;

		PlayerState state = CLOSED;

		Recorder Record_Object;

		//GUI Variables-------------------------------------------------------
		int window_height = 1080;
		int window_width = 1920;

		void calculateLandmarks();

		UI_Rectangle Canvas;
		UI_Rectangle BelowCanvas;

		UI_Rectangle BottomRight;

		float General_Offset = 10; //10 px Offset

		//windows splited into parts ( 7 parts Canvas, 1 Part Right Column, with Offset from Borders and between) 
		float WindowSection = (window_width - (3 * General_Offset)) / 8;


		float Label_height = 30;

		
		//--------------------------------------------------------------------

};