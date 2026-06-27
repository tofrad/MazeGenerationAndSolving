#pragma once

#include "ProgramCallbacks.hpp"
#include "LayoutManager.hpp"

class Program;

enum class MenuState{
	OPEN,
	CLOSED,
	HOLD
} ;

class Menu
{

	public:

		Menu();
		~Menu();

		void init(const ProgramCallbacks& callbacks);

		void open();
		void close();

		void displayGUI();

	private:

		ProgramCallbacks menu_callbacks;

		LayoutManager& layout_manager = LayoutManager::GetInstance();

		void syncToProgram();

		void requestStateChange(ProgramState newState) const;

		MenuState state = MenuState::CLOSED;
		void UpdateRectValues();

		Vector2 anchor01 = { 0, 0 };
		//Menu Rects #######################################################################################
        Rectangle ListViewGeneration{ anchor01.x + 32, anchor01.y + 32, 344, 384 };
        Rectangle ListViewSolving{ anchor01.x + 776, anchor01.y + 32, 344, 384 };
        Rectangle CheckBoxWeights{ anchor01.x + 408, anchor01.y + 144, 24, 24 };
        Rectangle SliderSize{ anchor01.x + 408, anchor01.y + 112, 320, 16 };
        Rectangle GeneratorCanvas{ anchor01.x + 32, anchor01.y + 464, 656, 368 };
        Rectangle SolvingCanvas{ anchor01.x + 776, anchor01.y + 464, 656, 368 };
        Rectangle SpinnerWeightCnt{ anchor01.x + 408, anchor01.y + 216, 120, 24 };
        Rectangle SpinnerMinWeight{ anchor01.x + 408, anchor01.y + 304, 120, 24 };
        Rectangle TextBoxWeightCnt{ anchor01.x + 408, anchor01.y + 184, 120, 24 };
        Rectangle TextBoxMinWeight{ anchor01.x + 408, anchor01.y + 272, 120, 24 };
        Rectangle LineWeightsSeparate{ anchor01.x + 408, anchor01.y + 248, 344, 16 };
        Rectangle TextBoxSize{ anchor01.x + 408, anchor01.y + 72, 120, 24 };
        Rectangle TextBoxMaxWeight{ anchor01.x + 544, anchor01.y + 272, 120, 24 };
        Rectangle SpinnerMaxWeight{ anchor01.x + 544, anchor01.y + 304, 120, 24 };
        Rectangle GenerateButton{ anchor01.x + 408, anchor01.y + 360, 120, 24 };
        Rectangle SolveGenButton{ anchor01.x + 1152, anchor01.y + 360, 120, 24 };
        Rectangle ButtonExit{ anchor01.x + 1504, anchor01.y + 16, 80, 32 };
        Rectangle Rect_PlayEdit{ anchor01.x + 1424, anchor01.y + 264, 160, 136 };
        Rectangle ButtonEditor{ anchor01.x + 1448, anchor01.y + 296, 120, 24 };
        Rectangle ButtonPlayer{ anchor01.x + 1448, anchor01.y + 336, 120, 24 };
        Rectangle LinePlayerEditor{ anchor01.x + 1448, anchor01.y + 320, 120, 16 };

		Rectangle Scaled_ListViewGeneration = layout_manager.ScaleRect(ListViewGeneration);
		Rectangle Scaled_ListViewSolving = layout_manager.ScaleRect(ListViewSolving);
		Rectangle Scaled_CheckBoxWeights = layout_manager.ScaleRect(CheckBoxWeights);
		Rectangle Scaled_SliderSize = layout_manager.ScaleRect(SliderSize);
		Rectangle Scaled_GeneratorCanvas = layout_manager.ScaleRect(GeneratorCanvas);
		Rectangle Scaled_SolvingCanvas = layout_manager.ScaleRect(SolvingCanvas);
		Rectangle Scaled_SpinnerWeightCnt = layout_manager.ScaleRect(SpinnerWeightCnt);
		Rectangle Scaled_SpinnerMinWeight = layout_manager.ScaleRect(SpinnerMinWeight);
		Rectangle Scaled_TextBoxWeightCnt = layout_manager.ScaleRect(TextBoxWeightCnt);
		Rectangle Scaled_TextBoxMinWeight = layout_manager.ScaleRect(TextBoxMinWeight);
		Rectangle Scaled_LineWeightsSeparate = layout_manager.ScaleRect(LineWeightsSeparate);
		Rectangle Scaled_TextBoxSize = layout_manager.ScaleRect(TextBoxSize);
		Rectangle Scaled_TextBoxMaxWeight = layout_manager.ScaleRect(TextBoxMaxWeight);
		Rectangle Scaled_SpinnerMaxWeight = layout_manager.ScaleRect(SpinnerMaxWeight);
		Rectangle Scaled_GenerateButton = layout_manager.ScaleRect(GenerateButton);
		Rectangle Scaled_SolveGenButton = layout_manager.ScaleRect(SolveGenButton);
		Rectangle Scaled_ButtonExit = layout_manager.ScaleRect(ButtonExit);
		Rectangle Scaled_Rect_PlayEdit = layout_manager.ScaleRect(Rect_PlayEdit);
		Rectangle Scaled_ButtonEditor = layout_manager.ScaleRect(ButtonEditor);
		Rectangle Scaled_ButtonPlayer = layout_manager.ScaleRect(ButtonPlayer);
		Rectangle Scaled_LinePlayerEditor = layout_manager.ScaleRect(LinePlayerEditor);

		//GUI Variables-------------------------------------------------------
		int ListViewGenerationScrollIndex = 0;
		int ListViewGenerationActive = 0;

		int ListViewSolvingScrollIndex = 0;
		int ListViewSolvingActive = 0;

		bool CheckBoxWeightsChecked = false;

		float SliderSizeValue = 0.0f;
		char TextBoxSizeText[8] = "";

		int SpinnerWeightCntValue = 0;
		int SpinnerMinWeightValue = 0;
		int SpinnerMaxWeightValue = 0;

		int Maze_GUI = 0;
		int Path_GUI = 0;

		bool MazeEdit = false;
		bool PathEdit = false;

		bool Generate_Button_pressed = false;
		bool Solve_Button_pressed = false;
		bool Exit_Button_pressed = false;
		bool Editor_Button_pressed = false;
		bool Player_Button_pressed = false;

		//Values from GUI Input-----------------------------------------------

		//Maze-------------------------------------
		int MazeSize = 50;
		GenerationMethod MazeMethod = GenerationMethod::REC_BACKTRACKING;
		//Solver-----------------------------------
		SolvingMethod PathMethod = SolvingMethod::SM_BFS;
};

