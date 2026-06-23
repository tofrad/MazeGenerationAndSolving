#include "Player.hpp"
#include "../lib/raylib/include/raygui.h"
#include<cstdio>

Player::Player()
{
}

Player::~Player()
{
}

void Player::init(const ProgramCallbacks& callbacks) {

    this->player_callbacks = callbacks;
    calculateLandmarks();
    this->state = PlayerState::CLOSED;
}

void Player::open(Recorder* Rec) {
    Record_Object = Rec;
    this->state = PlayerState::OPEN;

    if (Record_Object != nullptr) {

        stepValue = Record_Object->getStep();
        maxValue = Record_Object->getSize();
        setState(PlayerState::PAUSED);
    }
}

void Player::open(Recorder* Rec_Maze, Recorder* Rec_Path) {
    Record_Maze = Rec_Maze;
    Record_Path = Rec_Path;

    Record_Object = Rec_Maze;

    this->state = PlayerState::OPEN;

    if (Record_Object != nullptr) {

        stepValue = Record_Object->getStep();
        maxValue = Record_Object->getSize();
        setState(PlayerState::PAUSED);
    }
}

void Player::close() {
    Record_Object = nullptr;
    this->state = PlayerState::CLOSED;
}

void Player::displayPlayerGUI()
{
    calculateLandmarks();
    ClearBackground(BLACK);
    //checking truth for toggle slider
    if (Record_Object != nullptr)
    {
        if (Record_Object->getRecordType() == MAZE)
        {
            toggle_slider_val = 0;
            toggle_slider_val_prev = 0;
        }
        else
        {
            toggle_slider_val = 1;
            toggle_slider_val_prev = 1;
        }
    }
    GuiToggleSlider(ToggleSlider_Recordtype, "MAZE ; PATH", &toggle_slider_val);
    if (toggle_slider_val != toggle_slider_val_prev)
    {
        //switch record object
        if (toggle_slider_val == 0)
        {
            Record_Object = Record_Maze;
        }else
        {
            Record_Object = Record_Path;
        }
        stepValue = Record_Object->getStep();
        maxValue = Record_Object->getSize();

        setState(PlayerState::PAUSED);

        toggle_slider_val_prev = toggle_slider_val;
    }
    if (Record_Object != nullptr)
    {
        //set pause design depending on status
        if (is_paused)
        {
            GuiSetState(STATE_PRESSED);
        }else
        {
            GuiSetState(STATE_NORMAL);
        }

        if (GuiButton(Button_Pause, GuiIconText(132, is_paused? "RESUME" : "PAUSE")))
        {
            //program was paused and now needs to resume
            if (is_paused)
            {
                setState(PlayerState::PLAYING_FORWARD);
            }else
            {
                setState(PlayerState::PAUSED);
            }
        }
        GuiSetState(STATE_NORMAL);

        if (GuiButton(Button_FullRewind, GuiIconText(129, ""))) { setState(PlayerState::INITIAL); }
        if (GuiButton(Button_Rewind, GuiIconText(130, ""))) {setState(PlayerState::BACKWARD_ONE); }
        if (GuiButton(Button_Forward, GuiIconText(131, ""))) { setState(PlayerState::FORWARD_ONE); }
        if (GuiButton(Button_FullForward, GuiIconText(134, ""))) { setState(PlayerState::END); }

        //Menu Button
        if (GuiButton(Rectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height),
                        GuiIconText(142, "Menu")))
        {
            player_callbacks.onStateRequest(MENU);
        }

        //max Value of Slider Textbox
        char maxValue_char[8];
        snprintf(maxValue_char, sizeof(maxValue_char), "%d", maxValue);

        //Slider handling ----------------------------------------------------------------------------------------------
        const int current_step = Record_Object->getStep();

        //not used, update from recorder
        if (!slider_dragging)
        {
            slider_value_float = static_cast<float>(current_step);
        }

        bool mouse_on_slider = CheckCollisionPointRec(GetMousePosition(), Slider);
        bool mouse_is_clicked = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        if (mouse_on_slider && mouse_is_clicked)
        {
            slider_dragging = true;
            slider_was_dragged = true;

        }else if (!mouse_is_clicked)
        {
            slider_dragging = false;
        }

        GuiSliderBar(Slider, "", "", &slider_value_float, 0, static_cast<float>(maxValue));
        slider_value_int = static_cast<int>(slider_value_float);

        //slider is dragged keep up the frame with the dragging
        if (slider_dragging)
        {
            setState(PlayerState::FRAME_REQUEST);
        }
        //when dragging stopped resole last action
        if (!slider_dragging && current_step != slider_value_int && slider_was_dragged)
        {
            setState(PlayerState::FRAME_REQUEST);
            slider_was_dragged = false;
        }
        char slide_buffer[8];
        snprintf(slide_buffer, sizeof(slide_buffer), "%d", slider_value_int);
        //--------------------------------------------------------------------------------------------------------------

        //Display Slider value Boxes
        DrawRectangleRec(Slider_TextBox, RAYWHITE);
        GuiTextBox(Slider_TextBox, slide_buffer, 11, false);

        DrawRectangleRec(MaxValue_TextBox, RAYWHITE);
        GuiTextBox(MaxValue_TextBox, maxValue_char, 11, false);

        //every input checked, resolve action if needed
        resolveRenderAction();
        const auto frame = Record_Object->getFrameTexture().texture;

        // Calculate scale to fit canvas
        const float scaleX = (float)Canvas.width / frame.width;
        const float scaleY = (float)Canvas.height / frame.height;
        const float scale = min(scaleX, scaleY);  // Maintain aspect ratio

        const auto source = Rectangle{ 0, static_cast<float>(frame.height), static_cast<float>(frame.width), -static_cast<float>(frame.height)};
        const float x = Canvas.Point.x + (Canvas.width - frame.width * scale) / 2;
        const float y = Canvas.Point.y + (Canvas.height - frame.height * scale) / 2;
        const auto dest = Rectangle{ x, y, frame.width * scale,frame.height * scale};

        // draw frame texture to canvas
        DrawTexturePro(
              frame,
              source,
              dest,
              Vector2{0,0},
              0,
              WHITE);
        //Debug Circle
        //DrawCircle(x, y, 10, RED);

        //visual debug
        // DrawRectangle(Canvas.Point.x, Canvas.Point.y, Canvas.width, Canvas.height, RAYWHITE);
        //DrawRectangle(BelowCanvas.Point.x, BelowCanvas.Point.y, BelowCanvas.width, BelowCanvas.height, RED);
        //DrawRectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height, GREEN);
    }else
    {
        //no valid record object here
        DrawRectangle(Canvas.Point.x, Canvas.Point.y, Canvas.width, Canvas.height, MAROON);
    }
}

