#pragma once
#include <iostream>
#include <windows.h>
#include <thread>
#include <unordered_map>

#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "globals.h"

HWND main_hwnd = nullptr;

LPDIRECT3DDEVICE9        g_pd3dDevice;
D3DPRESENT_PARAMETERS    g_d3dpp;
LPDIRECT3D9              g_pD3D;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

bool debug = false;

int rotation_start_index;

double average(int a[], float n)
{
	float sum = 0;
	for (int i = 0; i < n; i++) {
		sum += a[i];
	}
	return (double)sum / n;
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
	std::fill(vars::loadedClicks, vars::loadedClicks + 5000, 0);
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

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
        return false;
    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void keybind(int& bind) {
	int g = 0;
	while (g != 1) {
		for (auto i = 1; i < 256; i++) 
			if (GetAsyncKeyState(i) & 0x8000)
			{
				if (i == 27 | i == 1 | i == 2 | i == 4) {
					bind = 0;
					g = 1;
					break;
				}
				else {
					bind = i;
					g = 1;
					break;
				}
			}
	}
}

std::string get_key_name_by_id(int id)
{
	static std::unordered_map<int, std::string> key_names = {
		{ 0, "NONE" },
		{ VK_LBUTTON, "Mouse 1" },
		{ VK_RBUTTON, "Mouse 2" },
		{ VK_MBUTTON, "Mouse 3" },
		{ VK_XBUTTON1, "Mouse 4" },
		{ VK_XBUTTON2, "Mouse 5" },
		{ VK_BACK, "Back" },
		{ VK_TAB, "Tab" },
		{ VK_CLEAR, "Clear" },
		{ VK_RETURN, "Enter" },
		{ VK_SHIFT, "Shift" },
		{ VK_CONTROL, "Ctrl" },
		{ VK_MENU, "Alt" },
		{ VK_PAUSE, "Pause" },
		{ VK_CAPITAL, "Caps Lock" },
		{ VK_ESCAPE, "Escape" },
		{ VK_SPACE, "Space" },
		{ VK_PRIOR, "Page Up" },
		{ VK_NEXT, "Page Down" },
		{ VK_END, "End" },
		{ VK_HOME, "Home" },
		{ VK_LEFT, "Left Key" },
		{ VK_UP, "Up Key" },
		{ VK_RIGHT, "Right Key" },
		{ VK_DOWN, "Down Key" },
		{ VK_SELECT, "Select" },
		{ VK_PRINT, "Print Screen" },
		{ VK_INSERT, "Insert" },
		{ VK_DELETE, "Delete" },
		{ VK_HELP, "Help" },
		{ VK_SLEEP, "Sleep" },
		{ VK_MULTIPLY, "*" },
		{ VK_ADD, "+" },
		{ VK_SUBTRACT, "-" },
		{ VK_DECIMAL, "." },
		{ VK_DIVIDE, "/" },
		{ VK_NUMLOCK, "Num Lock" },
		{ VK_SCROLL, "Scroll" },
		{ VK_LSHIFT, "Left Shift" },
		{ VK_RSHIFT, "Right Shift" },
		{ VK_LCONTROL, "Left Ctrl" },
		{ VK_RCONTROL, "Right Ctrl" },
		{ VK_LMENU, "Left Alt" },
		{ VK_RMENU, "Right Alt" },
	};

	if (id >= 0x30 && id <= 0x5A)
		return std::string(1, (char)id);

	if (id >= 0x60 && id <= 0x69)
		return "Num " + std::to_string(id - 0x60);

	if (id >= 0x70 && id <= 0x87)
		return "F" + std::to_string((id - 0x70) + 1);

	return key_names[id];
}

void GetDesktopResolution()
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	menu::screen[0] = desktop.right;
	menu::screen[1] = desktop.bottom;
}


void ImRotateStart()
{
	rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

ImVec2 ImRotationCenter()
{
	ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

	const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

	return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
	float s = sin(rad), c = cos(rad);
	center = ImRotate(center, s, c) - center;

	auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
	for (int i = rotation_start_index; i < buf.Size; i++)
		buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}