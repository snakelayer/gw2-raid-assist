#include "raid_boss_factory.h"
#include "vale_guardian.h"
#include "gorseval.h"
#include "sabetha.h"
#include "unknown_boss.h"

RaidBossFactory::RaidBossFactory() {
	addBossEntry(&ValeGuardian::matchesTarget, &ValeGuardian::instance);
	addBossEntry(&Gorseval::matchesTarget, &Gorseval::instance);
	addBossEntry(&Sabetha::matchesTarget, &Sabetha::instance);
}

void RaidBossFactory::addBossEntry(raidBossMatcher matcher, raidBossSupplier supplier) {
	matcherMap[matcher] = supplier;
}

RaidBoss* RaidBossFactory::getNextBoss() {
	GW2LIB::Agent agent;

	/*agent = GetLockedSelection();
	if (agent.IsValid() && agent.GetCharacter().IsMonster()) {
		return Sabetha::instance(agent);
	}*/

	while (agent.BeNext()) {
		for (raidBossMatcherMap::iterator it = matcherMap.begin(); it != matcherMap.end(); ++it) {
			if (it->first(agent)) {
				return it->second(agent);
			}
		}
	}

	agent = GW2LIB::GetLockedSelection();
	if (agent.IsValid() && agent.GetCharacter().IsMonster()) {
		return new UnknownBoss(agent);
	}

	return nullptr;
}
