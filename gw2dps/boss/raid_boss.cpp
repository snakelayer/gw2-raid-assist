#include "raid_boss.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const string RaidBoss::logFilePrefix = "gw2dpsLog-RaidAssist-";
int RaidBoss::DPS_DURATIONS[3] = { 10, 30, 60 };

const float RaidBoss::BOMB_KIT_RANGE = 240.0f;

RaidBoss::RaidBoss(GW2LIB::Agent agent) : agent(agent), secondsToDeath(0.0f), lastX(-1), lastY(-1) {
	dps[0] = 0.0f; dps[1] = 0.0f; dps[2] = 0.0f;
}

RaidBoss::~RaidBoss() {
	writeDataToFile();
}

void RaidBoss::updateState() {
	if (agent.m_ptr == nullptr) {
		if (!tryResetBossAgent()) {
			return;
		}
	}
	if (encounterTimer.isStopped() && hasTakenDamage()) {
		startEncounter();
	}

	if (!encounterTimer.isStopped() && agent.IsValid() && !isDead()) {
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

void RaidBoss::drawAssistInfo() {
	if (GetOwnCharacter().GetProfession() == GW2::Profession::PROFESSION_ENGINEER) {
		Vector3 pos = agent.GetPos();
		DrawCircleProjected(pos, BOMB_KIT_RANGE, AssistDrawer::WHITE);
	}
}

bool RaidBoss::isDead() {
	Character character = agent.GetCharacter();
	if (character.m_ptr == nullptr) {
		string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
		outputHeader += str(format("// DEBUG could not find character object for isDead check at time: %s\n") % now);
		return false;
	}

	return !character.IsAlive();
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

Vector3 RaidBoss::getDrawAssistPosition() {
	Vector3 pos = agent.GetPos();
	pos.z -= getBossHeight();
	return pos;
}

void RaidBoss::startEncounter() {
	encounterTimer.start();
	outputHeader += "\n// start raid boss output\n";
	outputHeader += str(format("// Boss: %s\n") % getName());

	string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
	outputHeader += str(format("// start time: %s\n") % now);
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

void RaidBoss::writeDataToFile() {
	std::ofstream file;
	file.open(getLogFileName(), std::ofstream::out | std::ofstream::app);

	if (file.is_open()) {
		file << getOutputHeader();
		string universalNow = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
		file << format("//   end time: %s\n") % universalNow;
		file << format("// remaining health: %d\n") % (int)getCurrentHealth();
		file << format("// encounter duration: %d\n") % encounterTimer.getElapsedSeconds();
		writeHealthData(file);

		file << format("// end raid boss output\n");
		file.close();
	}
}

void RaidBoss::writeHealthData(ostream &stream) {
	stream << "\n// https://jsfiddle.net/snakelayer/n7vbpjbf/" << endl;
	stream << "// [milliseconds, boss health]:\n";
	for (auto health : remainingHealthMap) {
		stream << format("[%s, %s],\n") % to_string(health.first) % to_string(int(health.second));
	}
}

string RaidBoss::getLogFileName() {
	string localNow = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
	return logFilePrefix + localNow + ".txt";
}
