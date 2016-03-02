#pragma once

#include "gw2lib.h"

#include "raid_boss.h"

class Gorseval : public RaidBoss
{
    private:
        static const float MAX_HP;
        const float FIRST_TRANSITION_HP = 14190625;
        const float SECOND_TRANSITION_HP = 7020792;

        /*
          Haunting Aura is generally around 1.2k
          at 1000 toughness:
           swipe is 3.1k - 3.3k
           spectral impact is 4.1k - 5.7k
          at 1600 toughness:
           swipe is 2.4k - 2.6k
           spectral impact is 3.0k - 4.5k

          this is intended to ignore haunting aura, but trigger on swipe and spectral impact
        */
        static const float HEAVY_HIT_DAMAGE_THRESHOLD;

        Gorseval(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        float getBossHeight() { return 500.0f; } //TODO: check this

    public:
        static RaidBoss* instance(GW2LIB::Agent agent) { return new Gorseval(agent); }

        std::string getName() { return "Gorseval"; }
        static bool matchesTarget(GW2LIB::Agent &agent);

        void updateState(boost::circular_buffer<float> &damageBuffer);
        float getHeavyHitDamageThreshold() { return HEAVY_HIT_DAMAGE_THRESHOLD; }
        void drawAssistInfo();

        void outputDebug(std::stringstream &ss);
};