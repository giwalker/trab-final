#pragma once

//====================================================================//
class Timer {
protected:
	bool FLAG_OFF_Delay, FLAG_ON_Delay;
	int Mh, Ml, Sh, Sl, Mt, St, total_time_ms, remaining_time_ms, initial_time_ms_OFF;
public:
	Timer();
	~Timer();
	virtual void setRTC(int, int, int, int, int) = 0;
	virtual void updateRTC() = 0;
};		//End class Timer
//====================================================================//