void threadBossDps() {
	int pBossId = 0;
	float pBossHealth = 0;
	double pollingRate = 250; // ms

	timer::cpu_timer timer;
	while (true)
	{
		this_thread::interruption_point();

		if (boss == nullptr) {
			pBossId = 0;
			Sleep(1);
			continue;
		}

		if (logDps && (pBossId == boss->getAgentId())) {
			if (timer.is_stopped())
			{
				timer.start();
				bufferBossDps.push_front(0);
				pBossHealth = boss->getCurrentHealth();
				continue;
			}

			timer::cpu_times elapsed = timer.elapsed();
			double elapsedMs = elapsed.wall / 1e6;
			if (elapsedMs > pollingRate)
			{
				timer.start();
				float cHealth = boss->getCurrentHealth();

				if (pBossHealth == 0)
					pBossHealth = cHealth;

				float dmg = pBossHealth - cHealth;
				pBossHealth = cHealth;

				if (!dpsAllowNegative && dmg < 0)
					dmg = 0;

				bufferBossDps.push_front(dmg);
			}
		}
		else
		{
			if (!timer.is_stopped())
			{
				timer.stop();
				pBossHealth = 0;
			}

			if (!bufferBossDps.empty())
				bufferBossDps.clear();

			pBossId = boss->getAgentId();

			if (!logDps)
				Sleep(100); // Thread not needed, sleep
		}

		// go easy on the cpu
		if (loopLimiter)
			Sleep(1);
	}
}
