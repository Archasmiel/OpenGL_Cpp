#include "GameClock.h"

// clock_t is same as long
// CLOCKS_PER_SEC = clock_t { 1000 }

GameClock::GameClock(double fps) {
	cpf = CLOCKS_PER_SEC / fps; // CPF = CPS / FPS
	last = std::clock();
	deltaTime = 0;
}

// returns true if update was skipped
bool GameClock::update() {
	if (std::clock() - last >= cpf) {
		deltaTime = std::clock() - last;
		last = std::clock();
		return false;
	}
	return true;
}

float GameClock::getDeltaTime() {
	return deltaTime / 1000.0f;
}

long GameClock::getCPF() {
	return cpf;
}

long GameClock::getLast() {
	return last;
}
