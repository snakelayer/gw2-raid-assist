#include "squad.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

Squad::Squad()
    : disable(false), raidState(RAID::ACTIVE) {
    debugStr = "";

    addPlayer(GetOwnCharacter());

    Character character;
    while (character.BeNext()) {
        if (members.size() == MAX_SQUAD_SIZE) {
            break;
        }

        addPlayer(character);
    }
}

Squad::~Squad() {
    writeToFile();
}

void Squad::setBoss(RaidBoss *raidBoss) {
    this->raidBoss = raidBoss;
}

void Squad::addPlayer(Character character) {
    if (!character.IsPlayer()) {
        return;
    }

    SquadMember member(character);
    members.insert(SquadMemberEntry(character.GetName(), member));
}

CharacterMap Squad::getCharacterMap() {
    Character character;
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
    if (characterMap.empty()) {
        return; // sometimes hacklib_gw2 doesn't return any character objects for some reason...
    }

    for (auto &member : members) {
        CharacterMap::iterator it = characterMap.find(member.first);
        if (it != characterMap.end()) {
            member.second.updateStats(it->second);
        }
    }

    if (raidBoss != nullptr) {
        updateHeavyHits(raidBoss->getHeavyHitDamageThreshold());
    }

    updateRaidState(characterMap);
}

void Squad::outputPlayerStats(ostream &stream) {
    stream << "Player\tDodgeCount\tSuperspeedCount\tHeavyHitsTaken\tHeavyDamageTaken\tTotalDamageTaken\tDownedCount\n";
    for (auto &member : members) {
        SquadMember m = member.second;
        stream << format("%-20s\t%d\t%d\t%d\t%d\t%d\t%d\n") % m.getName() % m.getDodgeCount() % m.getSuperspeedCount() % m.getHeavyHitsTaken() % m.getHeavyDamageTaken() % m.getTotalDamageTaken() % m.getDownedCount();
    }
}

void Squad::updateHeavyHits(float heavyHitDamageThreshold) {
    for (auto &member : members) {
        if (member.second.getLastHealthDelta() < heavyHitDamageThreshold) {
            member.second.takeHeavyHit();
        }
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
        disable = true;
    }
    else if (allDowned) {
        raidState = RAID::DOWNED;
    }
    else {
        raidState = RAID::ACTIVE;
    }
}

void Squad::writeToFile() {
    ofstream file;
    file.open(getOutputFileName(), ofstream::out | ofstream::app);

    if (file.is_open()) {
        file << "\n// start squad output\n";
        outputPlayerStats(file);
        file << format("// end squad output\n");
        file.close();
    }
}
