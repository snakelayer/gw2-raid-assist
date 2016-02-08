void disableRaidAssist() {
	if (squad != nullptr) {
		delete squad;
	}
	squad = nullptr;

	if (boss != nullptr) {
		delete boss;
	}
	boss = nullptr;

	raid_boss_assist = false;
}

void threadRaidAssist() {
	float pBossHealth = 0;
	double pollingRate = 250; // ms

	timer::cpu_timer timer;
	while (true)
	{
		this_thread::interruption_point();

		if (raid_boss_assist) {
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
						if (boss != nullptr) {
							squad->setBoss(boss);
						}
						else {
							disableRaidAssist();
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

				if ((squad != nullptr && squad->turnOff()) ||
					(boss != nullptr && boss->isDead())) {
					disableRaidAssist();
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

			disableRaidAssist();

			if (!raid_boss_assist)
				Sleep(100); // Thread not needed, sleep
		}

		if (loopLimiter)
			Sleep(1);
	}
}
