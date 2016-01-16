#pragma once

#include "raid_boss.h"

int RaidBoss::DPS_DURATIONS[3] = { 10, 30, 60 };

RaidBoss::RaidBoss(GW2LIB::Agent agent) : agent(agent), logFile(""), secondsToDeath(0.0f) {
	dps[0] = 0.0f; dps[1] = 0.0f; dps[2] = 0.0f;
	encounterTimer = boost::timer::cpu_timer();
	encounterTimer.stop();
}

int RaidBoss::getEncounterDuration() {
	double encounterTime = encounterTimer.elapsed().wall / 1e9;
	return (int)encounterTime;
}

void RaidBoss::updateState() {
	if (encounterTimer.is_stopped() && hasTakenDamage()) {
		encounterTimer.start();
		string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
		outputHeader += str(format("\n// start time: %s\n") % now);
	}
	else if ((!encounterTimer.is_stopped() && !hasTakenDamage()) ||
	          !agent.GetCharacter().IsAlive()) {
		encounterTimer.stop();

		writeDataToFile();
		outputHeader.clear();
		remainingHealth.clear();
	}

	if (!encounterTimer.is_stopped() && agent.GetCharacter().IsAlive()) {
		remainingHealth.push_back(getCurrentHealth());
	}
}

void RaidBoss::outputDps(stringstream &ss) {
	ss << format("Est. Seconds To Death: %d\n") % (int)secondsToDeath;
	ss << format("DPS(10s): %0.0f\n") % dps[0];
	ss << format("DPS(30s): %0.0f\n") % dps[1];
	ss << format("DPS(60s): %0.0f\n") % dps[2];
}

void RaidBoss::outputAssistHeader(stringstream &ss) {
	ss << format("Name: %s\n") % getName();
	ss << format("Dist: %.0f\n") % dist(agent.GetPos(), GetOwnAgent().GetPos());
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

float RaidBoss::dist (Vector3 p1, Vector3 p2)
{
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

void RaidBoss::writeDataToFile() {
	std::ofstream file;
	file.open(logFile, std::ofstream::out | std::ofstream::app);

	if (file.is_open()) {
		file << getOutputHeader();

		string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
		file << format("// end time: %s\n") % now;
		file << format("// remaining health: %d\n") % (int)getCurrentHealth();
		file << format("// encounter duration: %d\n") % getEncounterDuration();
		writeHealthData(file);

		file.close();
	}
	else {
		logFile = strerror(errno);
	}
}

void RaidBoss::writeHealthData(ostream &stream) {
	stream << "\n// Boss health over time:\n";
	for (auto hp : remainingHealth) {
		stream << format("%.0f\n") % hp;
	}
}
