#include "raid_boss.h"

// for bug fixing...
#include "hacklib/Logging.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

int RaidBoss::DPS_DURATIONS[3] = { 10, 30, 60 };

const float RaidBoss::BOMB_KIT_RANGE = 240.0f;

RaidBoss::RaidBoss(GW2LIB::Agent agent) : agent(agent), healthMarker(RB::HEALTH_MARKER::NONE), heavyHitDamageThreshold(-0.0f), secondsToDeath(0.0f), lastX(-1), lastY(-1) {
    dps[0] = 0.0f; dps[1] = 0.0f; dps[2] = 0.0f;

    HL_LOG_DBG("initialize raid boss, agentPtr=%p, characterPtr=%p\n", agent.m_ptr, agent.GetCharacter().m_ptr);
}

RaidBoss::~RaidBoss() {
    writeToFile();
}

void RaidBoss::updateState() {
    if (!agent.IsValid()) {
        if (!tryResetBossAgent()) {
            return;
        }
    }

    if (!agent.GetCharacter().IsValid()) {
        HL_LOG_DBG("no character, skip update state, agentId=%d, agentPtr=%p, characterPtr=%p\n", agent.GetAgentId(), agent.m_ptr, agent.GetCharacter().m_ptr);
        return;
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
    drawHealthTicks();

    if (GetLockedSelection() == agent) {
        if (GetOwnCharacter().GetProfession() == GW2::Profession::PROFESSION_ENGINEER) {
            Vector3 pos = agent.GetPos();
            DrawCircleProjected(pos, BOMB_KIT_RANGE, AssistDrawer::WHITE);
        }
    }
}

bool RaidBoss::isDead() {
    Character character = agent.GetCharacter();
    if (!character.IsValid() || (character.GetMaxHealth() != getMaxHp())) {
        HL_LOG_DBG("invalid character at isDead check, agentId=%d, agentPtr=%p, characterPtr=%p\n", agent.GetAgentId(), agent.m_ptr, character.m_ptr);
        return false; // this is a (mostly correct) guess when we don't have the character object
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
    HL_LOG_DBG("tryResetBoss, agentId=%d, agentPtr=%p, characterPtr=%p\n", agent.GetAgentId(), agent.m_ptr, agent.GetCharacter().m_ptr);

    while (nextAgent.BeNext()) {
        if (nextAgent.GetCharacter().GetMaxHealth() == getMaxHp()) {
            agent.m_ptr = nextAgent.m_ptr;
            HL_LOG_DBG(" new agent: agentId=%d, agentPtr=%p\n", agent.GetAgentId(), agent.m_ptr);
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

void RaidBoss::drawAtPosition(stringstream &ss, Vector3 pos) {
    float x, y;
    if (getScreenLocation(&x, &y, pos)) {
        AssistDrawer::get().drawStreamToWindow(ss, x, y);
    }
}

void RaidBoss::outputAssistHeader(stringstream &ss) {
    ss << format("%s\n") % getName();
}

void RaidBoss::writeHeavyHitsInfo(ostream &stream) {
    stream << format("// heavy hit threshold: %d\n") % -heavyHitDamageThreshold;
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

void RaidBoss::drawHealthTicks() {
    if (GetLockedSelection() != agent) {
        return;
    }

    float x = (GetWindowWidth() / 2) - 178.0f;
    float y = 86.0f;

    if (healthMarker == RB::HEALTH_MARKER::THIRD) {
        DrawLine(x + HEALTHBAR_TICK_LENGTH/3, y, x + HEALTHBAR_TICK_LENGTH/3, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
        DrawLine(x + HEALTHBAR_TICK_LENGTH*2/3, y, x + HEALTHBAR_TICK_LENGTH*2/3, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    }
    else if (healthMarker == RB::HEALTH_MARKER::QUARTER) {
        DrawLine(x + HEALTHBAR_TICK_LENGTH/4, y, x + HEALTHBAR_TICK_LENGTH/4, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
        DrawLine(x + HEALTHBAR_TICK_LENGTH/2, y, x + HEALTHBAR_TICK_LENGTH/2, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
        DrawLine(x + HEALTHBAR_TICK_LENGTH*3/4, y, x + HEALTHBAR_TICK_LENGTH*3/4, y + HEALTHBAR_TICK_WIDTH, AssistDrawer::HEALTHBAR_TICK);
    }
}

void RaidBoss::writeToFile() {
    ofstream file;
    file.open(getOutputFileName(), ofstream::out | ofstream::app);

    if (file.is_open()) {
        file << getOutputHeader();
        string universalNow = boost::posix_time::to_simple_string(boost::posix_time::second_clock::universal_time());
        file << format("//   end time: %s\n") % universalNow;
        file << format("// remaining health: %d\n") % (int)getCurrentHealth();
        file << format("// encounter duration: %d\n") % encounterTimer.getElapsedSeconds();
        writeHeavyHitsInfo(file);
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
