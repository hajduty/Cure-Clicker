#pragma once
#include "../globals.h"
#include "clicks.h"
#include <random>
#include <ctime>

inline void decrease() {

	srand(time(0));
	int ran = rand() % modify::widthRange + 1;

	if (!modify::decreaseInvert) {
		for (int i = 0; i < vars::currentClickAmount; i++) {
			if (rand() % 100 < modify::modifyChance);
				if (vars::clicks[i] >= modify::widthMax)
					vars::clicks[i] -= modify::decreaseBy;
		}
	}
	else {
		for (int i = 0; i < vars::currentClickAmount; i++) {
			if (rand() % 100 < modify::modifyChance);
				if (vars::clicks[i] <= modify::widthMax)
					vars::clicks[i] -= modify::decreaseBy;
		}
	}
}

inline void arrayWidth() {
	srand(time(0));
	int ran = rand() % modify::widthRange + 1;

	for (int i = 0; i < vars::currentClickAmount; i++) {
		if (rand() % 100 < modify::modifyChance) {
			if (vars::clicks[i] > modify::widthMax)
				vars::clicks[i] += ran;

			if (vars::clicks[i] < modify::widthMin)
				vars::clicks[i] -= ran;
		}
	}
}