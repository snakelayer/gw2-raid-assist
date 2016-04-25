#include "slubling.h"

using namespace boost;
using namespace GW2LIB;
using namespace std;

const float Slubling::SLUBLING_MAX_HP = 150168.0f;

void Slubling::drawSlublingCounts(stringstream &ss) {
    Agent agent;
    int count = 0;

    while (agent.BeNext()) {
        if (isSlubling(agent)) {
            ++count;
        }
    }

    ss << format("Slublings: %d") % count;
}

bool Slubling::isSlubling(Agent &agent) {
    if (agent.IsValid()) {
        Character character = agent.GetCharacter();
        return character.IsValid() && character.GetMaxHealth() == SLUBLING_MAX_HP;
    }

    return false;
}
