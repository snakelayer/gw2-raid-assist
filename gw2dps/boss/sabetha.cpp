#include "sabetha.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Sabetha::MAX_HP = 34015256;
const float Sabetha::HEAVY_HIT_DAMAGE_THRESHOLD = -4000.0f;

Sabetha::Sabetha(Agent agent) : RaidBoss(agent), phase(SABETHA::Phase::FIRST)
{
}

void Sabetha::updateFlamewallState() {
	if (flamewall.getState() == FW::State::PENDING_START && !encounterTimer.isStopped()) {
		flamewall.startFirstCharge();
	}
	else if (flamewall.getState() == FW::State::PENDING_CENTER && isAtStartPosition()) {
		flamewall.startActivating();
	}
	else if (flamewall.getState() == FW::State::FIRST_CHARGE) {
		if (!isAtStartPosition()) {
			flamewall.disable();
		}
	}
	else if (flamewall.getState() == FW::State::ACTIVATING) {
		if (!flamewall.tryUpdateRotation(agent.GetRot())) {
			flamewall.tryStartAttack();
		}
	}
	else if (flamewall.getState() == FW::State::ACTIVE) {
		if (!isAtStartPosition()) {
			flamewall.disable();
		}
	}
	else if (flamewall.getState() == FW::State::RECHARGING) {
		if (!isAtStartPosition()) {
			flamewall.disable();
		}
	}
}

void Sabetha::drawFlamewallStatus() {
	Vector3 pos = getDrawAssistPosition();
	float x, y;
	bool onScreen = getScreenLocation(&x, &y, pos);
	y += flamewallDisplayOffset;

	if ((flamewall.getState() == FW::State::FIRST_CHARGE) || (flamewall.getState() == FW::State::RECHARGING)) {
		if (onScreen) {
			flamewall.drawCooldownMeter(x, y);
		}
	}
	else if (flamewall.getState() == FW::State::ACTIVATING) {
		flamewall.drawActivatingMarker(SABETHA_STARTING_POSITION);
		if (onScreen) {
			flamewall.drawCooldownMeter(x, y);
		}
	}
	else if (flamewall.getState() == FW::State::ACTIVE) {
		flamewall.drawActiveMarker(SABETHA_STARTING_POSITION);
		if (onScreen) {
			flamewall.drawCooldownMeter(x, y);
		}
	}
}

bool Sabetha::matchesTarget(Agent &agent) {
	if ((GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
		return true;
	}

	return false;
}

void Sabetha::updateState(boost::circular_buffer<float> &damageBuffer) {
	RaidBoss::updateState();
	RaidBoss::updateDps(damageBuffer);

	if (agent.m_ptr == nullptr || agent.GetCharacter().m_ptr == nullptr) {
		return;
	}

	updateFlamewallState();

	if (phase == SABETHA::Phase::FIRST && !isAtStartPosition()) { //getCurrentHealth() <= FIRST_PHASE_TRANSITION_HP) {
		phase = SABETHA::Phase::KERNAN;
		flamewall.disable();
		outputHeader += str(format("// first phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == SABETHA::Phase::KERNAN && isAtStartPosition()) {
		phase = SABETHA::Phase::SECOND;
		outputHeader += str(format("// Kernan phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if(phase == SABETHA::Phase::SECOND && !isAtStartPosition()) { //getCurrentHealth() <= SECOND_PHASE_TRANSITION_HP) {
		phase = SABETHA::Phase::KNUCKLES;
		flamewall.disable();
		outputHeader += str(format("// second phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == SABETHA::Phase::KNUCKLES && isAtStartPosition()) {
		phase = SABETHA::Phase::THIRD;
		outputHeader += str(format("// Knuckles phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == SABETHA::Phase::THIRD && !isAtStartPosition()) { //getCurrentHealth() <= THIRD_PHASE_TRANSITION_HP) {
		phase = SABETHA::Phase::KARDE;
		flamewall.disable();
		outputHeader += str(format("// third phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == SABETHA::Phase::KARDE && isAtStartPosition()) {
		phase = SABETHA::Phase::FOURTH;
		outputHeader += str(format("// Karde phase: %d\n") % encounterTimer.getSplitSeconds());
	}
}

void Sabetha::drawAssistInfo() {
	cannon.draw(encounterTimer.getElapsedSeconds());

	if (isAtStartPosition()) {
		drawFlamewallStatus();
	}

	//stringstream ss;
	//drawToWindow(ss, getDrawAssistPosition());
}

void Sabetha::outputDebug(stringstream &ss) {
	ss << format("agent valid: %s\n") % (agent.IsValid() ? "yes" : "no");
	ss << format("agentId: %d\n") % agent.GetAgentId();
	ss << format("agent ptr: %p\n") % &agent;
	ss << format("boss dead: %s\n") % (isDead() ? "yes" : "no");
	ss << format("elapsed: %d\n") % encounterTimer.getElapsedSeconds();
	ss << format("fw cooldown: %f\n") % flamewall.getCooldown();
	ss << format("fw state: %d\n") % flamewall.getState();
	ss << format("is centered: %s\n") % (isAtStartPosition() ? "yes" : "no");
	ss << outputHeader;
}