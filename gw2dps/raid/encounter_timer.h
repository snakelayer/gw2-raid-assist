#pragma once

#include <boost/timer/timer.hpp>

class EncounterTimer
{
	private:
		boost::timer::cpu_timer timer;
		double lastElapsed;


	public:
		EncounterTimer();

		void start();
		void stop();
		bool isStopped();
		int getElapsedMilliseconds();
		int getElapsedSeconds();
		int getSplitSeconds();

};