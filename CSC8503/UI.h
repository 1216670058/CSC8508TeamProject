#pragma once

#include "Window.h"
#include "Win32Window.h"
#include "../Plugins/imGui/imgui.h"
#include "../Plugins/imGui/imgui_impl_win32.h"
#include "../Plugins/imGui/imgui_impl_opengl3.h"

#include "Assets.h"
#include "GameWorld.h"

namespace NCL {
    namespace CSC8503 {

        enum UItype
        {

        };

        class UI
        {
        public:
            UI(GameWorld* world);
            ~UI();
            void Update(float dt);
            void DrawUI();
            void DrawLoading(float dt);
            void DrawMenu(float dt);
            void DrawChooseServer(float dt);
            void DrawPausedMenu(float dt);

            void SetLoadingStep(int step) { loadingstep = step; };
        protected:
            GameWorld* world;

            ImFont* titlefont;
            ImFont* normalfont;

            int loadingstep;

        };
    }
}