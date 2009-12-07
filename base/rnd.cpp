#include "rnd.h"

#include <stdint.h>
#include <ctime>

namespace {
uint32_t g_rndSeed = 0;
}

namespace Base {

void initRNG() {
	// TODO: Implement a nicer random number generator
	g_rndSeed = (uint32_t)std::time(0);
}

unsigned int rollDice(unsigned int pips) {
	g_rndSeed = 0xDEADBF03 * (g_rndSeed + 1);
	g_rndSeed = (g_rndSeed >> 13) | (g_rndSeed << 19);
	return (g_rndSeed % pips) + 1;
}

unsigned int rollDice(unsigned int num, unsigned int pips) {
	unsigned int count = 0;
	while (num--)
		count += rollDice(pips);
	return count;
}

}

