#pragma once

// Boost v1.56 library
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/timer/timer.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include <sstream> // stringstream
#include <algorithm> // min/max_element()
#include <fstream> // ofstream / file
#include <locale> // locale

#include "gw2lib.h"
#include "config.h"
#include "keymap.h"
#include "hotkey.h"
#include "preferences.h"
#include "compass.h"

#include "hacklib/Main.h"
#include "hacklib/Logging.h"

#include "assist_drawer.h"
#include "boss/raid_boss.h"
#include "combat/personal_combat.h"
#include "raid/squad.h"

// threads
void threadAttackRate();
void threadCrits();
void threadDps();
void threadHits();
void threadHotKeys();
void threadKillTimer();
void threadRaidAssist();
void threadSpeedometer();

// THREADS //
struct Target {
    Target() : valid(false), alive(true), id(0), type(0), cHealth(0), mHealth(0), pHealth(0), lvl(0), lvlActual(0), pos(GW2LIB::Vector3(0,0,0)), breakbar(0), rot(0) {}

    bool valid;
    bool alive;
    int id;
    int type; // 0 char, 9 struct, 10 wboss
    float cHealth;
    float mHealth;
    float pHealth;
    int lvl;
    int lvlActual;
    float breakbar;

    GW2LIB::Vector3 pos;
    float rot;
};

struct Ally {
    Ally() : id(0), profession(0), lvl(0), lvlActual(0), cHealth(0), mHealth(0), pHealth(0), vitality(0), pos({ 0, 0, 0 }), name(""), rot(0) {}

    int id;
    int profession;
    int lvl;
    int lvlActual;
    float cHealth; // current health
    float mHealth; // max health
    float pHealth; // current health in percent

    GW2LIB::Vector3 pos;
    float rot;

    int vitality;
    string name;
};

struct Allies {
    vector<Ally> war;
    vector<Ally> necro;
    vector<Ally> rev;

    vector<Ally> mes;
    vector<Ally> ranger;
    vector<Ally> engi;

    vector<Ally> guard;
    vector<Ally> ele;
    vector<Ally> thief;
};

struct Float {
    GW2LIB::Vector3 pos;
    float rot = 0;
    float cHealth;
    float mHealth;
    int prof;
    bool eliteSpec = false;
    string name;
};

struct Siege {
    int type;
    GW2LIB::Vector3 pos;
    float mHealth;
    float cHealth;
};

struct Floaters {
    vector<Float> allyNpc;
    vector<Float> enemyNpc;
    vector<Float> allyPlayer;
    vector<Float> enemyPlayer;
    vector<Siege> siege;
};

struct WBoss {
    int id;
    GW2LIB::Vector3 pos;
    float mHealth;
    float cHealth;
    float pHealth;
};

struct WBosses {
    vector<WBoss> list;
};

struct Killtimer {
    Killtimer() : dmg(0), dps(0), time(0), samplesKnown(0), samplesUnknown(0) {}

    float dmg;
    double dps;
    double time; // seconds
    int samplesKnown;
    int samplesUnknown;
};

struct Displacement {
    Displacement() : id(0), start(GW2LIB::Vector3(0, 0, 0)), dist(0) {}

    int id;
    GW2LIB::Vector3 start;
    int dist;
};

// Layout Anchors
struct Anchor {
    Anchor() : x(0), y(0) {}

    float x;
    float y;
};

struct baseHpReturn {
    float health;
    float vitality;
};

float Dist(GW2LIB::Vector3 p1, GW2LIB::Vector3 p2);
string SecondsToString(double input);
baseHpReturn baseHp(int lvl, int profession);

void load_preferences();
void save_preferences();

void displayDebug();
void displayAgent(std::string prefix, GW2LIB::Agent &agent, stringstream &ss);

float computeAverage(size_t samples, boost::circular_buffer<float> bufferDps);

void drawElementAt(std::stringstream &ss, Anchor &aTopLeft);
