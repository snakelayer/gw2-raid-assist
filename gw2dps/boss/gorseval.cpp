#include "gorseval.h"

using namespace GW2LIB;
using namespace std;

const float Gorseval::MAX_HP = 21628200;

Gorseval::Gorseval(Agent agent) : RaidBoss(agent)
{
	// TODO
}

bool Gorseval::matchesTarget(Agent &agent) {
	if ((GetCurrentMapId() == 1062) && (agent.GetCharacter().GetMaxHealth() == MAX_HP)) {
		return true;
	}

	return false;
}

void Gorseval::updateState(boost::circular_buffer<float> &damageBuffer) {
	RaidBoss::updateState();
	RaidBoss::updateDps(damageBuffer);

	// TODO: gorseval specific stuff
}

void Gorseval::updateSquadState(SquadMemberMap &members) {
	for (auto &member : members) {
		if (member.second.getLastHealthDelta() < PLAYER_HIT_DAMAGE_THRESHOLD) {
			member.second.incrementHitsTaken();
		}
	}
}

void Gorseval::outputAssistInfo(stringstream &ss) {
	RaidBoss::outputAssistHeader(ss);
	// TODO
}

void Gorseval::outputDebug(stringstream &ss) {
	ss << outputHeader;
	//outputHeader = "";
}
