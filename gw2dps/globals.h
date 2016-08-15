#pragma once

#include "gw2dps.h"

/* DO NOT INITIALIZE VARIABLES HERE, EVEN CONSTANTS, put them in globals.cpp instead */
/* to make a variable global, it must have "extern" so it will be seen across translation units */

extern GW2LIB::Texture profIcon[GW2LIB::GW2::PROFESSION_END];
extern GW2LIB::Texture eliteIcon[GW2LIB::GW2::PROFESSION_END];
extern float icon_w;
extern float icon_h;

// Settings //
extern bool killApp;

extern bool help;
extern bool expMode;
extern bool selfFloat;
extern bool selfHealthPercent;
extern bool loopLimiter;

extern bool showPing;
extern bool woldBosses;
extern bool compDotsFade;
extern bool compDots;
extern bool targetSelected;
extern bool targetInfo;
extern bool targetInfoAlt;
extern bool targetLock;

extern bool dpsAllowNegative;

extern bool logDps;
extern bool logDpsDetails;
extern std::string logDpsFile;

extern float averageDps[3];
extern float secondsToDeath;
extern bool logRaidAssistToFile;

extern bool logKillTimer;
extern bool logKillTimerDetails;
extern bool logKillTimerToFile;

extern bool logHits;
extern bool logHitsDetails;
extern bool logHitsToFile;
extern std::string logHitsFile;
extern int threadHitsCounter;

extern bool logAttackRate;
extern bool logAttackRateDetails;
extern bool logAttackRateToFile;
extern int AttackRateChainHits;
extern int AttackRateChainTime;
extern std::string logAttackRateFile;
extern int threadAttackRateCounter;

extern bool logCrits;
extern bool logCritsDetails;
extern int logCritsSample;
extern int logCritsGlances;
extern int logCritsNormals;
extern int logCritsCrits;
extern bool logCritsToFile;
extern std::string logCritsFile;

extern bool alliesList;
extern int wvwBonus;

extern bool floatAllyNpc;
extern bool floatEnemyNpc;
extern bool floatAllyPlayer;
extern bool floatAllyPlayerProf;
extern bool floatEnemyPlayer;
extern bool floatSiege;
extern int floatRadius;
extern bool floatCircles;
extern bool floatType;

extern bool logSpeedometer;
extern bool logSpeedometerEnemy;
extern int logDisplacementValue;
extern bool logDisplacement;
extern bool logDisplacementEnemy;
extern GW2LIB::Vector3 logDisplacementStart;

extern bool mouse_down;
extern int mouse_delta, mouse_btn, mouse_x, mouse_y, mouse_keys;
extern std::string chat;

extern DWORD thread_id_hotkey;

extern GW2LIB::Character me;
extern GW2LIB::Agent meAg;
extern CompassOverlay *compOverlay;

extern PersonalCombat pc;

extern bool raid_debug;
extern bool raid_boss_assist;
extern bool raid_boss_assist_was_on;

extern Squad *squad;
extern RaidBoss *boss;

extern int targetLockID;

extern Target selected;
extern Target locked;
extern Target self;

extern Killtimer bufferKillTimer;

extern Displacement bufferDisplacement;
extern boost::circular_buffer<float> bufferDps;
extern boost::circular_buffer<float> bufferBossDps;
extern boost::circular_buffer<int> bufferHits;
extern boost::circular_buffer<double> bufferAttackRate;
extern boost::circular_buffer<int> bufferSpeedometer;
