//
// Created by Grayson Cox on 11/27/19.
//

#ifndef CONCURRENT_RED_BLACK_TREE_TIMER_H
#define CONCURRENT_RED_BLACK_TREE_TIMER_H


#include <chrono>

class timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> initial_time;
	std::chrono::time_point<std::chrono::steady_clock> end_time;
public:
	void start();
	void stop();
	long long int get_time_microseconds();
};

#endif //CONCURRENT_RED_BLACK_TREE_TIMER_H
