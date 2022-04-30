#include "autoclicker.h"
#include <random>
#include <algorithm>
#include "../globals.h"
#include "clicks.h"
#include "sounds.h"

bool inJava() {
	if (vars::mcOnly == 1) {
		if (GetForegroundWindow() == FindWindowA(("LWJGL"), NULL))
			return true;

		return false;
	}

	// if current window is clicker window, disable clicking
	if (GetForegroundWindow() == FindWindowA(NULL, "name"))
		return false;

	return true;
}

void shuffleClicks() {
	// To obtain a time-based seed
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	// Shuffling our clicks
	std::shuffle(vars::clicks, vars::clicks + vars::loadedClicksAmount, std::default_random_engine(seed));

	menu::shuffles += 1;
	std::cout << "shuffled ";
}

bool is_cursor_visible() { //From Biscoito clicker 
	if (vars::invOnly == 1)
		return invcheck::cursor_visible() || (invcheck::inventory_opened && !invcheck::cursor_visible());

	return true;
}
// gets next rightclick cps value
int rightRando(int x) {
	int xe = ((vars::clicks[x] / vars::leftBoost) / 2);
	menu::cps = xe;
	if (xe > 2)
		return xe;
	else
		return ((vars::clicks[x - 1] / vars::leftBoost) / 2);
}

// sends right click
void sendRight(int currentClick) {
	if (!vars::eat) {
		Sleep(rightRando(currentClick));

		PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));

		Sleep(rightRando(currentClick));

		PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
	}
		if (vars::eat) { // sends rdown instantly after rup allowing to eat while rightclicking
			PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
			PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
			Sleep(rightRando(currentClick) * 4);

			if (!(GetAsyncKeyState(VK_RBUTTON))) {
				PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
			}
	}
}

// gets next leftclick cps value
int leftRando(int x) {
	int xe = ((vars::clicks[x] / vars::leftBoost) / 2);
	menu::cps = xe;
	if (xe > 2)
		return xe;
	else
		return ((vars::clicks[x - 1] / vars::leftBoost) / 2);
}

// sends left click
void sendLeft(int currentClick) {
	if (leftRando(1) != 0) {

		if (vars::breakBlock) { // sends ldown instantly after lup allowint go break blocks while clicking (flags on verus, karhu)

			PostMessage(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));

			if (vars::blockhit != 0) {
				if (rand() % 100 < vars::blockhit) {
					PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));

					Sleep(10);

					PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
				}
			}

			PostMessage(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
			Sleep(leftRando(currentClick) * 2);

			if (!(GetAsyncKeyState(VK_LBUTTON))) {
				PostMessage(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
			}
		}
		
		if (!vars::breakBlock) {
			Sleep(leftRando(currentClick));
			PostMessage(GetForegroundWindow(), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));

			if (vars::blockhit != 0) {
				if (rand() % 100 < vars::blockhit) {
					PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));

					Sleep(10);

					PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
				}
			}

			Sleep(leftRando(currentClick));
			PostMessage(GetForegroundWindow(), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
		}
	}
}

void jitter() {
	while (true) {
		Sleep(1);

		if (!vars::jitter)
			continue;

		if (!inJava())
			continue;

		if (!vars::lEnabled && !vars::lockL)
			continue;

		if (!GetAsyncKeyState(VK_LBUTTON))
			continue;

		mouse_event(MOUSEEVENTF_MOVE, invcheck::random_int(), invcheck::random_int(), 0, 0);
		Sleep(leftRando(vars::crntLeftclick));
	}
}

void leftClickThread() {
	while (true) {

		Sleep(10);
		
		if (vars::lEnabled | vars::lockL)	
			if ((vars::crntLeftclick + 1) >= vars::currentClickAmount) {
				vars::crntLeftclick = 0;
				shuffleClicks();
			}

		if (!is_cursor_visible())
			continue;

		if (!inJava())
			continue;

		if (vars::lEnabled) {
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
				vars::crntLeftclick += 1;
				sendLeft(vars::crntLeftclick);
				vars::sessionClicks += 1;
			}
		}

		if (vars::lockL && !vars::lEnabled) {
			vars::crntLeftclick += 1;
			sendLeft(vars::crntLeftclick);
			vars::sessionClicks += 1;
		}
	}
}

void rightClickThread() {
	while (true) {

		Sleep(10);

		if (!is_cursor_visible())
			continue;

		if (!inJava())
			continue;

		if (!vars::rEnabled)
			continue;

			if ((vars::crntRightclick + 1) >= vars::currentClickAmount) {
				vars::crntRightclick = 0;
				shuffleClicks();
			}

			if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
				vars::crntRightclick += 1;
				sendRight(vars::crntRightclick);
				vars::sessionClicks += 1;
			}
	}
}
void bindThreads() {
	while (true) {

		if (GetAsyncKeyState(binds::hideBind) & 0x8000 && binds::hideBind != 0) {
			menu::hide = !menu::hide;
			Sleep(150);
		}

		if (inJava()) {
			if (GetAsyncKeyState(binds::rightBind) & 0x8000 && binds::rightBind != 0) {
				vars::rEnabled = !vars::rEnabled;
				Sleep(150);
			}

			if (GetAsyncKeyState(binds::leftBind) & 0x8000 && binds::leftBind != 0) {
				vars::lEnabled = !vars::lEnabled;
				Sleep(150);
			}

			if (GetAsyncKeyState(binds::shiftBind) & 0x8000 && binds::shiftBind != 0) {
				int tempLeft = 0;
				int tempRight = 0;
				while (GetAsyncKeyState(binds::shiftBind) & 0x8000) {
					if (vars::lEnabled) {
						tempRight = 1;
						vars::lEnabled = !vars::lEnabled;
					}

					if (vars::rEnabled) {
						tempLeft = 1;
						vars::rEnabled = !vars::rEnabled;
					}
					Sleep(3);
				}
				if (tempLeft == 1)
					vars::rEnabled = !vars::rEnabled;
				if (tempRight == 1)
					vars::lEnabled = !vars::lEnabled;
			}
		}
		Sleep(9);
	}
}

int time() {
	auto now = std::chrono::high_resolution_clock::now();
	auto timeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	return timeMillis;
}

void recordGame() {
	while (true) {

		Sleep(4);

		if (!vars::recordGame)
			continue;
		
		if (!inJava())
			continue;
		
		if ((GetAsyncKeyState(VK_LBUTTON) & 1))
			calcClicks();
	}
}

void calcClicks() {

	if (menu::totalClicks > 4995) {
		return;
	}
	menu::totalClicks += 1;

	std::cout << " ";
	static auto captureTime2 = time();

	if (record::click == 2) {
		if (record::click == 2) {
			record::click = 1;
			record::time2Total = time() - captureTime2;
			int totalTime = record::time2Total - record::time1Total;
			record::ms = totalTime;

			if (record::ms < menu::msLimitMax) {
				vars::recordedClicks[menu::totalClicks] = record::ms;
				std::cout << vars::recordedClicks[menu::totalClicks] << std::endl;
			}

		}
	}

	static auto captureTime1 = time();
	if (record::click == 1) {
		record::click = 2;
		record::time1Total = time() - captureTime1;
		int totalTime = record::time1Total - record::time2Total;
		if (totalTime > menu::msLimitMin) {
			record::ms = totalTime;
		}
	}
}

void clickThreads() {
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)leftClickThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)rightClickThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)bindThreads, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)jitter, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recordGame, 0, 0, 0);
} 
