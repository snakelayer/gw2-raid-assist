#include "raid_boss_factory.h"
#include "spirit_vale/vale_guardian.h"
#include "spirit_vale/gorseval.h"
#include "spirit_vale/sabetha.h"
#include "salvation_pass/slothasor.h"
#include "salvation_pass/matthias.h"
#include "unknown_boss.h"

using namespace GW2LIB;

RaidBossFactory::RaidBossFactory() {
    addBossEntry(&ValeGuardian::matchesTarget, &ValeGuardian::instance);
    addBossEntry(&Gorseval::matchesTarget, &Gorseval::instance);
    addBossEntry(&Sabetha::matchesTarget, &Sabetha::instance);

    addBossEntry(&Slothasor::matchesTarget, &Slothasor::instance);
    addBossEntry(&Matthias::matchesTarget, &Matthias::instance);
}

void RaidBossFactory::addBossEntry(raidBossMatcher matcher, raidBossSupplier supplier) {
    matcherMap[matcher] = supplier;
}

RaidBoss* RaidBossFactory::getNextBoss() {
    GW2LIB::Agent agent = GetLockedSelection();
    
    /*if (agent.IsValid() && agent.GetCharacter().IsMonster()) {
        return Slothasor::instance(agent);
    }*/

    for (raidBossMatcherMap::iterator it = matcherMap.begin(); it != matcherMap.end(); ++it) {
        if (it->first(agent)) {
            return it->second(agent);
        }
    }

    if (agent.IsValid() && agent.GetCharacter().IsMonster()) {
        return new UnknownBoss(agent);
    }

    return nullptr;
}
