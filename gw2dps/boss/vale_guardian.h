#pragma once

#include <sstream>

#include <boost/circular_buffer.hpp>
#include <boost/format.hpp>
#include <boost/timer/timer.hpp>

#include "gw2lib.h"

#include "raid_boss.h"
#include "skills/magic_storm.h"
#include "skills/seeker.h"
#include "skills/unstable_pylon.h"

namespace VG {

    enum Phase {
        FIRST = 0,
        FIRST_TRANSITION,
        FIRST_SPLIT,
        SECOND,
        SECOND_TRANSITION,
        SECOND_SPLIT,
        THIRD
    };
}

class ValeGuardian : public RaidBoss
{
    private:
        static const float MAX_HP;
        const float FIRST_PHASE_TRANSITION_HP = 14553669;
        const float SECOND_PHASE_TRANSITION_HP = 7255058;

        // for reference, HP of each guardian at the entrance:
        // red:   2457750
        // green: 3440850
        // blue:  3539160
        const float RED_GUARDIAN_MAX_HP = 540705;
        const float GREEN_GUARDIAN_MAX_HP = 722579;
        const float BLUE_GUARDIAN_MAX_HP = 540705; // TODO: verify

        const float magicStormDisplayOffset = 40.0f;

        std::list<Seeker> seekers;
        MagicStorm magicStorm;
        UnstablePylon unstablePylon;
        VG::Phase phase;

        ValeGuardian(GW2LIB::Agent agent);

        float getMaxHp() { return MAX_HP; }
        float getBossHeight() { return 350.0f; }
        bool isSplit() { return !(phase == VG::Phase::FIRST || phase == VG::Phase::SECOND || phase == VG::Phase::THIRD); }
        bool isPylonPhase() { return phase == VG::Phase::SECOND || phase == VG::Phase::THIRD; }

        void updateSeekerState();
        void updateMagicStormState();
        void updatePylonState();

        void addEstTimeToSplit(std::stringstream &ss);
        void drawSeekerStatus();
        void drawMagicStormStatus();
        void drawUnstablePylonStatus();

        bool findRedGuardian();

    public:
        ~ValeGuardian();

        static RaidBoss* instance(GW2LIB::Agent agent) { return new ValeGuardian(agent); }
        static bool matchesTarget(GW2LIB::Agent &agent);

        std::string getName() { return "Vale Guardian"; }

        void updateState(boost::circular_buffer<float> &damageBuffer);
        void drawAssistInfo();


        bool isDead() { return !isSplit() && RaidBoss::isDead(); }

        void outputDebug(std::stringstream &ss);
};
