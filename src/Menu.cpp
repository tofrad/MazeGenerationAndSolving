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
    ClearBackground(GRAY);

    int window_height;
    int window_width;

    switch (menu_callbacks.getWindowSize()) {
    case UHD:
        window_height = 1440;
        window_width = 2560;
        break;
    case FHD:
        window_height = 1080;
        window_width = 1920;
        break;
    case WSXGA:
        window_height = 900;
        window_width = 1600;
        break;
    case SMALL:
        window_height = 540;
        window_width = 960;
        break;
    default:
        window_height = 540;
        window_width = 960;
        break;
    }

    constexpr float Label_offset = 10;

    const float Labelwidth = (window_width - (4*Label_offset)) / 3;

    if (state == MenuState::OPEN) {
        constexpr float Label_height = 30;
        constexpr float Labelpos_y = 10;

        float x = Label_offset;
        constexpr float y = Labelpos_y;
        
        //GuiListView(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick}, "BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &test , &Maze_GUI);
        // 
        //Terminate Section ##############################################################################################################################################################
        if (GuiButton(Rectangle{window_width - Label_offset - Labelwidth, window_height - Label_offset - 2*Label_height, Labelwidth, Label_height }, "Exit Program")) {

            Exit_Button = true;
        }

        //Generator Section ##############################################################################################################################################################

        if (GuiButton(Rectangle{ x, y + 3 * Label_height, Labelwidth, Label_height }, "Generate")) {

            Generate_Button = true;
        }

        GuiLabel(Rectangle{ x, y, Labelwidth, Label_height }, "Maze Generator");
        if (GuiDropdownBox(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick }, "BACKTRACKING;KRUSKAL;HUNTANDKILL;CUSTOM", &Maze_GUI, MazeEdit)) {

            MazeEdit = !MazeEdit;
            MazeMethod = static_cast<GenerationMethod>(Maze_GUI);
        }

        //Solver Section #################################################################################################################################################################

        x = x + Labelwidth + Label_offset;

        if (GuiButton(Rectangle{ x, y + 3 * Label_height, Labelwidth, Label_height }, "Solve")) {

            Solve_Button = true;
        }
        
        GuiLabel(Rectangle{ x , y, Labelwidth, Label_height }, "Path Solver");
        if (GuiDropdownBox(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick }, "BFS;DFS", &Path_GUI, PathEdit)) {

            PathEdit = !PathEdit;
            PathMethod = static_cast<SolvingMethod>(Path_GUI);
        }

        //Window Section ################################################################################################################################################################

        x = x + Labelwidth + Label_offset;

        GuiLabel(Rectangle{ x , y, Labelwidth, Label_height }, "Window Size");
        if (GuiDropdownBox(Rectangle{ x, y + Label_height + 1, Labelwidth, dropdown_thick }, "UHD;FHD;WSXGA;SMALL", &Window_GUI, WindowEdit)) {

            WindowEdit = !WindowEdit;
            WindowSize = static_cast<Screensize>(Window_GUI);
            menu_callbacks.onWindowRequest(WindowSize);
        }

        //Resolve Actions here
        if (Generate_Button) {

            menu_callbacks.onGenerateRequest(MazeSize, MazeMethod);
            //menu_callbacks.onSolveRequest(PathMethod);

            Generate_Button = false;
        }

        if (Solve_Button) {

            menu_callbacks.onSolveRequest(PathMethod);

            Solve_Button = false;
        }

        if (Exit_Button) {

            requestStateChange(STOPPED);

            Exit_Button = false;
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
    if (menu_callbacks.getWindowSize) {
        WindowSize = menu_callbacks.getWindowSize();
    }
    Maze_GUI = static_cast<int>(MazeMethod);
    Path_GUI = static_cast<int>(PathMethod);
    Window_GUI = static_cast<int>(WindowSize);
}

void Menu::requestStateChange(const ProgramState newState) const
{
    menu_callbacks.onStateRequest(newState);
}
