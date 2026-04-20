#pragma once

#include "raylib.h"

#include "ProgramCallbacks.hpp"

#include "Recorder.hpp"

class Program;


class Player {

	public:

		Player();
		~Player();

		void init(ProgramCallbacks& callbacks);

		void open(Recorder* Rec);
		void close();

		void displayPlayerGUI();


	private:

		ProgramCallbacks callbacks;

		Recorder* Record_Object = nullptr;

		//GUI Variables-------------------------------------------------------
		int window_height = 1080;
		int window_width = 1920;

		void calculateLandmarks();

		int x_section_cnt = 8;
		int y_section_cnt = 10;

		UI_Rectangle Canvas;
		UI_Rectangle BelowCanvas;

		UI_Rectangle BottomRight;

		Rectangle Button_FullRewind;
		Rectangle Button_Rewind;
		Rectangle Button_Pause;
		Rectangle Button_Forward;
		Rectangle Button_FullForward;

		Rectangle Slider;
		Rectangle Slider_TextBox;
		Rectangle MaxValue_TextBox;

		//GUI Values-------------------------------------------------
		int stepValue = 0;
		int maxValue = 0;

		float slider_value_float = 0;
		int slider_value_int = 0;


		float General_Offset = 10; //10 px Offset

		//windows splited into parts ( 7 parts Canvas, 1 Part Right Column, with Offset from Borders and between) 
		int WindowSection_x = (window_width - (2 * General_Offset)) / x_section_cnt;
		int WindowSection_y = (window_height - (2 * General_Offset)) / y_section_cnt;

		float Label_height = 30;

		
		//--------------------------------------------------------------------

};