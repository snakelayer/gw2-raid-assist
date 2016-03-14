#include "volatile_poison.h"

using namespace std;

VolatilePoison::VolatilePoison() : elapsedMilliseconds(0)
{
}

void VolatilePoison::updateState(int elapsedMilliseconds) {
    this->elapsedMilliseconds = elapsedMilliseconds;
}

void VolatilePoison::drawStatusMeter(float x, float y) {
    float remainingSeconds = (COOLDOWN - (elapsedMilliseconds % COOLDOWN)) / 1000.0f;
    float percent = (1.0f - remainingSeconds / (COOLDOWN/1000.0f));

    meter.drawAtPercent(x, y, AssistDrawer::SKILL_RECHARGING, remainingSeconds, percent);
}

int VolatilePoison::getElapsedMs() {
    return elapsedMilliseconds;
}
