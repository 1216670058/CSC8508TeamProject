#include "UI.h"
#include "TutorialGame.h"
#include "Win32Window.h"

using namespace NCL;
using namespace CSC8503;
using namespace std;

void ImguiProcessInput(auto* data) {
    ImGuiIO& io = ImGui::GetIO();
    RAWINPUT* rawInput = (RAWINPUT*)data;

    if (rawInput->header.dwType == RIM_TYPEMOUSE)
    {
        static int btDowns[5] = { RI_MOUSE_BUTTON_1_DOWN,
                                  RI_MOUSE_BUTTON_2_DOWN,
                                  RI_MOUSE_BUTTON_3_DOWN,
                                  RI_MOUSE_BUTTON_4_DOWN,
                                  RI_MOUSE_BUTTON_5_DOWN };

        static int btUps[5] = { RI_MOUSE_BUTTON_1_UP,
                                RI_MOUSE_BUTTON_2_UP,
                                RI_MOUSE_BUTTON_3_UP,
                                RI_MOUSE_BUTTON_4_UP,
                                RI_MOUSE_BUTTON_5_UP };

        for (int i = 0; i < 5; ++i) {
            if (rawInput->data.mouse.usButtonFlags & btDowns[i]) {
                io.MouseDown[i] = true;
            }
            else if (rawInput->data.mouse.usButtonFlags & btUps[i]) {
                io.MouseDown[i] = false;
            }
        }
    }
    else if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
    {
        USHORT key = rawInput->data.keyboard.VKey;
        bool down = !(rawInput->data.keyboard.Flags & RI_KEY_BREAK);

        if (key < 256)
            io.KeysDown[key] = down;
        if (key == VK_CONTROL)
            io.KeyCtrl = down;
        if (key == VK_SHIFT)
            io.KeyShift = down;
        if (key == VK_MENU)
            io.KeyAlt = down;
    }
}


UI::UI(GameWorld* world)
{
    this->world = world;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    Win32Code::Win32Window* win32 = (Win32Code::Win32Window*)Window::GetWindow();
    win32->SetExtraMsgFunc(ImguiProcessInput);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    titlefont = io.Fonts->AddFontFromFileTTF((Assets::FONTSSDIR + "ProggyTiny.ttf").c_str(), 60);
    normalfont = io.Fonts->AddFontFromFileTTF((Assets::FONTSSDIR + "KarlaRegular.ttf").c_str(), 30);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(win32->GetHandle());
    ImGui_ImplOpenGL3_Init("#version 330");

    loadingstep = 0;
}

UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void UI::Update(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    switch (world->GetGameState())
    {
    case CSC8503::GameState::LOADING:
        DrawLoading(dt);
        break;
    case CSC8503::GameState::MENU:
        DrawMenu(dt);
        break;
    case CSC8503::GameState::PLAYING:

        break;
    case CSC8503::GameState::PAUSED:
        DrawPausedMenu(dt);
        break;
    default:

        break;
    }

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::DrawUI()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::DrawLoading(float dt)
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y), ImGuiCond_Always);
    if (!ImGui::Begin("Paused Draw", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    ImGui::PushFont(titlefont);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 25, 25, 255));
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - ImGui::CalcTextSize("Train Bob").x * 0.5, main_viewport->GetCenter().y - main_viewport->GetCenter().y * 0.5));
    ImGui::Text("Train Bob");
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::PushFont(normalfont);
    string loadingStr = "Team 3";
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - ImGui::CalcTextSize(loadingStr.c_str()).x * 0.5, main_viewport->GetCenter().y - main_viewport->GetCenter().y * 0.15));
    ImGui::Text(loadingStr.c_str());
    ImGui::PopFont();

    ImGui::PushFont(normalfont);
    ImVec2 barSize(350, 35);
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - barSize.x / 2, main_viewport->GetCenter().y + main_viewport->GetCenter().y * 0.35));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(25, 200, 25, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 255, 255, 255));
    ImGui::ProgressBar((loadingstep + 1) * 20 / 100.0f, barSize);
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::End();
}

void UI::DrawMenu(float dt)
{
    Window::GetWindow()->ShowOSPointer(true);
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y), ImGuiCond_Always);
    if (!ImGui::Begin("Background", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    //ImGui::ShowDemoWindow();

    ImGui::PushFont(titlefont);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 25, 25, 255));
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - ImGui::CalcTextSize("Train Bob").x * 0.5, main_viewport->GetCenter().y - main_viewport->GetCenter().y * 0.5));
    ImGui::Text("Train Bob");
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::PushFont(normalfont);
    //Draw menu begin
    ImVec2 menuSize(300, 100);
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetCenter().x - menuSize.x / 2, main_viewport->GetCenter().y + menuSize.y / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
    ImGui::BeginChild("Main Menu", ImVec2(300, 175), false, ImGuiWindowFlags_NoSavedSettings);
    float contentWidth = ImGui::GetWindowContentRegionWidth();

    //set Singleplayer
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(200, 200, 200, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(125, 125, 125, 1));
    if (ImGui::Button("Single Player", ImVec2(contentWidth, 50))) {
        TutorialGame::GetGame()->InitGameWorld();
        world->SetGameState(GameState::PLAYING);
        Window::GetWindow()->ShowOSPointer(false);
    }

    //set Multiplayer
    if (ImGui::Button("Multi Player", ImVec2(contentWidth, 50))) {

    }

    //set Exit Game
    if (ImGui::Button("Exit", ImVec2(contentWidth, 50))) {
        world->SetGameState(GameState::EXIT);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::End();
}

void UI::DrawPausedMenu(float dt)
{
    Window::GetWindow()->ShowOSPointer(true);
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y), ImGuiCond_Always);
    if (!ImGui::Begin("Background", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    //ImGui::ShowDemoWindow();

    ImGui::PushFont(normalfont);
    //Draw menu begin
    ImVec2 menuSize(300, 100);
    ImGui::SetNextWindowPos(ImVec2(main_viewport->GetCenter().x - menuSize.x / 2, main_viewport->GetCenter().y - menuSize.y / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(menuSize, ImGuiCond_Always);
    ImGui::BeginChild("Paused Menu", ImVec2(300, 175), false, ImGuiWindowFlags_NoSavedSettings);
    float contentWidth = ImGui::GetWindowContentRegionWidth();

    //set Singleplayer
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(200, 200, 200, 1));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(125, 125, 125, 1));
    if (ImGui::Button("Resume Game", ImVec2(contentWidth, 50))) {
        world->SetGameState(GameState::PLAYING);
        Window::GetWindow()->ShowOSPointer(false);
    }

    //set Multiplayer
    if (ImGui::Button("Main Menu", ImVec2(contentWidth, 50))) {
        world->SetGameState(GameState::MENU);
        world->ClearAndErase();
        world->GetMainCamera().InitCam();
    }

    //set Exit Game
    if (ImGui::Button("Exit", ImVec2(contentWidth, 50))) {
        world->SetGameState(GameState::EXIT);
    }
    ImGui::PopStyleColor(3);
    ImGui::PopFont();
    ImGui::EndChild();

    ImGui::End();
}