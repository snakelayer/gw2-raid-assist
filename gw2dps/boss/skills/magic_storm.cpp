#include "magic_storm.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

MagicStorm::MagicStorm() : state(MS::PENDING) {
	timer.stop();
}

MS::State MagicStorm::getState() {
	if ((state == MS::RECHARGING) && !timer.is_stopped() && (getCooldown() <= 0.0f)) {
		timer.stop();
		state = MS::READY;
	}
	return state;
}

void MagicStorm::setState(MS::State state) {
	this->state = state;
	if (state == MS::RECHARGING) {
		timer.start();
	}
	else if (!timer.is_stopped()) {
		timer.stop();
	}
}

void MagicStorm::drawStatusMeter(float x, float y, float breakbar) {
	if (getState() == MS::READY) {
		DrawRectFilled(x - meterWidth / 2, y, meterWidth, meterHeight, AssistDrawer::BREAKBAR_FULL);
		AssistDrawer::get().drawFont(x - 20.0f, y + AssistDrawer::PADY, AssistDrawer::WHITE, "READY");
	}
	else if (getState() == MS::ACTIVE) {
		float breakbarMeterLength = meterWidth * breakbar;

		DrawRectFilled(x - meterWidth / 2, y, breakbarMeterLength, meterHeight, AssistDrawer::BREAKBAR_DEPLETING);
		AssistDrawer::get().drawFont(x - 10.0f, y + AssistDrawer::PADY, AssistDrawer::WHITE, str(format("%d") % int(breakbar * 100)));
	}
	else if (getState() == MS::RECHARGING) {

		float remainingCooldown = getCooldown();
		string cooldownStr;
		if (remainingCooldown < 5.0f) {
			cooldownStr = str(format("%.1f") % remainingCooldown);
		}
		else {
			cooldownStr = str(format("%d") % int(remainingCooldown));
		}

		float rechargeMeterWidth = meterWidth * (1.0f - (remainingCooldown / COOLDOWN));
		DrawRectFilled(x - meterWidth / 2, y, rechargeMeterWidth, meterHeight, AssistDrawer::BREAKBAR_RECHARGING);
		AssistDrawer::get().drawFont(x - 10.0f, y + AssistDrawer::PADY, AssistDrawer::WHITE, cooldownStr);
	}

	DrawRect(x - meterWidth / 2, y, meterWidth, meterHeight, AssistDrawer::BORDER_COLOR);
}

float MagicStorm::getCooldown() {
	float secondsElapsed = float(timer.elapsed().wall / 1e9);
	return COOLDOWN - secondsElapsed;
}