void Player::setState(const PlayerState new_state)
{
    this->state = new_state;
    if (this->state == PlayerState::PAUSED)
    {
        //for pause button
        is_paused = true;
    }else if (  state == PlayerState::PLAYING_FORWARD ||
                state == PlayerState::FORWARD_ONE ||
                state == PlayerState::PLAYING_BACKWARD ||
                state == PlayerState::BACKWARD_ONE)
    {
        is_paused = false;
    }
}

void Player::resolveRenderAction()
{
    switch (state)
    {
        case PlayerState::PLAYING_FORWARD:
            if (!Record_Object->stepForward())
            {
                setState(PlayerState::PAUSED);
            }
            updateSliderFloat();
            break;

        case PlayerState::PLAYING_BACKWARD:
            if (!Record_Object->stepBackward())
            {
                setState(PlayerState::PAUSED);
            }
            updateSliderFloat();
            break;

        case PlayerState::BACKWARD_ONE:
            if (Record_Object->stepBackward())
            {
                this->setState(PlayerState::PAUSED);
                updateSliderFloat();
            }
            break;

        case PlayerState::FORWARD_ONE:
            if (Record_Object->stepForward())
            {
                this->setState(PlayerState::PAUSED);
                updateSliderFloat();
            }
            break;

        case PlayerState::FRAME_REQUEST:
            Record_Object->playStep(slider_value_int);
            this->setState(PlayerState::PAUSED);
            updateSliderFloat();

            break;

        case PlayerState::INITIAL:
            Record_Object->playInitialGrid();
            this->setState(PlayerState::PAUSED);
            updateSliderFloat();

            break;

        case PlayerState::END:
            Record_Object->playLastFrame();
            this->setState(PlayerState::PAUSED);
            updateSliderFloat();
            break;

        default:
            //do nothing as canvas needs no render
            break;
    }
}

