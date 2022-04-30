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

	// if current window is clicker window, clicking should be disabled
	if (GetForegroundWindow() == FindWindowA(NULL, "name"))
		return false;

	return true;
}

int leftRando(int x) {
	if (menu::rand == 0) {
		int xe = ((prearray::defaultClicks[x] / vars::leftBoost) / 2);
		std::cout << xe << "\n";
		menu::cps = xe;
		if (xe > 2)
			return xe;
	}
	else if (menu::rand == 1) {
		int xe = ((vars::loadedClicks[x] / vars::leftBoost) / 2);
		menu::cps = xe;
		if (xe > 2)
			return xe;
	}
	else {
		int xe = ((prearray::butterflyClicks[x] / vars::leftBoost) / 2);
		menu::cps = xe;
		if (xe > 2)
			return xe;
	}
	return 100;
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

void shuffleClicks() {
	// To obtain a time-based seed
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	// Shuffling our clicks

	if (menu::rand == 0)
		std::shuffle(prearray::defaultClicks, prearray::defaultClicks + 2000, std::default_random_engine(seed));
	
	if (menu::rand == 1)
		std::shuffle(vars::loadedClicks, vars::loadedClicks + vars::amountClicks, std::default_random_engine(seed));
	
	if (menu::rand == 2)
		std::shuffle(prearray::butterflyClicks, prearray::butterflyClicks + 1469, std::default_random_engine(seed));

	menu::shuffles += 1;
	std::cout << "shuffled ";
}

bool is_cursor_visible() { //From Biscoito clicker 
	if (vars::invOnly == 1)
		return invcheck::cursor_visible() || (invcheck::inventory_opened && !invcheck::cursor_visible());

	return true;
}
// gets next rightclick cps value from selected array
int rightRando(int x) {
	if (menu::rand == 0) {
		return ((prearray::defaultClicks[x] / vars::rightBoost) / 2);
	}
	else if (menu::rand == 1) {
		int xe = ((vars::loadedClicks[x] / vars::rightBoost) / 2);
		if (xe > 5)
			return xe;
	}
	else {
		return ((prearray::butterflyClicks[x] / vars::rightBoost) / 2);
	}
	return 100;
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

void leftClickThread() {
	std::copy(std::begin(prearray::defaultClicks), std::end(prearray::defaultClicks), std::begin(vars::defaultClicksTemp));
	std::copy(std::begin(prearray::butterflyClicks), std::end(prearray::butterflyClicks), std::begin(vars::butterflyClicksTemp));
	while (true) {

		Sleep(10);

		if (menu::rand == 0) {
			vars::currentClickAmount = 2000;
			vars::averageCps = 146;
		}
		if (menu::rand == 1) {
			vars::currentClickAmount = vars::amountClicks;
		}
		if (menu::rand == 2) {
			vars::currentClickAmount = 1469;
			vars::averageCps = 89;
		}
		
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

		if (GetAsyncKeyState(vars::hideBind) & 0x8000 && vars::hideBind != 0) {
			menu::hide = !menu::hide;
			Sleep(150);
		}

		if (inJava()) {
			if (GetAsyncKeyState(vars::rightBind) & 0x8000 && vars::rightBind != 0) {
				vars::rEnabled = !vars::rEnabled;
				Sleep(150);
			}

			if (GetAsyncKeyState(vars::leftBind) & 0x8000 && vars::leftBind != 0) {
				vars::lEnabled = !vars::lEnabled;
				Sleep(150);
			}

			if (GetAsyncKeyState(vars::shiftBind) & 0x8000 && vars::shiftBind != 0) {
				int tempLeft = 0;
				int tempRight = 0;
				while (GetAsyncKeyState(vars::shiftBind) & 0x8000) {
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

	if (menu::click == 2) {
		if (menu::click == 2) {
			menu::click = 1;
			menu::time2Total = time() - captureTime2;
			int totalTime = menu::time2Total - menu::time1Total;
			menu::ms = totalTime;

			if (menu::ms < menu::msLimitMax) {
				vars::recordedClicks[menu::totalClicks] = menu::ms;
				std::cout << vars::recordedClicks[menu::totalClicks] << std::endl;
			}

		}
	}

	static auto captureTime1 = time();
	if (menu::click == 1) {
		menu::click = 2;
		menu::time1Total = time() - captureTime1;
		int totalTime = menu::time1Total - menu::time2Total;
		if (totalTime > menu::msLimitMin) {
			menu::ms = totalTime;
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
