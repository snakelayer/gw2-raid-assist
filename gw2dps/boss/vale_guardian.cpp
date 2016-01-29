#pragma once

#include "vale_guardian.h"
#include "skills/magic_storm.h"

const float ValeGuardian::MAX_HP = 22021440;

ValeGuardian::ValeGuardian(Agent agent) : RaidBoss(agent), phase(VG::Phase::FIRST)
{
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
	if (phase == VG::Phase::FIRST || phase == VG::Phase::SECOND || phase == VG::Phase::THIRD) {
		RaidBoss::updateState();
		RaidBoss::updateDps(damageBuffer);
	}

	if (phase == VG::Phase::FIRST && getCurrentHealth() <= FIRST_PHASE_TRANSITION_HP) {
		phase = VG::Phase::FIRST_TRANSITION;
		agent.m_ptr = nullptr;
		outputHeader += str(format("// first phase: %d\n") % encounterTimer.getElapsedSecondsSinceLast());
	}
	else if (phase == VG::Phase::FIRST_TRANSITION) {
		if (findRedGuardian()) {
			phase = VG::Phase::FIRST_SPLIT;
			outputHeader += str(format("// first transition phase: %d\n") % encounterTimer.getElapsedSecondsSinceLast());
		}
	}
	else if (phase == VG::Phase::FIRST_SPLIT) {
		if (reacquireValeGuardian()) {
			phase = VG::Phase::SECOND;
			outputHeader += str(format("// first split phase: %d\n") % encounterTimer.getElapsedSecondsSinceLast());
		}
	}
	else if (phase == VG::Phase::SECOND && getCurrentHealth() <= SECOND_PHASE_TRANSITION_HP) {
		phase = VG::Phase::SECOND_TRANSITION;
		agent.m_ptr = nullptr;
		outputHeader += str(format("// second phase: %d\n") % encounterTimer.getElapsedSecondsSinceLast());
	}
	else if (phase == VG::Phase::SECOND_TRANSITION) {
		if (findRedGuardian()) {
			phase = VG::Phase::SECOND_SPLIT;
			outputHeader += str(format("// second transition phase: %d\n") % encounterTimer.getElapsedSecondsSinceLast());
		}
	}
	else if (phase == VG::Phase::SECOND_SPLIT) {
		if (reacquireValeGuardian()) {
			phase = VG::Phase::THIRD;
			outputHeader += str(format("// second split phase: %d\n") % encounterTimer.getElapsedSecondsSinceLast());
		}
	}
}

void ValeGuardian::updateSquadState(SquadMemberMap &members) {

}

void ValeGuardian::outputAssistInfo(stringstream &ss) {
	RaidBoss::outputAssistHeader(ss);
	addEstTimeToSplit(ss);
	addSeekerStatus(ss);
	addMagicStormStatus(ss);
}

void ValeGuardian::outputDebug(stringstream &ss) {
	ss << format("boss agentId: %d\n") % agent.GetAgentId();
	ss << format("boss phase: %d\n") % phase;
	ss << format("boss encounter time: %d\n") % encounterTimer.getElapsedSeconds();
	ss << format("boss current hp: %f\n") % getCurrentHealth();
}

void ValeGuardian::addEstTimeToSplit(stringstream &ss) {
	float remainingHealth = 0.0f;

	if (getCurrentHealth() >= FIRST_PHASE_TRANSITION_HP) {
		remainingHealth = getCurrentHealth() - FIRST_PHASE_TRANSITION_HP;
	}
	else if (getCurrentHealth() >= SECOND_PHASE_TRANSITION_HP) {
		remainingHealth = getCurrentHealth() - SECOND_PHASE_TRANSITION_HP;
	}

	float secondsLeft = (dps[1] == 0) ? 0 : (remainingHealth / dps[1]) - 1;
	if (secondsLeft > 0.0f) {
		ss << format("Est. Time To Split: %d\n") % (int)secondsLeft;
	}
}

void ValeGuardian::addMagicStormStatus(stringstream &ss) {
	if ((magicStorm.getState() == MS::PENDING) && (getCurrentHealth() <= FIRST_PHASE_TRANSITION_HP)) {
		magicStorm.setState(MS::READY);
	}
	else if ((magicStorm.getState() == MS::READY) && (breakbarState() == GW2::BREAKBAR_STATE_READY) && (getBreakbar() > 0.9f)) { // transition to active
		magicStorm.setState(MS::ACTIVE);
	}
	else if ((magicStorm.getState() == MS::ACTIVE) && (breakbarState() == GW2::BREAKBAR_STATE_IMMUNE)) {// && breakbar() == 0) { // transition from active to recharging
		magicStorm.setState(MS::RECHARGING);
	}

	if (magicStorm.getState() == MS::PENDING) {
		ss << "Magic Storm: PENDING";
	}
	else if (magicStorm.getState() == MS::READY) {
		ss << "Magic Storm: READY\n";
	}
	else if (magicStorm.getState() == MS::ACTIVE) {
		ss << "Magic Storm: ACTIVE\n";
	}
	else if (magicStorm.getState() == MS::RECHARGING) {
		ss << format("Magic Storm: %d\n") % magicStorm.getCooldown();
	}
	else {
		ss << "Magic Storm: ?\n";
	}
}

void ValeGuardian::addSeekerStatus(stringstream &ss) {
	if (seeker.getState() == SEEKER::INACTIVE) {
		ss << "Seeker: INACTIVE\n";
	}
	else if (seeker.getState() == SEEKER::ACTIVE) {
		ss << format("Seeker respawn: %d\n") % seeker.getRespawnTime();
	}
}

bool ValeGuardian::reacquireValeGuardian() {
	Agent nextAgent;

	while (nextAgent.BeNext()) {
		if (nextAgent.GetCharacter().GetMaxHealth() == MAX_HP) {
			agent.m_ptr = nextAgent.m_ptr;
			return true;
		}
	}

	return false;
}

bool ValeGuardian::findRedGuardian() {
	Agent agent;

	while (agent.BeNext()) {
		if (agent.GetCharacter().GetMaxHealth() == RED_GUARDIAN_MAX_HP) {
			return true;
		}
	}

	return false;
}
