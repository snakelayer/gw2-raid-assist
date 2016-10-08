#include "personal_combat.h"

using namespace boost;
using namespace GW2LIB::GW2;

PersonalCombat::PersonalCombat() {
    timer.stop();
}

void PersonalCombat::record(CombatLogType type, int amount) {
    if (timer.is_stopped()) {
        timer.start();
    }

    switch (type) {
    case CL_CONDI_DMG_IN:
        condiDamageTaken += amount;
        break;
    case CL_CRIT_DMG_IN:
    case CL_GLANCE_DMG_IN:
    case CL_PHYS_DMG_IN:
        directDamageTaken += amount;
        break;
    case CL_HEAL_IN:
        healingTaken += amount;
        break;
    case CL_CONDI_DMG_OUT:
        condiDamageGiven += amount;
        break;
    case CL_CRIT_DMG_OUT:
    case CL_GLANCE_DMG_OUT:
    case CL_PHYS_DMG_OUT:
        directDamageGiven += amount;
        break;
    case CL_HEAL_OUT:
        healingGiven += amount;
        break;
    }
}

void PersonalCombat::reset() {
    if (!timer.is_stopped()) {
        writeToFile();
    }

    timer.stop();
    condiDamageTaken = directDamageTaken = healingTaken = 0;
    condiDamageGiven = directDamageGiven = healingGiven = 0;
}

int PersonalCombat::getTotalDamageGiven() {
    return condiDamageGiven + directDamageGiven;
}

int PersonalCombat::getTotalDps() {
    int seconds = int(timer.elapsed().wall / 1e9);
    if (seconds > 0) {
        return getTotalDamageGiven() / seconds;
    }

    return 0;
}

void PersonalCombat::writeToFile() {
    std::ofstream file;
    file.open(getOutputFileName(), std::ofstream::out | std::ofstream::app);

    if (file.is_open()) {
        file << format("\n// start personal combat output\n");
        file << format("Condition Damage Taken: %d\n") % condiDamageTaken;
        file << format("Direct Damage Taken:    %d\n") % directDamageTaken;
        file << format("Healing Taken:          %d\n\n") % healingTaken;

        file << format("Condition Damage Given: %d\n") % condiDamageGiven;
        file << format("Direct Damage Given:    %d\n") % directDamageGiven;
        file << format("Healing Given:          %d\n\n") % healingGiven;

        file << format("Total Duration(sec): %d\n") % int(timer.elapsed().wall / 1e9);
        file << format("Total Damage Given:  %d\n") % getTotalDamageGiven();
        file << format("Total DPS:           %d\n") % getTotalDps();
        file << format("// end personal combat output\n");
        file.close();
    }
}
