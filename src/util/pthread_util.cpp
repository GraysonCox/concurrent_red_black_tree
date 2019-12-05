//
// Created by Grayson Cox on 12/4/19.
//

#include "util/pthread_util.h"

std::string get_thread_id_string() {
	pthread_t id = pthread_self();
	std::stringstream stream;
	stream << std::hex << id;
	return stream.str();
}