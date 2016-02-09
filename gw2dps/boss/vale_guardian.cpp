#include "vale_guardian.h"
#include "skills/magic_storm.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float ValeGuardian::MAX_HP = 22021440;

ValeGuardian::ValeGuardian(Agent agent) : RaidBoss(agent), phase(VG::Phase::FIRST)
{
	if (getCurrentHealth() <= SECOND_PHASE_TRANSITION_HP) {
		phase = VG::Phase::SECOND_SPLIT;
	}
	else if (getCurrentHealth() <= FIRST_PHASE_TRANSITION_HP) {
		phase = VG::Phase::FIRST_SPLIT;
	}
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
	if (agent.IsValid() && !isSplit()) {
		RaidBoss::updateState();
		RaidBoss::updateDps(damageBuffer);
		updateSeekerState();
	}

	if (phase == VG::Phase::FIRST && getCurrentHealth() <= FIRST_PHASE_TRANSITION_HP) {
		phase = VG::Phase::FIRST_TRANSITION;
		agent.m_ptr = nullptr;
		outputHeader += str(format("// first phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == VG::Phase::FIRST_TRANSITION && findRedGuardian()) {
		phase = VG::Phase::FIRST_SPLIT;
		outputHeader += str(format("// first transition phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == VG::Phase::FIRST_SPLIT && tryResetBossAgent()) {
		phase = VG::Phase::SECOND;
		outputHeader += str(format("// first split phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == VG::Phase::SECOND && getCurrentHealth() <= SECOND_PHASE_TRANSITION_HP) {
		phase = VG::Phase::SECOND_TRANSITION;
		agent.m_ptr = nullptr;
		outputHeader += str(format("// second phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == VG::Phase::SECOND_TRANSITION && findRedGuardian()) {
		phase = VG::Phase::SECOND_SPLIT;
		outputHeader += str(format("// second transition phase: %d\n") % encounterTimer.getSplitSeconds());
	}
	else if (phase == VG::Phase::SECOND_SPLIT && tryResetBossAgent()) {
		phase = VG::Phase::THIRD;
		outputHeader += str(format("// second split phase: %d\n") % encounterTimer.getSplitSeconds());
	}
}

void ValeGuardian::updateSquadState(SquadMemberMap &members) {
	for (auto &member : members) {
		if (member.second.getLastHealthDelta() < HEAVY_HIT_DAMAGE_THRESHOLD) {
			member.second.incrementHitsTaken();
		}
	}
}

void ValeGuardian::drawAssistInfo() {
	stringstream ss;

	if (!isSplit()) {
		RaidBoss::outputAssistHeader(ss);
		addEstTimeToSplit(ss);
		addMagicStormStatus(ss);

		Vector3 pos = agent.GetPos();
		pos.z -= getBossHeight();
		drawToWindow(ss, pos);

		drawSeekerStatus();
	}
	else {
		ss.str("split phase\n");
		AssistDrawer::get().drawStreamToWindow(ss, round(GetWindowWidth() / 2), round(GetWindowHeight() / 8));
	}
}

void ValeGuardian::outputDebug(stringstream &ss) {
	ss << format("agent valid: %s\n") % (agent.IsValid() ? "yes" : "no");
	ss << format("boss agentId: %d\n") % agent.GetAgentId();
	ss << format("boss phase: %d\n") % phase;
	ss << format("boss encounter time: %d\n") % encounterTimer.getElapsedSeconds();
	ss << format("boss current hp: %f\n") % getCurrentHealth();
}

void ValeGuardian::updateSeekerState() {
	for (list<Seeker>::iterator it = seekers.begin(); it != seekers.end();) {
		if (it->getRespawnTime() <= 0) {
			it = seekers.erase(it);
		}
		else {
			++it;
		}
	}

	if (seekers.empty()) {
		Agent agent;

		while (agent.BeNext()) {
			if ((agent.GetCharacter().GetMaxHealth() == Seeker::getMaxHp()) &&
			    (Seeker::isSpawnPosition(agent))) {
				seekers.push_back(Seeker(agent));
			}
		}
	}
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
		//ss << "Magic Storm: PENDING";
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

void ValeGuardian::drawSeekerStatus() {
	stringstream ss;

	for (auto &seeker : seekers) {
		ss.str("");
		ss << seeker.getRespawnTime();
		drawToWindow(ss, seeker.getPosition());
	}
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
