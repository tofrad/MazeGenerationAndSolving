#pragma once

#include <algorithm>

#include "raylib.h"

//a singleton for every ui class to scale its elements
class LayoutManager
{
    public:
        //delete copy and assigment
        LayoutManager(const LayoutManager &) = delete;
        LayoutManager &operator=(const LayoutManager &) = delete;

        static LayoutManager* GetInstance()
        {
            static LayoutManager instance;  // Static local variable
            return &instance;
        }

        //Base size of every UI Layout
        static constexpr float BASE_WIDTH = 1920.0f;
        static constexpr float BASE_HEIGHT = 1080.0f;

        void UpdateScale()
        {
            float screenWidth = GetScreenWidth();
            float screenHeight = GetScreenHeight();
            float scaleX = screenWidth / BASE_WIDTH;
            float scaleY = screenHeight / BASE_HEIGHT;

            m_scale = std::min(scaleX, scaleY);
        }

        float GetScale() const { return m_scale; }

        //returns rescaled copy of rectangle
        Rectangle ScaleRect(Rectangle const rect) const
        {
            return{
                rect.x * m_scale,
                rect.y * m_scale,
                rect.width * m_scale,
                rect.height * m_scale
            };
        }

    private:
        //single instance
        // static LayoutManager* instance;

        LayoutManager(){};
        ~LayoutManager(){};
        float m_scale = 1.0f;
};
