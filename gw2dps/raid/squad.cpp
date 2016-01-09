#pragma once

#include "../hacklib/include/hacklib/Logging.h"

#include "squad.h"

Squad::Squad(string logFile)
	: resetStateAtNextRespawn(false), outputMask(0), raidState(RAID::ACTIVE), logFile(logFile) {
	debugStr = "";

	addPlayer(GW2LIB::GetOwnCharacter());
	GW2LIB::Character character;
	while (character.BeNext()) {
		if (members.size() == MAX_SQUAD_SIZE) {
			break;
		}

		addPlayer(character);
	}
}

Squad::~Squad() {
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

void Squad::updateState() {
	CharacterMap characterMap = getCharacterMap();
	updateRaidState(characterMap);

	for (auto &member : members) {
		CharacterMap::iterator it = characterMap.find(member.first);
		if (it != characterMap.end()) {
			member.second.updateStats(it->second);
		}
		else {
			// TODO: ???
		}
	}

	tryReset(characterMap);
}

void Squad::incrementDisplayMask() {
	++outputMask;
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

bool Squad::shouldOutputDodges() {
	return ((outputMask >> 0) % 2 == 0);
}

bool Squad::shouldOutputTotalDamageTaken() {
	return ((outputMask >> 1) % 2 == 0);
}

void Squad::outputPlayerStats(ostream &stream) {
	stream << "Player\tDodges \tTotalDamageTaken\n";
	for (auto &member : members) {
		stream << format("%-7s\t%d\t%d\n") % member.second.getName().substr(0, 6) % member.second.getDodgeCount() % member.second.getTotalDamageTaken();
	}
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
		resetStateAtNextRespawn = true;
	}
	else if (allDowned) {
		raidState = RAID::DOWNED;
	}
	else {
		raidState = RAID::ACTIVE;
	}
}

void Squad::tryReset(CharacterMap &characterMap) {
	if (resetStateAtNextRespawn && GW2LIB::GetOwnCharacter().IsAlive()) {
		resetStateAtNextRespawn = false;

		if (logFile.length() > 0) {
			writeStatsToFile();
		}

		for (auto &member : members) {
			member.second.reset();
		}
	}
}

void Squad::writeStatsToFile() {
	std::ofstream file;
	file.open(logFile, std::ofstream::out | std::ofstream::app);

	if (file.is_open()) {
		string raidBossName = (raidBoss == nullptr ? "BOSS" : raidBoss->getName());
		file << format("\n// %s\n") % raidBossName;

		file << raidBoss->getOutputHeader();

		string now = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
		file << format("// end time: %s\n") % now;

		if (raidBoss != nullptr) {
			float remainingHealth = raidBoss->getCurrentHealth();
			file << format("// remaining health: %d\n") % (int)remainingHealth;

			int encounterSeconds = raidBoss->getEncounterDuration();
			file << format("// encounter time: %d\n") % encounterSeconds;
		}

		outputPlayerStats(file);
		file.close();
	}
	else {
		logFile = strerror(errno);
	}
}
/*
 * This is unused for 2 reasons:
 *  There's an unresolved pointer issue somewhere.
 *  Even if the column toggling worked, displaying this dramatically reduces fps.
 */
/*bool Squad::writePlayerStats(stringstream &ss) {
	bool shouldOutput = false;
	string headerFormatString("%-10s");
	string rowFormatString("%-10s");
	vector<bool> outputRowIndex;
	vector<string> headerFields;

	outputRowIndex.push_back(true);
	headerFields.push_back("Name");

	if (shouldOutputDodges()) {
		shouldOutput = true;
		headerFormatString += "%-7s ";
		rowFormatString += "%-7d ";
		outputRowIndex.push_back(true);
		headerFields.push_back("Dodges");
	}
	else {
		outputRowIndex.push_back(false);
	}

	if (shouldOutputTotalDamageTaken()) {
		shouldOutput = true;
		headerFormatString += "%18s";
		rowFormatString += "%18d";
		outputRowIndex.push_back(true);
		headerFields.push_back("Total Damage Taken");
	}
	else {
		outputRowIndex.push_back(false);
	}

	headerFormatString += "\n";
	rowFormatString += "\n";

	if (shouldOutput) {
		format headerFormat(headerFormatString);

		for (int i = 0; i < outputRowIndex.size(); i++) {
			if (outputRowIndex[i]) {
				headerFormat % headerFields[i];
			}
		}
		ss << headerFormat;

		for (auto &member : members) {
			format rowFormat(rowFormatString);
			rowFormat % member.getName();

			if (shouldOutputDodges()) {
				rowFormat % member.getDodgeCount();
			}

			if (shouldOutputTotalDamageTaken()) {
				rowFormat % (int)member.getTotalDamageTaken();
			}

			ss << rowFormat;
		}
	}

	return shouldOutput;
}*/

bool Squad::hasPlayerWithName(string name) {
	for (auto &member : members) {
		if (member.second.getName().compare(name) == 0) {
			return true;
		}
	}

	return false;
}
