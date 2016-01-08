#ifndef VALE_GUARDIAN_CPP
#define VALE_GUARDIAN_CPP

#include "vale_guardian.h"

ValeGuardian::ValeGuardian(Agent &agent) : RaidBoss(agent), phase(VG::Phase::FIRST)
{
	cooldownTimer = boost::timer::cpu_timer();
	cooldownTimer.stop();
	magicStorm = {
		VG::PENDING,
		30.0f,
	};
}

ValeGuardian::~ValeGuardian() {

}

bool ValeGuardian::matchesTarget(Agent &agent) {
	if ((GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
		return true;
	}

	return false;
}

void ValeGuardian::updateState(boost::circular_buffer<float> &damageBuffer) {
	RaidBoss::updateState();
	updateDps(damageBuffer);

	if (phase == VG::Phase::FIRST && getCurrentHealth() <= FIRST_PHASE_TRANSITION_HP) {
		phase = VG::Phase::SPLIT;
	}
	else if (phase == VG::Phase::SECOND && getCurrentHealth() <= SECOND_PHASE_TRANSITION_HP) {
		phase = VG::Phase::SPLIT;
	}
	else if (phase == VG::Phase::SPLIT) {
		if (refreshAgent()) {
			if (getCurrentHealth() < SECOND_PHASE_TRANSITION_HP) {
				phase = VG::Phase::THIRD;
			}
			else {
				phase = VG::Phase::SECOND;
			}
		}
	}
}

void ValeGuardian::outputAssistInfo(stringstream &ss) {
	ss << format("Name: %s\n") % getName();
	addEstTimeToSplit(ss);
	addMagicStormStatus(ss);
}

void ValeGuardian::outputDebug(stringstream &ss) {
	ss << format("boss agentId: %d\n") % agent->GetAgentId();
	ss << format("boss phase: %d\n") % phase;
	ss << format("boss encounter time: %d\n") % getEncounterDuration();
	ss << format("boss phase: %d\n") % phase;
}

void ValeGuardian::addEstTimeToSplit(stringstream &ss) {
	float remainingHealth = 0.0f;

	if (currentHealth() >= FIRST_PHASE_TRANSITION_HP) {
		remainingHealth = currentHealth() - FIRST_PHASE_TRANSITION_HP;
	}
	else if (currentHealth() >= SECOND_PHASE_TRANSITION_HP) {
		remainingHealth = currentHealth() - SECOND_PHASE_TRANSITION_HP;
	}

	float secondsLeft = (dps[1] == 0) ? 0 : (remainingHealth / dps[1]) - 1;
	if (secondsLeft > 0.0f) {
		ss << format("Est. Time To Split: %d\n") % (int)secondsLeft;
	}
}

void ValeGuardian::addMagicStormStatus(stringstream &ss) {
	if ((magicStorm.state == VG::PENDING) && (currentHealth() <= FIRST_PHASE_TRANSITION_HP)) {
		magicStorm.state = VG::READY;
	}
	else if ((magicStorm.state == VG::READY) && (breakbarState() == GW2::BREAKBAR_STATE_READY) && (breakbar() > 0.9f)) { // transition to active
		magicStorm.state = VG::ACTIVE;
	}
	else if ((magicStorm.state == VG::ACTIVE) && (breakbarState() == GW2::BREAKBAR_STATE_IMMUNE)) {// && breakbar() == 0) { // transition from active to recharging
		magicStorm.state = VG::RECHARGING;
		cooldownTimer.start();
	}
	else if ((magicStorm.state == VG::RECHARGING) && (magicStorm.cooldown <= 0.0f)) { //transition from recharging to ready
		magicStorm.state = VG::READY;
		magicStorm.cooldown = 30.0f;
		cooldownTimer.stop();
	}


	if (!cooldownTimer.is_stopped()) {
		magicStorm.cooldown = getSecondsUntilMagicStormReady();
	}

	if (magicStorm.state == VG::PENDING) {
		ss << ("Magic Storm: PENDING");
	}
	else if (magicStorm.state == VG::READY) {
		ss << ("Magic Storm: READY\n");
	}
	else if (magicStorm.state == VG::ACTIVE) {
		ss << ("Magic Storm: ACTIVE\n");
	}
	else if (magicStorm.state == VG::RECHARGING) {
		ss << format("Magic Storm: %d\n") % (int)magicStorm.cooldown;
	}
	else {
		ss << "Magic Storm: ?\n";
	}
}

double ValeGuardian::getSecondsUntilMagicStormReady() {
	double secondsElapsed = cooldownTimer.elapsed().wall / 1e9;
	return MAGIC_STORM_COOLDOWN - secondsElapsed;
}

bool ValeGuardian::refreshAgent() {
	int currentAgentId = agent->GetAgentId();
	Agent nextAgent;

	while (nextAgent.BeNext()) {
		if (nextAgent.GetCharacter().GetMaxHealth() == MAX_HP && nextAgent.GetAgentId() != currentAgentId) {
			agent->m_ptr = nextAgent.m_ptr;
			return true;
		}
	}

	return false;
}

//vector<matcherFunction> RaidBoss::matcherFunctions.push_back(&ValeGuardian::matchesTarget);

#endif