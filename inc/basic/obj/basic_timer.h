#ifndef __BASIC_TIMER_H__
#define __BASIC_TIMER_H__

#ifdef BASIC_ANDROID
#include <sys/time.h>
#endif

#ifdef BASIC_TIZEN

#include <sys/time.h>

#endif

#ifdef BASIC_WIN
#include <Windows.h>
#endif

class BasicTimer {
	timeval start_time;

public:
	/**
	 * @brief Initiate BasicTimer
	 */
	void InitTimer();

	/**
	 * @brief Get elapsed time from starting
	 *
	 * @return time as seconds
	 */
	double GetElapsedTime() const;
};


#endif //__BASIC_TIMER_H__


