#pragma once
#include "../globals.h"
#include "clicks.h"
#include <random>
#include <ctime>

inline void decrease() {
	if (!vars::decreaseInvert) {
		if (menu::rand == 0) {
			for (int i = 0; i < vars::currentClickAmount; i++) {
				if (prearray::defaultClicks[i] >= vars::widthMax)
					prearray::defaultClicks[i] -= vars::decreaseBy;
			}
		}
		else if (menu::rand == 1) {
			for (int i = 0; i < vars::currentClickAmount; i++) {
				if (vars::loadedClicks[i] >= vars::widthMax)
					vars::loadedClicks[i] = vars::loadedClicks[i] - vars::decreaseBy;
			}
		}
		else {
			for (int i = 0; i < vars::currentClickAmount; i++) {
				if (prearray::butterflyClicks[i] >= vars::widthMax)
					prearray::butterflyClicks[i] -= vars::decreaseBy;
			}
		}
	}
	else {
		if (menu::rand == 0) {
			for (int i = 0; i < vars::currentClickAmount; i++) {
				if (prearray::defaultClicks[i] <= vars::widthMax)
					prearray::defaultClicks[i] -= vars::decreaseBy;
			}
		}
		else if (menu::rand == 1) {
			for (int i = 0; i < vars::currentClickAmount; i++) {
				if (vars::loadedClicks[i] <= vars::widthMax)
					vars::loadedClicks[i] -= vars::decreaseBy;
			}
		}
		else {
			for (int i = 0; i < vars::currentClickAmount; i++) {
				if (prearray::butterflyClicks[i] <= vars::widthMax)
					prearray::butterflyClicks[i] -= vars::decreaseBy;
			}
		}
	}
}

inline void arrayWidth() {
	srand(time(0));
	int ran = rand() % vars::widthRange + 1;
	if (menu::rand == 0)
		for (int i = 0; i < vars::currentClickAmount; i++) {

			if (prearray::defaultClicks[i] > vars::widthMax)
				prearray::defaultClicks[i] += ran;

			if (prearray::defaultClicks[i] < vars::widthMin)
				prearray::defaultClicks[i] -= ran;
		}
	if (menu::rand == 1) {
		for (int i = 0; i < vars::currentClickAmount; i++) {

			if (vars::loadedClicks[i] > vars::widthMax)
				vars::loadedClicks[i] += ran;

			if (vars::loadedClicks[i] < vars::widthMin)
				vars::loadedClicks[i] -= ran;
		}
	}

	if (menu::rand == 2) {
		for (int i = 0; i < vars::currentClickAmount; i++) {

			if (prearray::butterflyClicks[i] > vars::widthMax)
				prearray::butterflyClicks[i] += ran;

			if (prearray::butterflyClicks[i] < vars::widthMin)
				prearray::butterflyClicks[i] -= ran;
		}
	}
}