#include "Program.hpp"

Program::Program()
{
	InitProgram();
    State = RUNNING; 
}

Program::~Program()
{

}

void Program::InitProgram()
{
	//create raylib dependencies
    // 
    // Initialization
    //--------------------------------------------------------------------------------------

    //SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Maze Generator with raylib");

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    ToggleBorderlessWindowed();

    SetTargetFPS(200);

    //--------------------------------------------------------------------------------------
    MazeSize = 100;

    M = Maze(MazeSize, screenWidth, screenHeight, HUNTANDKILL);

    S = Pathsolver(M.getGeneratedMaze(), M.getStart(), SM_BFS);

    //create buffer for drawing
    buffer = LoadRenderTexture(screenWidth, screenHeight);
    source = { 0, 0, (float)buffer.texture.width, (float) - buffer.texture.height};

    //BeginTextureMode(buffer);
    //M.drawMaze();
    //EndTextureMode();
    // 
    //--------------------------------------------------------------------------------------

    scale = min((float)GetScreenWidth() / screenWidth, (float)GetScreenHeight() / screenHeight);

}

int Program::Run()
{

    // Main program loop
    while (!WindowShouldClose())
    {
        //write changes into buffer
        BeginTextureMode(buffer);

        if (M.playRecording()) {
            SetTargetFPS(1450);
        }
        else {
            S.playRecording();
            SetTargetFPS(170);
        }

        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        //display buffer
        //DrawTextureRec(buffer.texture, source, Vector2{0, 0}, WHITE);

        DrawTexturePro(buffer.texture,
            source,
            Rectangle{ (GetScreenWidth() - ((float)screenWidth * scale)) * 0.5f, (GetScreenHeight() - ((float)screenHeight * scale)) * 0.5f,(float)screenWidth * scale, (float)screenHeight * scale },
            Vector2{ 0, 0 },
            0.0f,
            WHITE);
        //DrawTexture(buffer.texture, 0, 0, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();

    State = STOPPED;
	return 0;
}
