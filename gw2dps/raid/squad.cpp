#include "squad.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

Squad::Squad()
    : disable(false), raidState(RAID::ACTIVE) {
    debugStr = "";

    addPlayer(GetOwnAgent().GetPlayer());

    Player player;
    while (player.BeNext()) {
        if (members.size() == MAX_SQUAD_SIZE) {
            break;
        }

        addPlayer(player);
    }
}

Squad::~Squad() {
    writeToFile();
}

void Squad::setBoss(RaidBoss *raidBoss) {
    this->raidBoss = raidBoss;
}

CharacterMap Squad::getCharacterMap() {
    Player player;
    CharacterMap characterMap;

    while (player.BeNext()) {
        SquadMemberMap::iterator it = members.find(player.GetName());
        if (it != members.end()) {
            characterMap.insert(CharacterEntry(player.GetName(), player.GetCharacter()));
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

void Squad::updateDamage(Agent src, int damage) {
    Character character = src.GetCharacter();
    if (!src.IsValid() || !character.IsValid()) {
        return;
    }

    SquadMemberMap::iterator it = members.find(character.GetName());
    if (it == members.end()) {
        return;
    }
    it->second.addDirectDamage(damage);
}

void Squad::outputPlayerStats(ostream &stream) {
    stream << "Player\tProfession\tDodgeCount\tSuperspeedCount\tHeavyHitsTaken\tHeavyDamageTaken\tTotalDamageTaken\tDirectDamage\tDownedCount\n";
    for (auto &member : members) {
        SquadMember m = member.second;
        stream << format("%-19s\t%-12s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n") % m.getName() % m.getProfession() % m.getDodgeCount() % m.getSuperspeedCount() % m.getHeavyHitsTaken() % int(m.getHeavyDamageTaken()) % int(m.getTotalDamageTaken()) % m.getDirectDamage() % m.getDownedCount();
    }
}

void Squad::addPlayer(Player &player) {
    if (!player.IsValid()) {
        return;
    }

    SquadMember member(player);
    members.insert(SquadMemberEntry(player.GetName(), member));
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
