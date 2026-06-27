#include "Menu.hpp"
#include "Program.hpp"

#include "../lib/raylib/include/raygui.h"

Menu::Menu()
{
    
}

Menu::~Menu()
{

}

void Menu::init(const ProgramCallbacks& callbacks)
{
    this->menu_callbacks = callbacks;

    //Center Label
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
    syncToProgram();
}

void Menu::open()
{
    state = MenuState::OPEN;
    syncToProgram();
}

void Menu::close()
{
    state = MenuState::CLOSED;
}

void Menu::displayGUI()
{
    layout_manager.UpdateScale();
    UpdateRectValues();

    ClearBackground(LIGHTGRAY);
    //draw ui ###############################################################################################
    //Maze List View
    GuiListView(Scaled_ListViewGeneration,"Recursive Backtracking\nKruskal\nHunt & Kill", &ListViewGenerationScrollIndex, &Maze_GUI);
    //Path List View
    GuiListView(Scaled_ListViewSolving, "DFS\nBFS", &ListViewSolvingScrollIndex, &Path_GUI);

    GuiCheckBox(Scaled_CheckBoxWeights, "with weights", &CheckBoxWeightsChecked);

    GuiSlider(Scaled_SliderSize,"10", "100", &SliderSizeValue, 10, 100);
    MazeSize = static_cast<int>(SliderSizeValue);

    snprintf(TextBoxSizeText, sizeof(TextBoxSizeText), "%d", MazeSize);
    GuiTextBox(Scaled_TextBoxSize, TextBoxSizeText, DEFAULT, false);

    GuiSpinner(Scaled_SpinnerWeightCnt, "", &SpinnerWeightCntValue, 1, 50, false);
    GuiTextBox(Scaled_TextBoxWeightCnt, "number of weights", DEFAULT, false);

    GuiSpinner(Scaled_SpinnerMinWeight, "", &SpinnerMinWeightValue, 1, 50, false);
    GuiTextBox(Scaled_TextBoxMinWeight, "min weights val", DEFAULT, false);

    GuiSpinner(Scaled_SpinnerMaxWeight, "", &SpinnerMaxWeightValue, 1, 50, false);
    GuiTextBox(Scaled_TextBoxMaxWeight, "max weights val", DEFAULT, false);

    GuiLine(Scaled_LineWeightsSeparate, "");

    if (GuiButton(Scaled_GenerateButton, "Generate")){Generate_Button_pressed = true;}
    if (GuiButton(Scaled_SolveGenButton, "Solve")){Solve_Button_pressed = true;}
    if (GuiButton(Scaled_ButtonExit, "Exit")){Exit_Button_pressed = true;}

    DrawRectangleLinesEx(Scaled_Rect_PlayEdit, 3, BLACK);
    if (GuiButton(Scaled_ButtonEditor, "Editor")){Editor_Button_pressed = true;};
    if (GuiButton(Scaled_ButtonPlayer, "Player")){Player_Button_pressed = true;};
    GuiLine(Scaled_LinePlayerEditor, "");

    DrawRectangleRec(Scaled_GeneratorCanvas, SKYBLUE);
    DrawRectangleRec(Scaled_SolvingCanvas, BROWN);

    // resolve actions ######################################################################################
    if (state == MenuState::OPEN) {

        //Resolve Actions here
        if (Generate_Button_pressed) {
            menu_callbacks.onGenerateRequest(MazeSize, MazeMethod);
            Generate_Button_pressed = false;
        }
        if (Solve_Button_pressed) {
            menu_callbacks.onSolveRequest(PathMethod);
            Solve_Button_pressed = false;
        }
        if (Exit_Button_pressed) {
            requestStateChange(STOPPED);
            Exit_Button_pressed = false;
        }
        if (Editor_Button_pressed) {
            requestStateChange(EDITING);
            Editor_Button_pressed = false;
        }
        if (Player_Button_pressed) {
            requestStateChange(PLAYER);
            Player_Button_pressed = false;
        }
    }
}

void Menu::syncToProgram()
{
    if (menu_callbacks.getGenerator) {
        MazeMethod = menu_callbacks.getGenerator();
    }
    if (menu_callbacks.getMazeSize) {
        MazeSize = menu_callbacks.getMazeSize();
    }
    if (menu_callbacks.getSolver) {
        PathMethod = menu_callbacks.getSolver();
    }

    Maze_GUI = static_cast<int>(MazeMethod);
    Path_GUI = static_cast<int>(PathMethod);
}

void Menu::requestStateChange(const ProgramState newState) const
{
    menu_callbacks.onStateRequest(newState);
}

void Menu::UpdateRectValues()
{
    Scaled_ListViewGeneration = layout_manager.ScaleRect(ListViewGeneration);
    Scaled_ListViewSolving = layout_manager.ScaleRect(ListViewSolving);
    Scaled_CheckBoxWeights = layout_manager.ScaleRect(CheckBoxWeights);
    Scaled_SliderSize = layout_manager.ScaleRect(SliderSize);
    Scaled_GeneratorCanvas = layout_manager.ScaleRect(GeneratorCanvas);
    Scaled_SolvingCanvas = layout_manager.ScaleRect(SolvingCanvas);
    Scaled_SpinnerWeightCnt = layout_manager.ScaleRect(SpinnerWeightCnt);
    Scaled_SpinnerMinWeight = layout_manager.ScaleRect(SpinnerMinWeight);
    Scaled_TextBoxWeightCnt = layout_manager.ScaleRect(TextBoxWeightCnt);
    Scaled_TextBoxMinWeight = layout_manager.ScaleRect(TextBoxMinWeight);
    Scaled_LineWeightsSeparate = layout_manager.ScaleRect(LineWeightsSeparate);
    Scaled_TextBoxSize = layout_manager.ScaleRect(TextBoxSize);
    Scaled_TextBoxMaxWeight = layout_manager.ScaleRect(TextBoxMaxWeight);
    Scaled_SpinnerMaxWeight = layout_manager.ScaleRect(SpinnerMaxWeight);
    Scaled_GenerateButton = layout_manager.ScaleRect(GenerateButton);
    Scaled_SolveGenButton = layout_manager.ScaleRect(SolveGenButton);
    Scaled_ButtonExit = layout_manager.ScaleRect(ButtonExit);
    Scaled_Rect_PlayEdit = layout_manager.ScaleRect(Rect_PlayEdit);
    Scaled_ButtonEditor = layout_manager.ScaleRect(ButtonEditor);
    Scaled_ButtonPlayer = layout_manager.ScaleRect(ButtonPlayer);
    Scaled_LinePlayerEditor = layout_manager.ScaleRect(LinePlayerEditor);
}
