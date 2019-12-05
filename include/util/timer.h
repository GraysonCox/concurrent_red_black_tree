//
// Created by Grayson Cox on 11/27/19.
//

#ifndef TIMER_H
#define TIMER_H


#include <chrono>

/**
 * A class that acts as a stopwatch for program execution.
 */
class timer {
private:

	std::chrono::time_point<std::chrono::steady_clock> initial_time;

	std::chrono::time_point<std::chrono::steady_clock> end_time;

public:

	/**
	 * Sets the initial time of the timer.
	 */
	void start();

	/**
	 * Stops recording the time. After this is called,
	 * the time can be retrieved with get_time_microseconds().
	 */
	void stop();

	/**
	 * Returns the number of microseconds elapsed between invocations
	 * of start() and stop().
	 *
	 * @return - The number of microseconds elapsed between invocations
	 * 			of start() and stop().
	 */
	long long int get_time_microseconds();
};


#endif //TIMER_H