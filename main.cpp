#include <fstream>
#include <thread>
#include <algorithm> 
#include <Shobjidl.h>
#include <array>
#include <functional>
#include <random>
#include <cmath>
#include <functional>

#include "destruct/destruct.h"
#include "globals.h"
#include "imgui/imation.h"
#include "implot/implot.h"
#include "fonts/font_awesome.h"
#include "fonts/font_awesome.cpp"
#include "fonts/anotherdanger.h"
#include "imgui/imgui_internal.h"
#include "autoclicker/autoclicker.h"
#include "imgui/imfilebrowser.h"
#include "Main.h"
#include "imgui/style.h"
#include "autoclicker/2kpre.h"
#include "fonts/fa_solid_900.h"

static float tab1 = 0.f;
static float tab2 = 0.f;
static float tab3 = 0.f;
static float tab4 = 0.f;

ImFont* smallfont;
ImFont* bigfont;
ImFont* bigsmallfont;

static void showOverlay(bool* p_open)
{
    ImGuiIO& io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDecoration;

    ImGui::SetNextWindowSize({ 190.f,40 });

    if (ImGui::Begin("###", p_open, window_flags))
    {
        ImGui::SetCursorPos({ 0.f,0.f });
        ImGui::BeginChild("##Text", { 75,40 });
        ImGui::PushFont(bigsmallfont);
        ImGui::SetCursorPos({ 9.f,-7.f });
        ImGui::Text("CURE");
        ImGui::PopFont();
        ImGui::EndChild();
        ImGui::PushFont(smallfont);
        std::string cps = "CPS: " + std::to_string(1000 / (menu::cps + 1) / 2);
        //std::string clicks = "POS: " + std::to_string(vars::crntLeftclick);
        //std::string shuffles = "SHUFFLES: " + std::to_string(menu::shuffles);
        std::string sessionclicks = std::to_string(vars::sessionClicks) + " clicks this session.";
        ImGui::SetCursorPos({ 95.f,5.f });
        //ImGui::Text(shuffles.c_str());
        //ImGui::SetCursorPos({ 95.f,20.f });
        //ImGui::Text(clicks.c_str());
        ImGui::SetCursorPos({ 77.5,5.f });
        ImGui::Text(cps.c_str());
        ImGui::SetCursorPos({ 77.5,20.f });
        ImGui::Text(sessionclicks.c_str());
        ImGui::PopFont();
    }
    ImGui::End();
}

