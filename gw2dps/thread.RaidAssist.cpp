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

				if (squad != nullptr) {
					squad->updateState();
				}

				if (boss != nullptr) {
					boss->updateState(bufferBossDps);
				}
			}
		}
		else {
			if (!timer.is_stopped()) {
				timer.stop();
			}
			if (boss != nullptr) {
				delete boss;
				boss = NULL;
			}
			if (squad != nullptr) {
				delete squad;
				squad = NULL;
			}
		}

		if (mark_raid_unit) {
			mark_raid_unit = false;

			if (squad == nullptr) {
				if (logRaidAssistToFile) {
					squad = new Squad(logRaidAssistFile);
				}
				else {
					squad = new Squad();
				}
			}

			GW2LIB::Agent agent = GetLockedSelection();

			if (agent.IsValid()) {
				//TODO: use boss matching functions
				if (agent.GetCharacter().GetMaxHealth() == 22021440) {
					boss = new ValeGuardian(agent);
					squad->setBoss(boss);
				}
				else {
					boss = new UnknownBoss(agent);
					squad->setBoss(boss);
				}
				//else if (!agent.GetCharacter().IsPlayer() && raid_debug) {
				//	boss = new ValeGuardian(agent);
				//	squad->setBoss(boss);
				//}
				//else if (agent.GetCharacter().IsPlayer()) {
				//	squad->addPlayer(agent);
				//}
			}
		}

		if (loopLimiter)
			Sleep(1);
	}
}
