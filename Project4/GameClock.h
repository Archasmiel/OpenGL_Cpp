#pragma once

#include <ctime>

// clock_t is same as long
// CLOCKS_PER_SEC = clock_t { 1000 }

class GameClock {
private:
	clock_t cpf;   // clocks per frame
	clock_t last;  // last update time
	long deltaTime; // delta time in clocks
public:
	GameClock(double fps);
	bool update();
	float getDeltaTime();
	long getCPF();
	long getLast();
};
