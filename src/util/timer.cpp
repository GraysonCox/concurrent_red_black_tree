//
// Created by Grayson Cox on 11/27/19.
//

#include "util/timer.h"

void timer::start() {
	initial_time = std::chrono::steady_clock::now();
}

void timer::stop() {
	end_time = std::chrono::steady_clock::now();
}

long long int timer::get_time_microseconds() {
	return std::chrono::duration_cast<std::chrono::microseconds>(end_time - initial_time).count();
}
