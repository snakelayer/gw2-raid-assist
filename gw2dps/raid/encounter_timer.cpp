#include "encounter_timer.h"

EncounterTimer::EncounterTimer() : lastElapsed(0.0f) {
	timer.stop();
}

void EncounterTimer::start() {
	timer.start();
}

void EncounterTimer::stop() {
	timer.stop();
}

bool EncounterTimer::isStopped() {
	return timer.is_stopped();
}

int EncounterTimer::getElapsedMilliseconds() {
	lastElapsed = timer.elapsed().wall / 1e6;
	return (int)lastElapsed;
}

int EncounterTimer::getElapsedSeconds() {
	lastElapsed = timer.elapsed().wall / 1e9;
	return (int)lastElapsed;
}

int EncounterTimer::getElapsedSecondsSinceLast() {
	double currentElapsed = timer.elapsed().wall / 1e9;
	int difference = (int)(currentElapsed - lastElapsed);

	lastElapsed = currentElapsed;
	return difference;
}