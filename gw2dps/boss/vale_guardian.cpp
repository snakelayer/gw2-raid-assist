#ifndef VALE_GUARDIAN_CPP
#define VALE_GUARDIAN_CPP

#include "vale_guardian.h"

ValeGuardian::ValeGuardian(Agent &agent) : agent(&agent), secondsToDeath(0.0f), phase(VG::Phase::FIRST)
{
	encounterTimer = boost::timer::cpu_timer();
	encounterTimer.stop();
	dps[0] = 0.0f; dps[1] = 0.0f; dps[2] = 0.0f;
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
	updateDps(damageBuffer);

	if (encounterTimer.is_stopped() && hasTakenDamage()) {
		encounterTimer.start();
	}
	else if (!encounterTimer.is_stopped() && !hasTakenDamage()) {
		encounterTimer.stop();
	}

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

void ValeGuardian::outputDps(stringstream &ss) {
	ss << format("Est. Seconds To Death: %d\n") % (int)secondsToDeath;
	ss << format("DPS(10s): %0.0f\n") % dps[0];
	ss << format("DPS(30s): %0.0f\n") % dps[1];
	ss << format("DPS(60s): %0.0f\n") % dps[2];
}

void ValeGuardian::outputAssistInfo(stringstream &ss) {
	ss << format("Name: %s\n") % getName();
	addEstTimeToSplit(ss);
	addMagicStormStatus(ss);
}

int ValeGuardian::getEncounterTime() {
	double encounterTime = encounterTimer.elapsed().wall / 1e9;
	return (int)encounterTime;
}

void ValeGuardian::outputDebug(stringstream &ss) {
	ss << format("boss agentId: %d\n") % agent->GetAgentId();
	ss << format("boss phase: %d\n") % phase;
	ss << format("boss encounter time: %d\n") % getEncounterTime();
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

void ValeGuardian::updateDps(boost::circular_buffer<float> &damageBuffer) {
	for (size_t i = 0; i < 3; ++i) {
		int seconds = DPS_DURATIONS[i];
		size_t samples = seconds * 4;

		if (samples > damageBuffer.size())
			samples = damageBuffer.size();

		float sum = 0.0;
		for (size_t j = 0; j < samples; j++)
			sum += damageBuffer[j];

		dps[i] = sum / seconds;
	}

	secondsToDeath = (dps[1] == 0.0f ? 0 : getCurrentHealth() / dps[1]);
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