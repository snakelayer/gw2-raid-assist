void disableRaidAssist() {
	if (boss != nullptr) {
		delete boss;
	}
	boss = nullptr;

	if (squad != nullptr) {
		delete squad;
	}
	squad = nullptr;

	raid_boss_assist = false;
}

void threadRaidAssist() {
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

					if (boss->isDead()) {
						disableRaidAssist();
					}
				}
			}
		}
		else {
			if (!timer.is_stopped()) {
				timer.stop();
			}
			disableRaidAssist();
		}

		if (loopLimiter)
			Sleep(1);
	}
}
