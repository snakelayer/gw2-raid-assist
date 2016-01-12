#pragma once

#include <map>

#include "raid_boss.h"
#include "gw2lib.h"

class RaidBossFactory
{
	private:
		RaidBossFactory();
		RaidBossFactory(RaidBossFactory const&) = delete;
		void operator=(RaidBossFactory const&) = delete;

		typedef bool (*raidBossMatcher)(GW2LIB::Agent&);
		typedef RaidBoss* (*raidBossSupplier)(GW2LIB::Agent);
		typedef std::map<raidBossMatcher, raidBossSupplier> raidBossMatcherMap;

		raidBossMatcherMap matcherMap;

		void addBossEntry(raidBossMatcher matcher, raidBossSupplier supplier);

	public:
		static RaidBossFactory& get()
		{
			static RaidBossFactory instance;
			return instance;
		}

		RaidBoss* getNextBoss();
};