static void showGraph(bool* p_open) {
    if (!menu::hide) {
        ImGui::SetNextWindowSize({ 600.f,350.f });
        ImGui::Begin("CPS GRAPH", p_open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetCursorPos({ 0.f, 10.f });

        if (ImPlot::BeginPlot(" ")) {

            ImVec2 l1 = ImPlot::PlotToPixels(ImPlotPoint(vars::crntLeftclick, 200));
            ImVec2 l2 = ImPlot::PlotToPixels(ImPlotPoint(vars::crntLeftclick,-50));

            ImVec2 r1 = ImPlot::PlotToPixels(ImPlotPoint(vars::crntRightclick, 200));
            ImVec2 r2 = ImPlot::PlotToPixels(ImPlotPoint(vars::crntRightclick, -50));

            ImPlot::PlotLine("Custom clicks", vars::cpsTemp, vars::amountClicks);
            ImPlot::PlotLine("Default clicks", prearray::defaultClicks, 2000);
            ImPlot::PlotLine("Butterfly clicks", prearray::butterflyClicks, 1469);

            ImPlot::GetPlotDrawList()->AddLine(l1, l2, IM_COL32(255, 0, 0, 255), 2);
            ImPlot::GetPlotDrawList()->AddLine(r1, r2, IM_COL32(0, 255, 0, 255), 2);

            ImPlot::EndPlot();
        }
        ImGui::SetCursorPos({ 0.f, 310.f });
        if (ImGui::Button("CLOSE", { 600.f, 40.f })) {
            *p_open = false;
            std::cout << "Graph closed\n";
        }
        ImGui::End();
    }
}

void console() {
    AllocConsole();
    ShowWindow(GetConsoleWindow(), SW_SHOW);
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
}

void saveClicks() {
    std::string name = vars::fName;
    std::string fileName = name + ".txt";

    if (menu::saveType == 0) {
        std::ofstream myfile(fileName);
        if (myfile.is_open())
        {
            std::cout << "saving " << fileName;
            for (int i = 0; i < menu::totalClicks; i++) {
                myfile << vars::recordedClicks[i] << "\n";
            }
            myfile.close();
        }
    }
    else {
        std::ofstream myfile(fileName, std::ios_base::app);
        if (myfile.is_open())
        {
            std::cout << "appending " << fileName;
            for (int i = 0; i < menu::totalClicks; i++) {
                myfile << vars::recordedClicks[i] << "\n";
            }
            myfile.close();
        }
    }
}

void loadCps(std::string filename) {
    std::fill(vars::loadedClicks, vars::loadedClicks+5000, 0);
    std::ifstream ms;

    ms.open(filename);

    int n = 0;
    while (ms >> vars::loadedClicks[n]) {
        vars::cpsTemp[n] = vars::loadedClicks[n];
        n++;
        Sleep(0.05);
    }
    vars::amountClicks = n;

    vars::averageCpsL = average(vars::loadedClicks, n);
    vars::averageCpsR = vars::averageCpsL;

    std::cout << "\n" << vars::averageCpsL;
    std::cout << "\n" << vars::averageCpsR;

    ms.close();
}

void resetClicks() {
    std::cout << menu::totalClicks;
    for (int i = 0; menu::totalClicks > i; i++) {
        vars::recordedClicks[i] = 0;
    }
    menu::totalClicks = 0;
}

// using noteffex imgui base

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    LPCSTR wndwName = "n";

    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, wndwName, NULL };
    RegisterClassEx(&wc);
    main_hwnd = CreateWindow(wc.lpszClassName, wndwName, WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(main_hwnd)) {
        CleanupDeviceD3D();
        UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ShowWindow(main_hwnd, SW_HIDE);
    SetWindowPos(main_hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    UpdateWindow(main_hwnd);

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGui::FileBrowser fileDialog;

    // (optional) set browser properties
    fileDialog.SetTitle("Select clicks file");
    fileDialog.SetTypeFilters({ ".txt", ".txt" });


    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = nullptr; //crutial for not leaving the imgui.ini file
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

    ImFontConfig icons_config;
    
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2;
    icons_config.OversampleV = 2;

    ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    ImFont* bigfont1 = io.Fonts->AddFontFromMemoryTTF(&anfont, sizeof anfont, 144.f);
    bigfont = io.Fonts->AddFontFromMemoryTTF(&anfont, sizeof anfont,58.f);
    bigsmallfont = io.Fonts->AddFontFromMemoryTTF(&anfont, sizeof anfont, 48.f);
    ImFont* normalfont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 16.f);
    ImFont* mediumfont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 14.f);
    smallfont = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 12.f);

    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 20.0f, &icons_config, icons_ranges);
    io.Fonts->AddFontDefault();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(main_hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    // start click threads

    clickThreads();

    // start reach threads

    //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)console, 0, 0, 0);

    GetDesktopResolution();

    // load imgui style

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
     
    // main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        // style options
        ImVec4 test = ImVec4(0.15f, 0.00f, 0.00f, 0.94f);
        ImVec4 icolor = ImVec4(0.15f, 0.00f, 0.00f, 0.94f);
        ImVec4 warn = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 test2 = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
        float anim = tab1 + tab2 + tab3 + tab4;
        if (menu::style == 0) {
            HLTheme();
            test = ImVec4(0.f, 0.f, 0.f, anim);
            icolor = ImVec4(0.f, 0.00f, 0.00f, 1.f);
            test2 = ImVec4(0.24f, 0.27f, 0.20f, 1.00f);
        }
        if (menu::style == 1) {
            embraceTheDarkness();
            test = ImVec4(0.00f, 0.00f, 0.00f, anim);
            icolor = ImVec4(1.f, 1.f, 1.f, 1.f);
            test2 = ImVec4(0.15f, 0.15f, 0.15f, 1.f);
        }

        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {

            if (vars::overlay)
                showOverlay(&vars::overlay);

            if (menu::graph)
                showGraph(&menu::graph);
            
            if (menu::hide == false) {

                
                ImGui::SetNextWindowPos(ImVec2(menu::screen[0] / 3.25, menu::screen[1] / 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f)); // ImGui::GetWindowSize().x doesnt work
                ImGui::SetNextWindowSize({ 450.f,280.f });

                ImGuiStyle* style = &ImGui::GetStyle();

                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.f);
                ImGui::PushFont(font);

                ImGui::Begin("Cure", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoDecoration | ImGuiViewportFlags_NoTaskBarIcon);

                ImGui::SetCursorPos({ 425,5 });
                ImGui::PushFont(smallfont);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
                if (ImGui::Button("_", { 20,20 })) {
                    ShowWindow(main_hwnd, SW_MINIMIZE);
                }
                ImGui::PopStyleVar();
                ImGui::PopFont();
                //SW_MINIMIZE(main_hwnd);

                ImGui::SameLine();

                ImGui::SetCursorPos({ 0.f,0.f });

                ImGui::BeginChild("child0", { 105,340.f }, true);

                ImGui::SetCursorPos({ 16.f,0.f });
                ImGui::PushStyleColor(ImGuiCol_Text, icolor);
                ImGui::PushFont(bigfont);
                ImGui::Text("Cure");
                ImGui::PopFont();
                ImGui::PopStyleColor();

                ImGui::SetCursorPos({ 10.f,50.f });
                ImGui::PushStyleColor(ImGuiCol_Text, icolor);
                ImGui::PushFont(smallfont);
                ImGui::Text("PUBLIC EDITION");
                ImGui::PopFont();
                ImGui::PopStyleColor();
                ImGui::PushFont(smallfont);

                //Clicker Button
                ImGui::SetCursorPos({ 0.f,80.f });
                if (menu::tab == 1) ImGui::PushStyleColor(ImGuiCol_Button, style->Colors[ImGuiCol_ButtonActive]);
                if (ImGui::Button(" ", { 105.f,50.f })) { menu::tab = 1; } //Clicker
                if (menu::tab == 1 && ImGui::GetCurrentContext()->ColorStack.Size > 0) ImGui::PopStyleColor();

                ImGui::PushFont(normalfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1, 1.45f }));
                ImGui::SetCursorPos({ 30.f, 90.f });
                ImGui::Text("Clicker");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushFont(smallfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1,0.6f }));
                ImGui::SetCursorPos({ 30.f, 105.f });
                ImGui::Text("cps, left, right");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0,0,0, 0.6f }));
                ImGui::SetCursorPos({ 6.5f, 102.f });
                if (menu::tab == 1) {
                    ImRotateStart();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.8f));
                    ImGui::Text(ICON_FA_CROSSHAIRS "");
                    ImGui::PopStyleColor();
                    ImRotateEnd(0.0015f * ::GetTickCount64());
                }
                else {
                    ImGui::Text(ICON_FA_CROSSHAIRS "");
                }
                ImGui::PopStyleColor();

                //Configs Button
                ImGui::SetCursorPos({ 0.f,130.f });
                if (menu::tab == 2) ImGui::PushStyleColor(ImGuiCol_Button, style->Colors[ImGuiCol_ButtonActive]);
                if (ImGui::Button("  ", { 105.f,50.f })) { menu::tab = 2; }//Configs
                if (menu::tab == 2 && ImGui::GetCurrentContext()->ColorStack.Size > 0) ImGui::PopStyleColor();

                ImGui::PushFont(normalfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1, 1.45f }));
                ImGui::SetCursorPos({ 30.f, 140.f });
                ImGui::Text("Configs");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushFont(smallfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1,0.6f }));
                ImGui::SetCursorPos({ 30.f, 155.f });
                ImGui::Text("clicks, presets");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0,0,0, 0.6f }));
                ImGui::SetCursorPos({ 6.5f, 152.f });
                if (menu::tab == 2) {
                    ImRotateStart();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.8f));
                    ImGui::Text(ICON_FA_COG "");
                    ImGui::PopStyleColor();
                    ImRotateEnd(0.0015f * ::GetTickCount64());
                }
                else {
                    ImGui::Text(ICON_FA_COG "");
                }
                ImGui::PopStyleColor();
                //Misc Button
                ImGui::SetCursorPos({ 0.f,180.f });
                if (menu::tab == 3) ImGui::PushStyleColor(ImGuiCol_Button, style->Colors[ImGuiCol_ButtonActive]);
                if (ImGui::Button("   ", { 105.f,50.f })) { menu::tab = 3; } //Misc
                if (menu::tab == 3 && ImGui::GetCurrentContext()->ColorStack.Size > 0) ImGui::PopStyleColor();

                ImGui::PushFont(normalfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1, 1.45f }));
                ImGui::SetCursorPos({ 30.f, 190.f });
                ImGui::Text("Misc");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushFont(smallfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1,0.6f }));
                ImGui::SetCursorPos({ 30.f, 205.f });
                ImGui::Text("inventory, bind");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0,0,0, 0.6f }));
                ImGui::SetCursorPos({ 6.5f, 200.f });
                if (menu::tab == 3) {
                    ImRotateStart();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.8f));
                    ImGui::Text(ICON_FA_COMPASS "");
                    ImGui::PopStyleColor();
                    ImRotateEnd(0.0015f * ::GetTickCount64());
                }
                else {
                    ImGui::Text(ICON_FA_COMPASS "");
                }
                ImGui::PopStyleColor();

                //Exit Button
                ImGui::SetCursorPos({ 0.f,230.f });
                if (menu::tab == 4) ImGui::PushStyleColor(ImGuiCol_Button, style->Colors[ImGuiCol_ButtonActive]);
                if (ImGui::Button("    ", { 105.f,50.f })) { menu::tab = 4; } //Exit
                if (menu::tab == 4 && ImGui::GetCurrentContext()->ColorStack.Size > 0) ImGui::PopStyleColor();

                ImGui::PushFont(normalfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1, 1.45f }));
                ImGui::SetCursorPos({ 30.f, 240.f });
                ImGui::Text("Exit");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushFont(smallfont);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 1,1,1,0.6f }));
                ImGui::SetCursorPos({ 30.f, 255.f });
                ImGui::Text("destruct & exit");
                ImGui::PopStyleColor();
                ImGui::PopFont();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4({ 0,0,0, 0.6f }));
                ImGui::SetCursorPos({ 6.5f, 250.f });
                if (menu::tab == 4) {
                    ImRotateStart();
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 0.8f));
                    ImGui::Text(ICON_FA_TIMES "");
                    ImGui::PopStyleColor();
                    ImRotateEnd(0.0015f * ::GetTickCount64());
                }
                else {
                    ImGui::Text(ICON_FA_TIMES "");
                }
                ImGui::PopStyleColor();

                ImGui::PopFont();

                ImGui::PopStyleVar();
                ImGui::SetCursorPos({ 45.f,63.f });
                ImGui::PushItemWidth(42.000000);
                ImGui::PushFont(smallfont);
                ImGui::PushStyleColor(ImGuiCol_Text, icolor);
                ImGui::Text("2.0");
                ImGui::PopStyleColor();
                ImGui::PopFont();
                ImGui::PopItemWidth();

                ImGui::EndChild();

                if (menu::tab == 0) {

                    ImGui::PushStyleColor(ImGuiCol_Text, icolor);
                    ImGui::SetCursorPos({ 190.f, 65.f });
                    ImGui::PushFont(bigfont1);
                    ImGui::Text("Cure");
                    ImGui::PopFont();
                    ImGui::PopStyleColor();

                }

                if (menu::tab == 1) {

                    //Child 0
                    ImGui::SetCursorPos({ 350.f, 40.f });

                    tab2 = 0;
                    tab3 = 0;
                    tab4 = 0;

                    imnimation::AnimLerp(&tab1, 0.35f, 0.01f);

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, test);
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.f);
                    ImGui::BeginChild("childbtns", { 80.f , 185.f }, true, ImGuiWindowFlags_NoDecoration);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
                    ImGui::SetCursorPos({ 5,7 });
                    ImGui::PushFont(mediumfont);
                    ImGui::Checkbox("MC Only", &vars::mcOnly);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Only click inside minecraft window"); }
                    ImGui::SetCursorPos({ 5,28 });
                    ImGui::Checkbox("Inventory", &vars::invOnly);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Disable clicking in inventory"); }
                    ImGui::SetCursorPos({ 5,49 });
                    ImGui::Checkbox("Shift", &vars::shiftDis);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Temporarily disables clicking while shifting"); }
                    ImGui::SetCursorPos({ 5,70 });
                    ImGui::Checkbox("Blocks [!]", &vars::breakBlock);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Allows breaking blocks while leftclicking (Might flag)"); }
                    ImGui::SetCursorPos({ 5,91 });
                    ImGui::Checkbox("Eat [!]", &vars::eat);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Allows eating while rightclicking (Might flag)"); } 

                    ImGui::SetCursorPos({ 5,112 });
                    if (ImGui::Checkbox("Overlay", &vars::overlay)) {
                        //rpc
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Shows a small window with cps info"); }

                    ImGui::SetCursorPos({ 5,133 });
                    ImGui::Checkbox("Lock L", &vars::lockL);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Lock leftclicker (leftclicker has to be off)"); }
                    ImGui::SetCursorPos({ 5,154 });
                    ImGui::Checkbox("Jitter", &vars::jitter);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Enables jitter while leftclicking"); }

                    ImGui::PopFont();
                    ImGui::PopStyleVar();
                    ImGui::EndChild();
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor();

                    //Child 1
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, test);
                    ImGui::SetCursorPos({ 135.f, 40.f });
                    if (menu::rand == 1)
                        ImGui::OpenPopup("WARNING");
                    if (menu::rand == 1 && vars::amountClicks == 0) {
                        bool open = true;
                        if (ImGui::BeginPopupModal("WARNING", &open, ImGuiWindowFlags_NoResize))
                        {
                            ImGui::Spacing();

                            ImGui::Text("You haven't loaded any clicks yet");

                            ImGui::Spacing();
                            ImGui::Spacing();

                            if (ImGui::Button("OK", { 240,20 })) {
                                menu::tab = 2;
                                ImGui::CloseCurrentPopup();
                            }
                            ImGui::EndPopup();
                        }
                    }
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
                    ImGui::SetCursorPos({ 135.f, 40.f });
                    ImGui::BeginChild("childLeft", { 205.f,85.f }, true);
                    ImGui::SetCursorPos({ 5.f,5.f });
                    ImGui::Text("Leftclicker");
                    ImGui::SetCursorPos({ 180,5.f });
                    ImGui::Checkbox("", &vars::lEnabled);

                    ImGui::SetCursorPos({ 5.f, 30.f });
                    ImGui::PushItemWidth(130);
                    ImGui::SliderFloat("Boost", &vars::leftBoost, 0.1, menu::maxBoost);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Divides selected clicks by"); }
                    ImGui::SetCursorPos({ 5.f, 55.f });
                    ImGui::SliderFloat("Blockhit", &vars::blockhit, 0, 100);
                    ImGui::PopItemWidth();
                    ImGui::PopStyleVar();

                    ImGui::EndChild();

                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
                    ImGui::SetCursorPos({ 135.f, 140.f });
                    ImGui::BeginChild("childRight", { 205.f,85.f }, true);
                    ImGui::SetCursorPos({ 5.f,5.f });
                    ImGui::Text("Rightclicker");
                    ImGui::SetCursorPos({ 180,5.f });
                    ImGui::Checkbox("", &vars::rEnabled);

                    ImGui::SetCursorPos({ 5.f, 30.f });
                    ImGui::PushItemWidth(130);
                    ImGui::SliderFloat("Boost", &vars::rightBoost, 0, 10);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Divides selected clicks by"); }
                    ImGui::SetCursorPos({ 5.f, 60.f });
                    ImGui::PopStyleVar();

                    ImGui::EndChild();

                    if (menu::rand == 4) {

                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);

                        ImGui::BeginChild("childLeft", { 205.f,110.f }, true);
                        ImGui::SetCursorPos({ 5.f,5.f });
                        ImGui::Text("Left Enabled");
                        ImGui::SetCursorPos({ 110.f,5.f });
                        ImGui::Checkbox("", &vars::lEnabled);

                        ImGui::SetCursorPos({ 5.f, 30.f });
                        ImGui::PushItemWidth(130);
                        ImGui::SliderFloat("Max cps", &menu::maxCps, 0, menu::cpsLimit);
                        ImGui::SetCursorPos({ 5.f, 55.f });
                        ImGui::SliderFloat("Min cps", &menu::minCps, 0, menu::maxCps);
                        ImGui::SetCursorPos({ 5.f, 80.f });
                        ImGui::SliderFloat("Blockhit", &vars::blockhit, 0, 100);
                        ImGui::PopItemWidth();

                        ImGui::EndChild();

                        ImGui::SetCursorPos({ 135.f, 170.f });
                        ImGui::BeginChild("childRight", { 205.f,85.f }, true);
                        ImGui::SetCursorPos({ 5.f,5.f });
                        ImGui::Text("Right Enabled");
                        ImGui::SetCursorPos({ 110.f,5.f });
                        ImGui::Checkbox("", &vars::rEnabled);

                        ImGui::SetCursorPos({ 5.f, 30.f });
                        ImGui::PushItemWidth(130);
                        ImGui::SliderFloat("Boost", &vars::rightBoost, 0, 10);
                        ImGui::SetCursorPos({ 5.f, 60.f });
                        ImGui::PopItemWidth();


                        ImGui::EndChild();

                        ImGui::PopStyleVar();
                    }

                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
                    ImGui::PushStyleColor(ImGuiCol_Button, test);
                    ImGui::SetCursorPos({ 282.5, 237.f });
                    if (ImGui::Button("##GRAPH", { 30.f,30.f })) { //ICON_FA_CHART_LINE
                        
                        for (int i = 0; i < vars::amountClicks; i++) {
                            vars::cpsTemp[i] = vars::loadedClicks[i] / vars::leftBoost;
                        }

                        menu::graph = !menu::graph;
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Show clicks graph"); }

                    ImGui::SetCursorPos({ 244.5, 237.f });
                    if (ImGui::Button("##SHUFFLE", { 30.f,30.f })) { //ICON_FA_RECYCLE
                        shuffleClicks();
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Shuffle click locations"); }
                    ImGui::PushFont(smallfont);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.50f, 0.50f, 0.80f));
                    ImGui::SetCursorPos({ 250, 250.5 });
                    ImGui::Text(ICON_FA_RECYCLE"");
                    ImGui::SetCursorPos({ 288.5, 249.5 });
                    ImGui::Text(ICON_FA_CHART_PIE"");
                    ImGui::PopFont();

                    ImGui::PopStyleColor(3);
                    ImGui::PopStyleVar(2);

                }

                if (menu::tab == 2) {

                    tab1 = 0;
                    tab3 = 0;
                    tab4 = 0;
                    imnimation::AnimLerp(&tab2, 0.35f, 0.01f);

                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 1.f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, test);

                    ImGui::SetCursorPos({ 125.f,80.f });
                    ImGui::PushItemWidth(85);
                    ImGui::SliderInt("Limit", &menu::msLimitMax, 0, 2000);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Limits the max threshold for click-times"); }
                    ImGui::PopItemWidth();

                    ImGui::SetCursorPos({ 125.f, 110.f });
                    ImGui::PushItemWidth(130);
                    ImGui::InputText("", vars::fName, IM_ARRAYSIZE(vars::fName));
                    ImGui::PopItemWidth();

                    static std::string clk = "Click Here";

                    ImGui::SetCursorPos({ 195.f,140.f });
                    if (ImGui::Button("Reset", { 60,20 })) {
                        resetClicks();
                        clk = "Click Here";
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Reset recorded clicks array"); }

                    ImGui::SetCursorPos({ 125.f,140.f });
                    if (ImGui::Button("Save", { 60,20 })) {
                        saveClicks();
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Save recorded clicks"); }

                    ImGui::SetCursorPos({ 125.f,165.f });
                    if (ImGui::Button("Load", ImVec2(130.f, 20.f))) {
                        fileDialog.Open();
                    }
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Load custom clicks"); }

                    bool open = true;
                    if (ImGui::BeginPopupModal("Loaded", &open, ImGuiWindowFlags_NoResize))
                    {
                        std::string clicksLoaded = "Successfully loaded " + std::to_string(vars::amountClicks) + " clicks.";
                        ImGui::Text(clicksLoaded.c_str());
                        
                        if (ImGui::Button("Close"))
                            ImGui::CloseCurrentPopup();
                        ImGui::EndPopup();
                    }


                    ImGui::SetCursorPos({ 125.f,195.f });
                    ImGui::Checkbox("Add to", &menu::saveType);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Add clicks to file instead of overwriting"); }

                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);

                    ImGui::SetCursorPos({ 280.f,45.f });
                    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.75f, 0.75f, 0.75f, 0.94f));
                    ImGui::ProgressBar(menu::totalClicks * 0.0005, ImVec2(150.f,0.f));
                    ImGui::PopStyleColor();

                    ImGui::SetCursorPos({ 280.f,45.f });
                    if (ImGui::Button(clk.c_str(), { 150.f, 175.f })) {
                        if (!vars::recordGame) {
                          calcClicks();
                          clk = std::to_string(menu::totalClicks);
                        }
                    }

                    ImGui::SetCursorPos({ 280.f,20.f });
                    ImGui::Checkbox("Record In-Game", &vars::recordGame);
                    if (ImGui::IsItemHovered()) { ImGui::SetTooltip("Record clicks in-game instead"); }

                    ImGui::PopStyleVar();

                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();

                    ImGui::SetCursorPos({ 125.f,225.f });

                    ImPlot::PushStyleVar(ImPlotStyleVar_PlotPadding, ImVec2(0, 0));
                    ImGui::PushItemWidth(300);
                    if (ImPlot::BeginPlot("##", ImVec2(280,40))) {
                        vars::recordedClicks[1] = 190;
                        ImPlot::SetupAxes(NULL, NULL, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_AutoFit | ImPlotAxisFlags_NoDecorations);
                        ImPlot::PlotLine("##", vars::recordedClicks, menu::totalClicks);
                        ImPlot::EndPlot();
                    }
                    ImGui::PopItemWidth();
                    ImPlot::PopStyleVar();
                }

                if (menu::tab == 3) {

                    tab1 = 0;
                    tab2 = 0;
                    tab4 = 0;
                    imnimation::AnimLerp(&tab3, 0.35f, 0.01f);

                    ImGui::SetCursorPos({ 135.f, 38.5f });
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, test);
                    ImGui::BeginChild("childMisc", { 185,205.f }, true);

                    ImGui::SetCursorPos({ 70.f,10.f });
                    ImGui::Text("Styles");
                    ImGui::SetCursorPos({ 0.f,30.f });
                    ImGui::PushItemWidth(185);
                    ImGui::SliderInt("##", &menu::style, 0, 1, "##");

                    static const char* rands[]{ "Default 2K", "Custom", "Butterfly 1K" }; 

                    ImGui::SetCursorPos({ 40.f, 107.f });
                    ImGui::Text("Randomisation");
                    ImGui::SetCursorPos({ 0.f, 132.f });
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
                    ImGui::ListBox("", &menu::rand, rands, IM_ARRAYSIZE(rands), 3);
                    ImGui::PopStyleVar();
                    ImGui::PopItemWidth();

                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();

                    ImGui::EndChild();

                    ImGui::PushFont(smallfont);
                    

                    //works for now
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
                    ImGui::SetCursorPos({ 345.f,45.f });
                    std::string b1 = "Left (" + get_key_name_by_id(vars::leftBind) + ")";
                    if (ImGui::Button(b1.c_str(), {80,40})) {
                        keybind(vars::leftBind);
                    }
                    
                    ImGui::SetCursorPos({ 345.f,95 });
                    std::string b2 = "Right (" + get_key_name_by_id(vars::rightBind) + ")";
                    if (ImGui::Button(b2.c_str(), { 80,40 })) {
                        keybind(vars::rightBind);
                    }

                    ImGui::SetCursorPos({ 345.f,145.f });
                    std::string b3 = "Shift (" + get_key_name_by_id(vars::shiftBind) + ")";
                    if (ImGui::Button(b3.c_str(), { 80,40 })) {
                        keybind(vars::shiftBind);
                    }

                    ImGui::SetCursorPos({ 345.f,195.f });
                    std::string b4 = "Hide (" + get_key_name_by_id(vars::hideBind) + ")";
                    if (ImGui::Button(b4.c_str(), { 80,40 })) {
                        keybind(vars::hideBind);
                    }
                    ImGui::PopStyleVar();
                    ImGui::PopFont();
                }

                if (menu::tab == 4) {

                    tab1 = 0;
                    tab2 = 0;
                    tab3 = 0;

                    imnimation::AnimLerp(&tab4, 0.35f, 0.01f);
                    ImGui::SetCursorPos({ 105.f,0.f });
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, test);
                    ImGui::SetCursorPos({ 135.f, 38.5f });
                    ImGui::BeginChild("childLeft", { 285,205.f }, true);

                    ImGui::SetCursorPos({ 10.f, 20.f });
                    ImGui::Checkbox("Destruct Opt 1", &destruct::o1);
                    ImGui::SetCursorPos({ 10.f, 45.f });
                    ImGui::Checkbox("Destruct Opt 2", &destruct::o2);
                    ImGui::SetCursorPos({ 10.f, 70.f });
                    ImGui::Checkbox("Destruct Opt 3", &destruct::o3);
                    ImGui::SetCursorPos({ 10.f, 95.f });
                    ImGui::Checkbox("Destruct Opt 4", &destruct::o4);
                    
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.f);
                    ImGui::SetCursorPos({ 0,174.f });
                    if (ImGui::Button("Exit", ImVec2(285, 30))) {
                        exit(0);
                    }
                    ImGui::PopStyleVar();
                    ImGui::EndChild();
                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();
                }

                if (fileDialog.HasSelected())
                {
                    loadCps(fileDialog.GetSelected().string());
                    std::cout << "Loaded filename" << fileDialog.GetSelected().string() << std::endl;
                    ImGui::OpenPopup("Loaded");
                    fileDialog.ClearSelected();
                }

                ImGui::End();
                ImGui::PopFont();
                ImGui::SetNextWindowPos(ImVec2(menu::screen[0] / 1.75, menu::screen[1] / 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                fileDialog.Display();
            }
        }

        ImGui::EndFrame();
        Sleep(5);


        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
            ResetDevice();
        }
    }

    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(main_hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

