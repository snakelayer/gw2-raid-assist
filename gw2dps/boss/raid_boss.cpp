#pragma once

#include "raid_boss.h"

int RaidBoss::DPS_DURATIONS[3] = { 10, 30, 60 };

RaidBoss::RaidBoss(GW2LIB::Agent agent) : agent(agent), secondsToDeath(0.0f) {
	dps[0] = 0.0f; dps[1] = 0.0f; dps[2] = 0.0f;
	encounterTimer = boost::timer::cpu_timer();
	encounterTimer.stop();
}

int RaidBoss::getEncounterDuration() {
	double encounterTime = encounterTimer.elapsed().wall / 1e9;
	return (int)encounterTime;
}

void RaidBoss::updateState() {
	string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
	if (encounterTimer.is_stopped() && hasTakenDamage()) {
		encounterTimer.start();
		outputHeader += str(format("// start time: %s\n") % now);
	}
	else if (!encounterTimer.is_stopped() && !hasTakenDamage()) {
		encounterTimer.stop();
	}

	if (!agent.GetCharacter().IsAlive()) {
		encounterTimer.stop();
		totalEncounterDuration = getEncounterDuration();
		outputHeader += str(format("// encounter duration: %s\n") % totalEncounterDuration);
	}
}

void RaidBoss::outputDps(stringstream &ss) {
	ss << format("Est. Seconds To Death: %d\n") % (int)secondsToDeath;
	ss << format("DPS(10s): %0.0f\n") % dps[0];
	ss << format("DPS(30s): %0.0f\n") % dps[1];
	ss << format("DPS(60s): %0.0f\n") % dps[2];
}

void RaidBoss::updateDps(boost::circular_buffer<float> &damageBuffer) {
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
