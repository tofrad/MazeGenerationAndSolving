#pragma once

#include "../lib/raylib/include/raylib.h"

#include "ProgramCallbacks.hpp"
#include "LayoutManager.hpp"
#include "Recorder.hpp"

class Program;

enum class PlayerState
{
	OPEN,
	INITIAL,
	FORWARD_ONE,
	BACKWARD_ONE,
	PLAYING_FORWARD,
	PLAYING_BACKWARD,
	FRAME_REQUEST,
	UNPAUSE,
	PAUSED,
	END,
	CLOSED
};

class Player {

	public:

		Player();
		~Player();

		void init(const ProgramCallbacks& callbacks);

		void open(Recorder* Rec);
		void open(Recorder* Rec_Maze, Recorder* Rec_Path);
		void close();

		void displayPlayerGUI();


	private:

		ProgramCallbacks player_callbacks;

		LayoutManager& layout_manager = LayoutManager::GetInstance();

		PlayerState state = PlayerState::CLOSED;

		Recorder* Record_Maze = nullptr;
		Recorder* Record_Path = nullptr;

		Recorder* Record_Object = nullptr;

		void setState(PlayerState new_state);

		void resolveRenderAction();
		void updateSliderFloat();

		//GUI Variables-------------------------------------------------------
		int window_width = 1600;
		int window_height = 900;

		bool is_paused = true;

		Vector2 anchor01 = { 0, 0 };
		//Defining Rects
		Rectangle Canvas{anchor01.x + 16, anchor01.y + 16, 1428, 802};
		Rectangle Sidebar{ anchor01.x + 1456, anchor01.y + 16, 134, 802 }; //Dummy Side rect
		//Record Controls
		Rectangle Button_FullRewind{anchor01.x + 440, anchor01.y + 856, 40, 32};
		Rectangle Button_Rewind{anchor01.x + 496, anchor01.y + 856, 40, 32 };
		Rectangle Button_Pause{anchor01.x + 552, anchor01.y + 856, 72, 32 };
		Rectangle Button_Forward{anchor01.x + 640, anchor01.y + 856, 40, 32};
		Rectangle Button_FullForward{anchor01.x + 696, anchor01.y + 856, 40, 32 };
		//UnderCanvas
		Rectangle ToggleSlider_Recordtype{anchor01.x + 1120, anchor01.y + 864, 136, 24};
		Rectangle Slider{anchor01.x + 16, anchor01.y + 824, 1096, 24};
		Rectangle Slider_TextBox{anchor01.x + 1120, anchor01.y + 824, 64, 24 };
		Rectangle MaxValue_TextBox{anchor01.x + 1192, anchor01.y + 824, 64, 24};
		Rectangle ButtonMenu{ anchor01.x + 1464, anchor01.y + 832, 120, 24 };
		//Scaled Values for updated drawing
		Rectangle Scaled_Canvas = layout_manager.ScaleRect(Canvas);
		Rectangle Scaled_Sidebar = layout_manager.ScaleRect(Sidebar);
		Rectangle Scaled_Button_FullRewind = layout_manager.ScaleRect(Button_FullRewind);
		Rectangle Scaled_Button_Rewind = layout_manager.ScaleRect(Button_Rewind);
		Rectangle Scaled_Button_Pause = layout_manager.ScaleRect(Button_Pause);
		Rectangle Scaled_Button_Forward = layout_manager.ScaleRect(Button_Forward);
		Rectangle Scaled_Button_FullForward = layout_manager.ScaleRect(Button_FullForward);
		Rectangle Scaled_ToggleSlider_Recordtype = layout_manager.ScaleRect(ToggleSlider_Recordtype);
		Rectangle Scaled_Slider = layout_manager.ScaleRect(Slider);
		Rectangle Scaled_Slider_TextBox = layout_manager.ScaleRect(Slider_TextBox);
		Rectangle Scaled_MaxValue_TextBox = layout_manager.ScaleRect(MaxValue_TextBox);
		Rectangle Scaled_ButtonMenu = layout_manager.ScaleRect(ButtonMenu);

		//GUI Values-------------------------------------------------
		int stepValue = 0;
		int maxValue = 0;

		int toggle_slider_val = 0;
		int toggle_slider_val_prev = 0;

		float slider_value_float = 0;
		int slider_value_int = 0;
		bool slider_dragging = false;
		bool slider_was_dragged = false;

		float General_Offset = 10; //10 px Offset

		//--------------------------------------------------------------------
};