void Player::updateSliderFloat()
{
    slider_value_int = Record_Object->getStep();
    slider_value_float = static_cast<float>(slider_value_int);
}

void Player::calculateLandmarks()
{
    switch (player_callbacks.getWindowSize()) {
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

    WindowSection_x = (window_width - (2 * General_Offset)) / x_section_cnt;
    WindowSection_y = (window_height - (2 * General_Offset)) / y_section_cnt;

    //Calculate Canvas as Center
    Canvas.width = 8 * WindowSection_x;
    Canvas.height = WindowSection_y * 9 - General_Offset;//(Canvas.width / 16) * 9;
    Canvas.Point = Vector2{ General_Offset, General_Offset};

    //BelowCanvas Field
    BelowCanvas.width = WindowSection_x * 7;
    BelowCanvas.Point = Vector2{ General_Offset, Canvas.Point.y + Canvas.height + General_Offset};
    BelowCanvas.height = WindowSection_y * 1 - General_Offset; // window_height - General_Offset - BelowCanvas.Point.y;


    //Bottom right Field
    BottomRight.width = 1 * WindowSection_x; //RightToCanvas.width;
    BottomRight.height = BelowCanvas.height;
    BottomRight.Point = Vector2{ BelowCanvas.Point.x + BelowCanvas.width, BelowCanvas.Point.y };


    const int Bottom_Split_x = BelowCanvas.width / 10;
    const int BetweenOffset = Bottom_Split_x / 4;

    const int Button_Size_x = Bottom_Split_x;

    const int Bottom_Split_y = BelowCanvas.height / 9;

    const int Button_Y = Bottom_Split_y * 5;
    const int ButtonSize_y = Bottom_Split_y * 4;

    const int Button_FirstFrame_x = 2 * Bottom_Split_x;
    const int Button_Rewind_x = Button_FirstFrame_x + Button_Size_x + BetweenOffset;
    const int Button_Pause_x = Button_Rewind_x + Button_Size_x + BetweenOffset;
    const int Button_Resume_x = Button_Pause_x + Button_Size_x + BetweenOffset;
    const int Button_LastFrame_x = Button_Resume_x + Button_Size_x + BetweenOffset;

    const int ToggleSlider_x = Button_LastFrame_x + Button_Size_x + BetweenOffset;

    
    Button_FullRewind = Rectangle(Button_FirstFrame_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_Rewind = Rectangle(Button_Rewind_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_Pause = Rectangle(Button_Pause_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_Forward = Rectangle(Button_Resume_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);
    Button_FullForward = Rectangle(Button_LastFrame_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);

    ToggleSlider_Recordtype = Rectangle(ToggleSlider_x, BelowCanvas.Point.y + Button_Y, Button_Size_x, ButtonSize_y);

    Slider = Rectangle(BelowCanvas.Point.x, BelowCanvas.Point.y, Bottom_Split_x * 8,Bottom_Split_y * 3);
    Slider_TextBox = Rectangle(BelowCanvas.Point.x + Bottom_Split_x * 8, BelowCanvas.Point.y, Bottom_Split_x, Bottom_Split_y * 4 );
    MaxValue_TextBox = Rectangle(BelowCanvas.Point.x + Bottom_Split_x * 9, BelowCanvas.Point.y, Bottom_Split_x, Bottom_Split_y * 4);

}
