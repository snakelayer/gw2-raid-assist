#include "globals.h"

using namespace GW2LIB;

Texture profIcon[GW2::PROFESSION_END];
float icon_w = 20;
float icon_h = 20;

// Settings //
bool killApp = false;

bool help = false;
bool expMode = false;
bool selfFloat = false;
bool selfHealthPercent = true;
bool loopLimiter = true;

bool showPing = false;
bool woldBosses = false;
bool compDotsFade = false;
bool compDots = false;
bool targetSelected = true;
bool targetInfo = false;
bool targetInfoAlt = false;
bool targetLock = false;

bool dpsAllowNegative = false;

bool logDps = true;
bool logDpsDetails = false;
string logDpsFile = "gw2dpsLog-Dps.txt";

float averageDps[3] = { 0.0f, 0.0f, 0.0f };
float secondsToDeath = 0.0f;
bool logRaidAssistToFile = true;

bool logKillTimer = false;
bool logKillTimerDetails = false;
bool logKillTimerToFile = false;

bool logHits = false;
bool logHitsDetails = false;
bool logHitsToFile = false;
string logHitsFile = "gw2dpsLog-Hits.txt";
int threadHitsCounter = 0;

bool logAttackRate = false;
bool logAttackRateDetails = false;
bool logAttackRateToFile = false;
int AttackRateChainHits = 1;
int AttackRateChainTime = 0; // not used atm
string logAttackRateFile = "gw2dpsLog-AttackRate.txt";
int threadAttackRateCounter = 0;

bool logCrits = false;
bool logCritsDetails = true;
int logCritsSample = 0;
int logCritsGlances = 0;
int logCritsNormals = 0;
int logCritsCrits = 0;
bool logCritsToFile = false;
string logCritsFile = "gw2dpsLog-Crits.txt";

bool alliesList = false;
int wvwBonus = 0;

bool floatAllyNpc = false;
bool floatEnemyNpc = false;
bool floatAllyPlayer = false;
bool floatAllyPlayerProf = false;
bool floatEnemyPlayer = false;
bool floatSiege = false;
int floatRadius = 7000;
bool floatCircles = false;
bool floatType = true; // false = HP; true = Dist;

bool logSpeedometer = false;
bool logSpeedometerEnemy = false;
int logDisplacementValue = 0;
bool logDisplacement = false;
bool logDisplacementEnemy = false;
Vector3 logDisplacementStart = Vector3(0, 0, 0);

bool mouse_down = false;
int mouse_delta = 0, mouse_btn = 0, mouse_x = 0, mouse_y = 0, mouse_keys = 0;
string chat;

DWORD thread_id_hotkey = 0;

Character me;
Agent meAg = me.GetAgent();
CompassOverlay *compOverlay = nullptr;

PersonalCombat pc;

bool raid_debug = false;
bool raid_boss_assist = false;
bool raid_boss_assist_was_on = false;

Squad *squad;
RaidBoss *boss;

int targetLockID;

Target selected;
Target locked;
Target self;

Killtimer bufferKillTimer;

Displacement bufferDisplacement;
boost::circular_buffer<float> bufferDps(240); // 1 minute for 250ms sampling rate
boost::circular_buffer<float> bufferBossDps(240); // 1 minute for 250ms sampling rate
boost::circular_buffer<int> bufferHits(50);
boost::circular_buffer<double> bufferAttackRate(50); // seconds
boost::circular_buffer<int> bufferSpeedometer(30); // inches/sec, 100ms sampleRate,3s worth
