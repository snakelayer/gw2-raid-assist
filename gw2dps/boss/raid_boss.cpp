#include "raid_boss.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const string RaidBoss::logFile = "gw2dpsLog-RaidAssist.txt";
int RaidBoss::DPS_DURATIONS[3] = { 10, 30, 60 };

RaidBoss::RaidBoss(GW2LIB::Agent agent) : agent(agent), secondsToDeath(0.0f), lastX(-1), lastY(-1) {
	dps[0] = 0.0f; dps[1] = 0.0f; dps[2] = 0.0f;
}

RaidBoss::~RaidBoss() {
	writeDataToFile();
}

void RaidBoss::updateState() {
	if (agent.GetCharacter().GetMaxHealth() != getMaxHp()) {
		outputHeader += str(format("// DEBUG CharacterData ptr %p\n") % agent.GetCharacter().m_ptr);
		outputHeader += str(format("// DEBUG agentId %d switch %f %f\n") % agent.GetAgentId() % getMaxHp() % agent.GetCharacter().GetMaxHealth());
		if (tryResetBossAgent()) {
			outputHeader += str(format("// DEBUG new agentId %d\n") % agent.GetAgentId());
		}
		else {
			outputHeader += str(format("// DEBUG new agentId (same) %d\n") % agent.GetAgentId());
		}
	}

	if (encounterTimer.isStopped() && hasTakenDamage()) {
		encounterTimer.start();
		outputHeader += str(format("\n// Boss: %s\n") % getName());
		outputHeader += str(format("// DEBUG agentId: %d\n") % agent.GetAgentId());

		string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
		outputHeader += str(format("// start time: %s\n") % now);
	}

	if (!encounterTimer.isStopped() && agent.GetCharacter().IsAlive()) {
		int elapsed = encounterTimer.getElapsedMilliseconds();
		remainingHealthMap.insert(pair<int, float>(elapsed, getCurrentHealth()));
	}
}

void RaidBoss::outputDps(stringstream &ss) {
	ss << format("Est. Seconds To Death: %d\n") % (int)secondsToDeath;
	ss << format("DPS(10s): %0.0f\n") % dps[0];
	ss << format("DPS(30s): %0.0f\n") % dps[1];
	ss << format("DPS(60s): %0.0f\n") % dps[2];
}

bool RaidBoss::getScreenLocation(float *x, float *y, Vector3 pos) {
	if (!agent.IsValid()) {
		return false;
	}

	bool onScreen = WorldToScreen(pos, x, y);

	if (onScreen) {
		if (lastX > 0.0 && lastY > 0.0) {
			if ((*x < X_BUFFER) || (*x + X_BUFFER > GetWindowWidth())) {
				*x = lastX;
			}
			if ((*y < Y_BUFFER) || (*y + Y_BUFFER > GetWindowHeight())) {
				*y = lastY;
			}
		}
		lastX = *x;
		lastY = *y;
		return onScreen;
	}

	return false;
}

bool RaidBoss::tryResetBossAgent() {
	Agent nextAgent;

	while (nextAgent.BeNext()) {
		if (nextAgent.GetCharacter().GetMaxHealth() == getMaxHp()) {
			agent.m_ptr = nextAgent.m_ptr;
			return true;
		}
	}

	return false;
}

void RaidBoss::drawToWindow(stringstream &ss, Vector3 pos) {
	float x, y;
	if (getScreenLocation(&x, &y, pos)) {
		AssistDrawer::get().drawStreamToWindow(ss, x, y);
	}
	else {
		AssistDrawer::get().drawStreamToWindow(ss, round(GetWindowWidth() / 2), round(GetWindowHeight() / 8));
	}
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
		file << format("// encounter duration: %d\n") % encounterTimer.getElapsedSeconds();
		file << format("// DEBUG ending agentId: %d\n") % agent.GetAgentId();
		writeHealthData(file);

		file.close();
	}
}

void RaidBoss::writeHealthData(ostream &stream) {
	stream << "\n// Boss health over time:\n";
	for (auto health : remainingHealthMap) {
		stream << format("[%s, %s],\n") % to_string(health.first) % to_string(int(health.second));
	}
}
