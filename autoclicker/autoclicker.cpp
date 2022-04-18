#include "autoclicker.h"
#include "invcheck.h"
#include <random>
#include <algorithm>
#include "../globals.h"
#include "2kpre.h"

bool inJava() {
	if (vars::mcOnly == 1) {
		if (GetForegroundWindow() == FindWindowA(("LWJGL"), NULL) && ScreenToClient(GetForegroundWindow(), &vars::pos)) {
			return true;
		}
		return false;
	}
	return true;
}

int leftRando(int x) {
	if (menu::rand == 0) {
		return ((prearray::preArr[x] / vars::leftBoost) / 2);
	}
	else if (menu::rand == 1) {
		int xe = ((vars::cps[x] / vars::leftBoost) / 2);
		if (xe > 10)
			return xe;
	}
	else {
		return ((prearray::butterArr[x] / vars::leftBoost) / 2);
	}
	return 1000;
}

void jitter() {
	while (true) {
		if (vars::jitter) {
			if (inJava() && ScreenToClient(GetForegroundWindow(), &vars::pos) && (vars::lEnabled && GetAsyncKeyState(VK_LBUTTON)) | vars::lockL) {
				mouse_event(MOUSEEVENTF_MOVE, random_int(), random_int(), 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		}
		if (leftRando(vars::cps[vars::crntClick] != 0)) {
			Sleep(leftRando(vars::cps[vars::crntClick]));
		}
		else {
			Sleep(50);
		}
	}
}
//only custom clicks shuffle atm
void shuffleArr() {
	// To obtain a time-based seed
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	// Shuffling our array
	std::shuffle(vars::cps, vars::cps + vars::amountClicks, std::default_random_engine(seed));
}

bool is_cursor_visible() { //From Biscoito clicker 
	if (vars::invOnly == 1)
	{
		if (vars::invOnly == 1)
		{
			return cursor_visible() || (inventory_opened && !cursor_visible());
		}
		return cursor_visible();
	}
	return true;
}

int rightRando(int x) {
	if (menu::rand == 0) {
		return ((prearray::preArr[x] / vars::rightBoost) / 2);
	}
	else if (menu::rand == 1) {
		int xe = ((vars::cps[x] / vars::rightBoost) / 2);
		if (xe > 10)
			return xe;
	}
	else {
		return ((prearray::butterArr[x] / vars::rightBoost) / 2);
	}
	return 1000;
}

void sendRight(int currentClick) {
	if (vars::eat == 0) {
		Sleep(rightRando(currentClick));

		PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));

		Sleep(rightRando(currentClick));

		PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
	}
		if (vars::eat == 1) {
			PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
			PostMessage(GetForegroundWindow(), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(0, 0));
			Sleep(rightRando(currentClick) * 4);

			if (!(GetAsyncKeyState(VK_RBUTTON))) {
				PostMessage(GetForegroundWindow(), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(0, 0));
			}
	}
}

void sendLeft(int currentClick) {
	if (leftRando(1) != 0) {

		if (vars::breakBlock == 1) {

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
				std::cout << "second click\n";
			}
		}
		
		if (vars::breakBlock == 0) {
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
	int currentLeftClick = 0;
	while (true) {
		if (vars::lEnabled == 1 && is_cursor_visible() && inJava()) {

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
				currentLeftClick = currentLeftClick + 1;
				sendLeft(currentLeftClick);
				std::cout << leftRando(currentLeftClick) << "\n";
				//vars::sessionClicks += 1;
			}
		}

		if (vars::lockL && vars::lEnabled != 1 && is_cursor_visible() && inJava()) {
			currentLeftClick = currentLeftClick + 1;
			sendLeft(currentLeftClick);
			std::cout << leftRando(currentLeftClick) << "\n";
			//vars::sessionClicks += 1;
		}
		if (menu::rand == 1) {
			if (vars::cps[currentLeftClick] == 0) {
				if (currentLeftClick != 0) {
					currentLeftClick = 0;
					shuffleArr();
				}
			}
		}

		vars::crntClick = currentLeftClick;

		Sleep(10);
	}
}

void rightClickThread() {
	int currentRightClick = { 0 };
	while (true) {
		if (vars::rEnabled == 1 && is_cursor_visible() && inJava()) {
			if (!(GetAsyncKeyState(VK_RBUTTON))) {
				if (currentRightClick != 0) {
					int rnd = rand() % currentRightClick + 1;
					currentRightClick = rnd;
					std::cout << rnd << "\n";
				}
			}
			if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
				currentRightClick = currentRightClick + 1;
				sendRight(currentRightClick);
				//vars::sessionClicks += 1;
			}
		}

		if (vars::cps[currentRightClick] == 0) {
			if (currentRightClick != 0) {
				int rnd = rand() % currentRightClick + 1;
				currentRightClick = rnd;
				std::cout << rnd << std::endl;
			}
		}

		Sleep(10);
	}
}
void bindThreads() {
	while (true) {

		if (GetAsyncKeyState(vars::hideBind) & 0x8000 && vars::hideBind != 0) {
			menu::hide = !menu::hide;
			Sleep(50);
		}

		if (inJava()) {
			if (GetAsyncKeyState(vars::rightBind) & 0x8000 && vars::rightBind != 0) {
				vars::rEnabled = !vars::rEnabled;
				Sleep(50);
			}

			if (GetAsyncKeyState(vars::leftBind) & 0x8000 && vars::leftBind != 0) {
				vars::lEnabled = !vars::lEnabled;
				Sleep(50);
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

int k = 0; //Flag
void recordGame() {
	while (true) {
		if (vars::recordGame) {
			if (inJava()) {
				if ((GetAsyncKeyState(VK_LBUTTON) & 1)) {
						calcIngame();
				}
			}
	}
		Sleep(4);
	}
}

void calcIngame() {

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
			//menu::cps = 1000 / totalTime;

			if (menu::ms < menu::msLimit) {
				vars::msArr[menu::totalClicks] = menu::ms;
				std::cout << vars::msArr[menu::totalClicks] << std::endl;
			}

		}
	}

	static auto captureTime1 = time();
	if (menu::click == 1) {
		menu::click = 2;
		menu::time1Total = time() - captureTime1;
		int totalTime = menu::time1Total - menu::time2Total;
		if (totalTime > 20) {
			std::cout << "MS: " << totalTime << "  |   CPS: " << 1000 / totalTime << "\n";
			menu::ms = totalTime;
			menu::cps = 1000 / totalTime;
		}
	}
	k = 0;
}

void clickThreads() {
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)leftClickThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)rightClickThread, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)bindThreads, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)jitter, 0, 0, 0);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)recordGame, 0, 0, 0);
} 
