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
    layout_manager->UpdateScale();

    Scaled_Canvas = layout_manager->ScaleRect(Canvas);
    Scaled_Sidebar = layout_manager->ScaleRect(Sidebar);
    Scaled_Button_Rewind = layout_manager->ScaleRect(Button_Rewind);
    Scaled_Button_FullRewind = layout_manager->ScaleRect(Button_FullRewind);
    Scaled_Button_Pause = layout_manager->ScaleRect(Button_Pause);
    Scaled_Button_Forward = layout_manager->ScaleRect(Button_Forward);
    Scaled_Button_FullForward = layout_manager->ScaleRect(Button_FullForward);
    Scaled_ToggleSlider_Recordtype = layout_manager->ScaleRect(ToggleSlider_Recordtype);
    Scaled_Slider = layout_manager->ScaleRect(Slider);
    Scaled_Slider_TextBox = layout_manager->ScaleRect(Slider_TextBox);
    Scaled_MaxValue_TextBox = layout_manager->ScaleRect(MaxValue_TextBox);
    Scaled_ButtonMenu = layout_manager->ScaleRect(ButtonMenu);

    ClearBackground(LIGHTGRAY);
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
    GuiToggleSlider(Scaled_ToggleSlider_Recordtype, "MAZE ; PATH", &toggle_slider_val);
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

        if (GuiButton(Scaled_Button_Pause, GuiIconText(132, is_paused? "RESUME" : "PAUSE")))
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

        if (GuiButton(Scaled_Button_FullRewind, GuiIconText(129, ""))) { setState(PlayerState::INITIAL); }
        if (GuiButton(Scaled_Button_Rewind, GuiIconText(130, ""))) {setState(PlayerState::BACKWARD_ONE); }
        if (GuiButton(Scaled_Button_Forward, GuiIconText(131, ""))) { setState(PlayerState::FORWARD_ONE); }
        if (GuiButton(Scaled_Button_FullForward, GuiIconText(134, ""))) { setState(PlayerState::END); }

        if (GuiButton(Scaled_ButtonMenu,GuiIconText(142, "Menu")))
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

        bool mouse_on_slider = CheckCollisionPointRec(GetMousePosition(), Scaled_Slider);
        bool mouse_is_clicked = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        if (mouse_on_slider && mouse_is_clicked)
        {
            slider_dragging = true;
            slider_was_dragged = true;

        }else if (!mouse_is_clicked)
        {
            slider_dragging = false;
        }

        GuiSliderBar(Scaled_Slider, "", "", &slider_value_float, 0, static_cast<float>(maxValue));
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
        DrawRectangleRec(Scaled_Slider_TextBox, RAYWHITE);
        GuiTextBox(Scaled_Slider_TextBox, slide_buffer, 11, false);

        DrawRectangleRec(Scaled_MaxValue_TextBox, RAYWHITE);
        GuiTextBox(Scaled_MaxValue_TextBox, maxValue_char, 11, false);

        //every input checked, resolve action if needed
        resolveRenderAction();
        const auto frame = Record_Object->getFrameTexture().texture;

        // Calculate scale to fit canvas
        const float scaleX = (float)Scaled_Canvas.width / frame.width;
        const float scaleY = (float)Scaled_Canvas.height / frame.height;
        const float scale = min(scaleX, scaleY);  // Maintain aspect ratio

        const auto source = Rectangle{ 0, static_cast<float>(frame.height), static_cast<float>(frame.width), -static_cast<float>(frame.height)};
        const float x = Scaled_Canvas.x + (Scaled_Canvas.width - frame.width * scale) / 2;
        const float y = Scaled_Canvas.y + (Scaled_Canvas.height - frame.height * scale) / 2;
        const auto dest = Rectangle{ x, y, frame.width * scale,frame.height * scale};

        // draw frame texture to canvas
        DrawTexturePro(
              frame,
              source,
              dest,
              Vector2{0,0},
              0,
              WHITE);
        DrawRectangleLinesEx(dest, 4,  BLACK);
        //Debug Circle
        //DrawCircle(x, y, 10, RED);

        //visual debug
        // DrawRectangle(Canvas.Point.x, Canvas.Point.y, Canvas.width, Canvas.height, RAYWHITE);
        //DrawRectangle(BelowCanvas.Point.x, BelowCanvas.Point.y, BelowCanvas.width, BelowCanvas.height, RED);
        //DrawRectangle(BottomRight.Point.x, BottomRight.Point.y, BottomRight.width, BottomRight.height, GREEN);
    }else
    {
        //no valid record object here
        DrawRectangleRec(Scaled_Canvas, MAROON);
        DrawRectangleLinesEx(Scaled_Canvas, 4,  BLACK);
    }

    DrawRectangleLinesEx(Scaled_Sidebar, 4,  BLACK);
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
