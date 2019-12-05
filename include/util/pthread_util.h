//
// Created by Grayson Cox on 12/4/19.
//

#ifndef PTHREAD_UTIL_H
#define PTHREAD_UTIL_H

#include <iomanip>
#include <sstream>
#include <pthread.h>
#include <string>

/**
 * Returns a hexadecimal string representation of the calling thread's id.
 *
 * @return A hexadecimal string representation of the calling thread's id.
 */
std::string get_thread_id_string();


#endif //PTHREAD_UTIL_H
