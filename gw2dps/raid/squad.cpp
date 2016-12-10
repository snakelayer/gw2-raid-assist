#include "squad.h"

using namespace boost;
using namespace boost::chrono;
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
        if (player == GetOwnAgent().GetPlayer()) {
            continue;
        }

        addPlayer(player);
    }
}

Squad::~Squad() {
    for (auto &member : members) {
        member.second.updateUptime();
    }
    writeToFile();
}

void Squad::setBoss(RaidBoss *raidBoss) {
    this->raidBoss = raidBoss;
}

CharacterMap Squad::getCharacterMap() {
    Player player;
    CharacterMap characterMap;

    while (player.BeNext()) {
        uint32_t agentId = player.GetAgent().GetAgentId();
        SquadMemberMap::iterator it = members.find(agentId);
        if (it != members.end()) {
            characterMap.insert(CharacterEntry(agentId, player.GetCharacter()));
        }
    }

    return characterMap;
}

void Squad::updateState() {
    if (raidBoss == nullptr) {
        HL_LOG_ERR("raid boss ptr missing\n");
        return;
    }

    if (!raidBoss->isStarted()) {
        return;
    }

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

    updateHeavyHits(raidBoss->getHeavyHitDamageThreshold());

    updateRaidState(characterMap);
}

void Squad::updateDamage(Agent &src, int damage) {
    if (!src.IsValid()) {
        return;
    }

    SquadMemberMap::iterator it = members.find(src.GetAgentId());
    if (it != members.end()) {
        it->second.addDirectDamageOutput(damage);
    }
}

void Squad::drawAssistInfo() {
    if (isHealerRole(GetOwnAgent())) {
        CharacterMap characterMap = getCharacterMap();
        for (auto &characterEntry : characterMap) {
            if (characterEntry.first != GetOwnAgent().GetAgentId()) {
                members.at(characterEntry.first).tryDrawHealthMeter(characterEntry.second);
            }
        }
    }
}

void Squad::outputPlayerStats(ostream &stream) {
    double totalMillisecondDuration = (raidBoss != nullptr) ? raidBoss->getEncounterDuration().count() : 0;

    stream << "Player\t\tProfession\tDirectDamageOutput\tAverageMight\tFuryUptime\tScholarUptime\n";
    for (auto &member : members) {
        stream << format("%-19s\t\t%-12s\t%d\t%2.1f\t%1.2f\t%1.2f\n") %
            member.second.getName() %
            member.second.getProfession() %
            member.second.getDirectDamageOutput() %
            (member.second.getAverageMight()) %
            (member.second.getFuryUptime()) %
            (member.second.getScholarUptime());
    }

    stream << endl;

    stream << "Player\t\tProfession\tDodgeCount\tSuperspeedCount\tHeavyHitsTaken\tHeavyDamageTaken\tTotalDamageTaken\tDownedCount\tUptimeSeconds\tUptimePercent\n";
    for (auto &member : members) {
        int memberUptime = member.second.getTotalUptime().count();
        stream << format("%-19s\t\t%-12s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%2.1f\n") %
            member.second.getName() %
            member.second.getProfession() %
            member.second.getDodgeCount() %
            member.second.getSuperspeedCount() %
            member.second.getHeavyHitsTaken() %
            int(member.second.getHeavyDamageTaken()) %
            int(member.second.getTotalDamageTaken()) %
            member.second.getDownedCount() %
            int(memberUptime / 1e3) %
            ((totalMillisecondDuration == 0) ? 0 : (memberUptime / totalMillisecondDuration * 100));
    }
}

void Squad::addPlayer(Player &player) {
    if (!player.IsValid()) {
        return;
    }

    members.emplace(player.GetAgent().GetAgentId(), SquadMember(player));
}

bool Squad::isHealerRole(Agent &agent) {
    Player player = agent.GetPlayer();
    Character character = agent.GetCharacter();

    if ((character.IsValid() && character.GetProfession() == GW2::Profession::PROFESSION_RANGER) &&
        (player.IsValid() && player.HasEliteSpec())) {
        return true;
    }

    return false;
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
