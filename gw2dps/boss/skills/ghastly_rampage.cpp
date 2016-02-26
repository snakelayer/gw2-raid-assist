#include "ghastly_rampage.h"

using namespace boost;
using namespace boost::assign;
using namespace GW2LIB;
using namespace std;

map<GR::State, float> GhastlyRampage::MILLISECONDS_UNTIL_HIT_MAP = map_list_of
(GR::State::INITIAL, 5000.0f)
(GR::State::AFTER_FIRST_HIT, 4000.0f)
(GR::State::AFTER_SECOND_HIT, 4000.0f)
(GR::State::AFTER_THIRD_HIT, 4000.0f)
(GR::State::AFTER_FOURTH_HIT, 4000.0f)
(GR::State::AFTER_FIFTH_HIT, 3000.0f);

map<GR::State, GR::State> GhastlyRampage::NEXT_STATE_MAP = map_list_of
(GR::State::INITIAL, GR::State::AFTER_FIRST_HIT)
(GR::State::AFTER_FIRST_HIT, GR::State::AFTER_SECOND_HIT)
(GR::State::AFTER_SECOND_HIT, GR::State::AFTER_THIRD_HIT)
(GR::State::AFTER_THIRD_HIT, GR::State::AFTER_FOURTH_HIT)
(GR::State::AFTER_FOURTH_HIT, GR::State::AFTER_FIFTH_HIT)
(GR::State::AFTER_FIFTH_HIT, GR::State::PENDING);

GhastlyRampage::GhastlyRampage() : state(GR::State::PENDING) {
    timer.stop();
}

void GhastlyRampage::update(GW2::BreakbarState breakbarState, float breakbar) {
    if (breakbarState == GW2::BreakbarState::BREAKBAR_STATE_IMMUNE) {
        turnOff();
        return;
    }

    if (breakbarState == GW2::BreakbarState::BREAKBAR_STATE_READY) {
        if (state == GR::State::PENDING && breakbar == 1.0f) {
            turnOn();
            return;
        }

        float millisecondsUntilHit = MILLISECONDS_UNTIL_HIT_MAP[state];
        if (getElapsedMs() >= millisecondsUntilHit) {
            state = NEXT_STATE_MAP[state];
            if (state == GR::State::PENDING) {
                turnOff();
            }
            else {
                timer.start();
            }
        }
    }
}

void GhastlyRampage::draw(float x, float y) {
    float nowUntilHit = MILLISECONDS_UNTIL_HIT_MAP[state] - getElapsedMs();
    float percent = nowUntilHit / MILLISECONDS_UNTIL_HIT_MAP[state];
    string nowUntilHitStr = str(format("%.1f") % (nowUntilHit / 1000));

    meter.drawAtPercent(x, y, AssistDrawer::BLACK, nowUntilHitStr, percent);
}

void GhastlyRampage::turnOff() {
    state = GR::State::PENDING;
    timer.stop();
}

void GhastlyRampage::turnOn() {
    timer.start();
    state = GR::State::INITIAL;
}

float GhastlyRampage::getElapsedMs() {
    return float(timer.elapsed().wall / 1e6);
}
