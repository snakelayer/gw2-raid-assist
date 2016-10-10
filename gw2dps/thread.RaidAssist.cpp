#include "boss/raid_boss_factory.h"

// for bug fixing...
#include "hacklib/Logging.h"

int disableRaidAssist() {
    if (squad != nullptr) {
        delete squad;
    }
    squad = nullptr;

    if (boss != nullptr) {
        delete boss;
    }
    boss = nullptr;

    raid_boss_assist = false;
    if (raid_boss_assist_was_on) {
        raid_boss_assist_was_on = false;
        pc.reset();
    }

    return 0;
}

void threadRaidAssist() {
    int mapId = 0;
    float pBossHealth = 0;
    double pollingRate = 250; // ms

    timer::cpu_timer timer;
    while (true)
    {
        this_thread::interruption_point();

        if (raid_boss_assist) {
            if (mapId == 0) {
                mapId = GetCurrentMapId();
            }
            else if (mapId != GetCurrentMapId()) {
                mapId = disableRaidAssist();
                continue;
            }

            if (timer.is_stopped()) {
                timer.start();
            }

            timer::cpu_times elapsed = timer.elapsed();
            double elapsedMs = elapsed.wall / 1e6;
            if (elapsedMs > pollingRate) {
                timer.start();
                
                //squad&boss creation/update scope
                {
                    if (squad == nullptr) {
                        squad = new Squad();
                    }
                    else {
                        squad->updateState();
                    }

                    if (boss == nullptr) {
                        boss = RaidBossFactory::get().getNextBoss();
                        if (squad != nullptr && boss != nullptr) {
                            boss->setSquad(squad);
                            squad->setBoss(boss);
                        }
                        else {
                            mapId = disableRaidAssist();
                        }
                    }
                    else {
                        boss->updateState(bufferBossDps);
                    }
                }

                // boss damage buffer scope
                if (boss != nullptr) {
                    float cHealth = boss->getCurrentHealth();

                    if (pBossHealth == 0)
                        pBossHealth = cHealth;

                    float dmg = pBossHealth - cHealth;
                    pBossHealth = cHealth;

                    if (!dpsAllowNegative && dmg < 0)
                        dmg = 0;

                    bufferBossDps.push_front(dmg);
                }

                if (squad != nullptr && boss != nullptr) {
                    mapId = GetCurrentMapId();
                }

                if ((squad != nullptr && squad->turnOff()) ||
                    (boss != nullptr && boss->isDead())) {
                    HL_LOG_DBG("squad off or boss dead: turnOff:%s, isDead:%s\n", squad->turnOff() ? "yes" : "no", boss->isDead() ? "yes" : "no");
                    mapId = disableRaidAssist();
                }
            }
        }
        else {
            if (!timer.is_stopped()) {
                timer.stop();
                pBossHealth = 0;
            }

            if (!bufferBossDps.empty())
                bufferBossDps.clear();

            mapId = disableRaidAssist();

            if (!raid_boss_assist)
                Sleep(100); // Thread not needed, sleep
        }

        if (loopLimiter)
            Sleep(1);
    }
}
