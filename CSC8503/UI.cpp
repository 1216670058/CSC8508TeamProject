#include "UI.h"

using namespace NCL;
using namespace CSC8503;
using namespace std;

UI::UI(GameWorld* world)
{
    this->world = world;
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    titlefont = io.Fonts->AddFontFromFileTTF((Assets::FONTSSDIR + "ProggyTiny.ttf").c_str(), 60);
    normalfont = io.Fonts->AddFontFromFileTTF((Assets::FONTSSDIR + "KarlaRegular.ttf").c_str(), 30);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(((Win32Code::Win32Window*)Window::GetWindow())->GetHandle());
    ImGui_ImplOpenGL3_Init("#version 330");
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

        break;
    case CSC8503::GameState::PLAYING:

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
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    //Draw background begin
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(main_viewport->Size.x, main_viewport->Size.y), ImGuiCond_Always);
    if (!ImGui::Begin("Pause Background", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }

    ImGui::PushFont(titlefont);
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 25, 25, 255));
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - ImGui::CalcTextSize("Train Bob").x * 0.5, main_viewport->GetCenter().y - 175));
    ImGui::Text("Train Bob");
    ImGui::PopFont();
    ImGui::PopStyleColor();

    ImGui::PushFont(normalfont);
    string loadingStr = "Team 3";
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - ImGui::CalcTextSize(loadingStr.c_str()).x * 0.5, main_viewport->GetCenter().y + 25));
    ImGui::Text(loadingStr.c_str());
    ImGui::PopFont();

    ImGui::PushFont(normalfont);
    ImGui::SetCursorPos(ImVec2(main_viewport->GetCenter().x - 175, main_viewport->GetCenter().y + 100));
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, IM_COL32(25, 200, 25, 255));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 255, 255, 255));
    ImGui::ProgressBar((loadingstep + 1) * 20 / 100.0f, ImVec2(350, 35));
    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::End();
}

void UI::DrawMenu(float dt)
{

}