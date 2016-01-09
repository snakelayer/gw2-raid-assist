#pragma once

#include "raid_boss_factory.h"
#include "vale_guardian.h"
#include "gorseval.h"
#include "unknown_boss.h"

RaidBossFactory::RaidBossFactory() {
	addBossEntry(&ValeGuardian::matchesTarget, &ValeGuardian::instance);
	addBossEntry(&Gorseval::matchesTarget, &Gorseval::instance);
}

void RaidBossFactory::addBossEntry(raidBossMatcher matcher, raidBossSupplier supplier) {
	matcherMap[matcher] = supplier;
}

RaidBoss* RaidBossFactory::getBossForAgent(GW2LIB::Agent &agent) {
	for (raidBossMatcherMap::iterator it = matcherMap.begin(); it != matcherMap.end(); ++it) {
		if (it->first(agent)) {
			return it->second(agent);
		}
	}

	return new UnknownBoss(agent);
}
