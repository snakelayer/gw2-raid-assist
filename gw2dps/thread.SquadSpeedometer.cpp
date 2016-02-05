#include <map>
#include <utility>

#include "common_typedefs.h"

void threadSquadSpeedometer() {
	double pollingRate = 100; // ms

	boost::timer::cpu_timer timer;
	timer.stop();

	CharacterPositions characterPositions;
	CharacterSpeeds characterSpeeds;
	while (true)
	{
		this_thread::interruption_point();

		if (squad != nullptr) {
			if (timer.is_stopped())
			{
				timer.start();
				continue;
			}

			double elapsedMs = timer.elapsed().wall / 1e6;
			if (elapsedMs > pollingRate)
			{
				timer.start();

				CharacterMap characterMap = squad->getCharacterMap();
				for (auto &characterEntry : characterMap) {
					string characterName = characterEntry.first;
					Character character = characterEntry.second;

					Vector3 currentPos = character.GetAgent().GetPos();

					if (characterPositions.find(characterName) != characterPositions.end()) {
						int distance = int(Dist(characterPositions[characterName], currentPos));

						if (characterSpeeds[characterName].capacity() == 0) {
							characterSpeeds[characterName].set_capacity(9); // sample for a length of time equal to 1 dodge (+ 1 more sample)
						}
						characterSpeeds[characterName].push_front(distance);
					}

					characterPositions[characterName] = currentPos;
				}

				squad->updateDodgeState(characterSpeeds);
			}
		}
		else
		{
			if (!timer.is_stopped())
				timer.stop();

			characterPositions.clear();
			characterSpeeds.clear();

			Sleep(100); // Thread not needed, sleep
		}

		// go easy on the cpu
		if (loopLimiter)
			Sleep(1);
	}
}
