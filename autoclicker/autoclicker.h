#pragma once
#include <thread>
#include <random>
#include <Windows.h>
#include <iostream>
#include <fstream>

void clickThreads();
void loadCps(std::string filename);
void calcClicks();
void shuffleClicks();
void arrayWidth();

namespace invcheck { // From b1scoito's clicker https://github.com/b1scoito/clicker
	inline std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

	inline auto inventory_opened = false;
	inline static bool cursorVis = false;

	inline HHOOK h_hook;

	inline int random_buffer() {

		std::uniform_int_distribution<> dis(0.0, 2.0);

		return dis(mersenne);
	}

	inline int random_int() {
		std::uniform_int_distribution<> ger{ -random_buffer(), random_buffer() };

		std::cout << ger(mersenne) << std::endl;
		return ger(mersenne);
	}

	inline bool cursor_visible() {
		CURSORINFO ci{ sizeof(CURSORINFO) };
		if (GetCursorInfo(&ci))
		{
			const auto handle = ci.hCursor;

			if (!((handle > (HCURSOR)50000) & (handle < (HCURSOR)100000)))
				return true;
		}

		return false;
	}

	inline bool is_application_focused()
	{
		const auto hwnd = GetForegroundWindow();
		if (!hwnd)
			return false;

		DWORD dw_thread_process_id;
		GetWindowThreadProcessId(hwnd, &dw_thread_process_id);
		return (GetCurrentProcessId() == dw_thread_process_id);
	}

	static LRESULT CALLBACK keyboard_callback(int nCode, WPARAM wParam, LPARAM lParam)
	{
		static auto* k_hook = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

		if (wParam == WM_KEYDOWN && nCode == HC_ACTION && (wParam >= WM_KEYFIRST) && (wParam <= WM_KEYLAST))
		{
			if (is_application_focused())
			{
				if (k_hook->vkCode == 69 /* E */)
					inventory_opened = !inventory_opened;

				if (k_hook->vkCode == VK_ESCAPE)
					inventory_opened = false;
			}
		}

		return CallNextHookEx(h_hook, nCode, wParam, lParam);
	}
}