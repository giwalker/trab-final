#include "Timer.h"

//====================================================================//
//Constructor
Timer::Timer() {
	FLAG_OFF_Delay = false, FLAG_ON_Delay = true;
	Mh = 0, Ml = 0, Sh = 0, Sl = 0, Mt = 0, St = 0, total_time_ms = 0, remaining_time_ms = 0, initial_time_ms_OFF = 0;
}
//Deconstructor
Timer::~Timer() {}
//====================================================================//