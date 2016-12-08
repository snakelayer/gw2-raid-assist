#pragma once

#include <map>
#include <string>

#include <boost/assign.hpp>
#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>

#include "GameData.h"

#include "../boss/skills/draw/meter.h"

class SquadMember {
    private:
        static const float WALK_SPEED;
        static const float RUN_SPEED;
        static const float COMBAT_RUN_SPEED;
        static const float DODGE_SPEED;
        static const float GLIDE_SPEED;
        static const float SWIFTNESS_SPEED;
        static const float COMBAT_SWIFTNESS_SPEED;
        static const float SUPERSPEED;

        static const double HEALTH_METER_FADE_DELAY_MILLISECONDS;
        static std::map<GW2LIB::GW2::Race, float> raceHeightOffset;

        std::string name;
        GW2LIB::GW2::Profession profession;
        int dodgeCount;
        int superspeedCount;
        int heavyHitsTaken;
        float heavyDamageTaken;
        float totalDamageTaken;
        int directDamageOutput;
        int downedCount;

        float lastSpeed;
        float secondLastSpeed;
        bool isAlive;
        float lastHealth;
        float lastHealthDelta;

        bool shouldDrawHealthMeter;
        boost::timer::cpu_timer healthMeterTimer;
        Meter meter;

        boost::timer::cpu_timer uptimeTimer;
        boost::chrono::milliseconds totalUptime;

        int sumMight;
        int mightSamples;
        void addMight(int stacks);

        int sumFury;
        int furySamples;
        void addFury(int stacks);

        bool isBelowHalfHealth(GW2LIB::Character &character);
        DWORD interpolateHealthColor(float percent);

        void updateLastHealthDelta(GW2LIB::Character &character);
        void updateDamageTaken();
        void updateMovementStats(GW2LIB::Character &character);

        SquadMember() = delete;

    public:
        SquadMember(GW2LIB::Player &player);
        void updateStats(GW2LIB::Character &character);

        void upped();
        void downed();
        void updateUptime();
        boost::chrono::milliseconds getTotalUptime();

        std::string getName() { return name; }
        std::string getProfession();
        int getDodgeCount() { return dodgeCount; }
        int getSuperspeedCount() { return superspeedCount; }
        float getLastHealthDelta() { return lastHealthDelta; }
        int getHeavyHitsTaken() { return heavyHitsTaken; }
        float getHeavyDamageTaken() { return heavyDamageTaken; }
        float getTotalDamageTaken() { return totalDamageTaken; }
        int getDirectDamageOutput() { return directDamageOutput; }
        int getDownedCount() { return downedCount; }

        double getAverageMight() { return (mightSamples == 0) ? 0.0f : (sumMight / double(mightSamples)); }
        double getFuryUptime() { return (furySamples == 0) ? 0.0f : (sumFury / double(furySamples)); }

        void tryDrawHealthMeter(GW2LIB::Character &character);

        void takeHeavyHit();
        void addDirectDamageOutput(int damage) { directDamageOutput += damage; }
};
