#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include <boost/circular_buffer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

#include "gw2lib.h"

#include "../assist_drawer.h"
#include "../common_typedefs.h"
#include "../raid/encounter_timer.h"

namespace RB {
    enum HEALTH_MARKER {
        NONE = 0,
        THIRD,
        QUARTER
    };
}

class RaidBoss {
    public:
        RaidBoss(GW2LIB::Agent agent);
        virtual ~RaidBoss();

        void updateState();
        void outputDps(std::stringstream &ss);
        std::string getOutputHeader() { return outputHeader; }

        virtual std::string getName() = 0;
        virtual void updateState(boost::circular_buffer<float> &damageBuffer) = 0;
        virtual float getHeavyHitDamageThreshold() = 0;
        virtual void drawAssistInfo();

        virtual bool isDead();
        int getAgentId() { return agent.GetAgentId(); }
        float getCurrentHealth() { return agent.GetCharacter().GetCurrentHealth(); }
        float getBreakbar() { return agent.GetCharacter().GetBreakbarPercent(); }
        GW2LIB::GW2::BreakbarState getBreakbarState() { return agent.GetCharacter().GetBreakbarState(); }

        bool getScreenLocation(float *x, float *y, GW2LIB::Vector3 pos);

        virtual void outputDebug(std::stringstream &ss) = 0;

    protected:
        GW2LIB::Agent agent;

        RB::HEALTH_MARKER healthMarker;

        static int DPS_DURATIONS[3];
        float dps[3];

        EncounterTimer encounterTimer;
        int totalEncounterDuration;

        std::string outputHeader;

        virtual float getMaxHp() = 0;
        virtual float getBossHeight() = 0;
        GW2LIB::Vector3 getDrawAssistPosition();
        void startEncounter();
        bool tryResetBossAgent();
        bool hasTakenDamage() { return agent.GetCharacter().GetCurrentHealth() != agent.GetCharacter().GetMaxHealth(); }
        void drawToWindow(std::stringstream &ss, GW2LIB::Vector3 pos);
        void drawAtPosition(std::stringstream &ss, GW2LIB::Vector3 pos);
        void outputAssistHeader(std::stringstream &ss);

        void updateDps(boost::circular_buffer<float> &damageBuffer);

    private:
        static const std::string logFilePrefix;

        const float HEALTHBAR_TICK_WIDTH = 14.0f;
        const float HEALTHBAR_TICK_LENGTH = 252.0f;

        std::map<int, float> remainingHealthMap;
        float secondsToDeath;

        static const float BOMB_KIT_RANGE;

        const float X_BUFFER = 100.0f;
        const float Y_BUFFER = 50.0f;
        float lastX;
        float lastY;

        void drawHealthTicks();

        void writeDataToFile();
        void writeHealthData(std::ostream &stream);
        std::string getLogFileName();
};
