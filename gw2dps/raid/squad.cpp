#include "squad.h"

using namespace boost;
using namespace std;

const std::string Squad::logFilePrefix = "gw2dpsLog-RaidAssist-";

Squad::Squad()
	: disable(false), raidState(RAID::ACTIVE) {
	debugStr = "";

	GW2LIB::Character character;
	while (character.BeNext()) {
		if (members.size() == MAX_SQUAD_SIZE) {
			break;
		}

		addPlayer(character);
	}
}

Squad::~Squad() {
	writeStatsToFile();
}

void Squad::setBoss(RaidBoss *raidBoss) {
	this->raidBoss = raidBoss;
}

void Squad::addPlayer(GW2LIB::Character character) {
	if (!character.IsPlayer()) {
		return;
	}

	SquadMember member(character);
	members.insert(SquadMemberEntry(character.GetName(), member));
}

CharacterMap Squad::getCharacterMap() {
	GW2LIB::Character character;
	CharacterMap characterMap;

	while (character.BeNext()) {
		SquadMemberMap::iterator it = members.find(character.GetName());
		if (it != members.end()) {
			characterMap.insert(CharacterEntry(character.GetName(), character));
		}
	}

	return characterMap;
}

void Squad::updateState() {
	CharacterMap characterMap = getCharacterMap();

	for (auto &member : members) {
		CharacterMap::iterator it = characterMap.find(member.first);
		if (it != characterMap.end()) {
			member.second.updateStats(it->second);
		}
	}

	if (raidBoss != nullptr) {
		raidBoss->updateSquadState(members);
	}

	updateRaidState(characterMap);
}

void Squad::updateDodgeState(CharacterSpeeds &characterSpeeds) {
	for (auto &characterSpeedEntry : characterSpeeds) {
		string characterName = characterSpeedEntry.first;
		circular_buffer<int> speedBuffer = characterSpeedEntry.second;

		float totalSpeed = 0.0f;
		for (auto &speed : speedBuffer) {
			totalSpeed += speed;
		}

		(members.find(characterName)->second).inferDodgeStateWithSpeed(totalSpeed / speedBuffer.size());
	}
}

void Squad::outputPlayerStats(ostream &stream) {
	stream << "Player\tDodgeSpeedAttainedCount\tHeavyHitsTaken\tHeavyDamageTaken\tTotalDamageTaken\n";
	for (auto &member : members) {
		stream << format("%-20s\t%d\t%d\t%d\t%d\n") % member.second.getName() % member.second.getDodgeCount() % member.second.getHeavyHitsTaken() % member.second.getHeavyDamageTaken() % member.second.getTotalDamageTaken();
	}
}

string Squad::getLogFileName() {
	string localNow = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
	return logFilePrefix + localNow + ".txt";
}

void Squad::updateRaidState(CharacterMap &characterMap) {
	bool allDead = true;
	bool allDowned = true;

	for (auto &character : characterMap) {
		if (character.second.IsDowned()) {
			allDead = false;
		}

		if (character.second.IsAlive()) {
			allDead = false;
			allDowned = false;
			break;
		}
	}

	if (allDead) {
		raidState = RAID::DEAD;
		disable = true;
	}
	else if (allDowned) {
		raidState = RAID::DOWNED;
	}
	else {
		raidState = RAID::ACTIVE;
	}
}

void Squad::writeStatsToFile() {
	std::ofstream file;
	file.open(getLogFileName(), std::ofstream::out | std::ofstream::app);

	if (file.is_open()) {
		file << "\n// start squad output\n";
		outputPlayerStats(file);
		file << format("// end squad output\n");
		file.close();
	}
}
