#pragma once

#include <boost/circular_buffer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"
#include "skills/cannon.h"
#include "skills/flamewall.h"

#include "../assist_drawer.h"

namespace SABETHA {
    enum Phase {
        FIRST = 0,
        KERNAN,
        SECOND,
        KNUCKLES,
        THIRD,
        KARDE,
        FOURTH
    };
}

class Sabetha : public RaidBoss
{
    private:
        static const float MAX_HP;
        const float FIRST_PHASE_TRANSITION_HP = 25497542;
        const float SECOND_PHASE_TRANSITION_HP = 16643379;
        const float THIRD_PHASE_TRANSITION_HP = 8510406;

        const float KERNAN_MAX_HP = 3440850;
        const float KERNAN_HP_AT_SABETHA_RETURN = 856528;
        const float KNUCKLES_MAX_HP = 3440850;
        const float KNUCKLES_HP_AT_SABETHA_RETURN = 876643;
        const float KARDE_MAX_HP = 4423950;
        const float KARDE_HP_AT_SABETHA_RETURN = 935633; // TODO: this is low, can be higher

        const float flamewallDisplayOffset = 20.0f;

        SABETHA::Phase phase;
        Cannon cannon;
        Flamewall flamewall;
        float startingRotation;

        const GW2LIB::Vector3 SABETHA_STARTING_POSITION = GW2LIB::Vector3(
            -5078.209961f,
            3356.969971f,
            -2457.178711f);
        const GW2LIB::Vector3 SABETHA_PHASE_POSITION;

        Sabetha(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        float getBossHeight() { return 300.0f; } //TODO: check this
        bool isAtStartPosition() {
            return (SABETHA_STARTING_POSITION.x == agent.GetPos().x) &&
                   (SABETHA_STARTING_POSITION.y == agent.GetPos().y) &&
                   (SABETHA_STARTING_POSITION.z == agent.GetPos().z);
        }

        void drawHealthTicks();

        void updateFlamewallState();
        void drawFlamewallStatus();

    public:

        static RaidBoss* instance(GW2LIB::Agent agent) { return new Sabetha(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        std::string getName() { return "Sabetha"; }

        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();

        void outputDebug(std::stringstream &ss);
};