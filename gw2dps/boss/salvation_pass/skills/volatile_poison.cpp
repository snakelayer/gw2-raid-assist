#include "volatile_poison.h"

using namespace boost;
using namespace std;

VolatilePoison::VolatilePoison() : elapsedMilliseconds(0)
{
}

void VolatilePoison::updateState(int elapsedMilliseconds) {
    this->elapsedMilliseconds = elapsedMilliseconds;
}

void VolatilePoison::drawStatusMeter(float x, float y) {
    float remainingSeconds = (COOLDOWN - (elapsedMilliseconds % COOLDOWN)) / 1000.0f;

    string cooldownStr;
    if (remainingSeconds < 5.0f) {
        cooldownStr = str(format("%.1f") % remainingSeconds);
    }
    else {
        cooldownStr = str(format("%d") % int(remainingSeconds));
    }
    float percent = (1.0f - remainingSeconds / (COOLDOWN/1000.0f));

    meter.drawAtPercent(x, y, AssistDrawer::SKILL_RECHARGING, cooldownStr, percent);
}

int VolatilePoison::getElapsedMs() {
    return elapsedMilliseconds;
}
