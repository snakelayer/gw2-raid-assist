#pragma once

#include <map>

#include "raid_boss.h"
#include "gw2lib.h"

class RaidBossFactory
{
	private:
		RaidBossFactory();
		RaidBossFactory(const RaidBossFactory&) {};
		RaidBossFactory& operator=(const RaidBossFactory&) { return *this; }

		typedef bool (*raidBossMatcher)(GW2LIB::Agent&);
		typedef RaidBoss* (*raidBossSupplier)(GW2LIB::Agent&);
		typedef std::map<raidBossMatcher, raidBossSupplier> raidBossMatcherMap;

		raidBossMatcherMap matcherMap;

	public:
		~RaidBossFactory() { matcherMap.clear(); }

		static RaidBossFactory* get()
		{
			static RaidBossFactory instance;
			return &instance;
		}

		void addBossEntry(raidBossMatcher matcher, raidBossSupplier supplier);
		RaidBoss* getBossForAgent(GW2LIB::Agent &agent);

};
