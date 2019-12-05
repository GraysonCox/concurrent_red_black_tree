//
// Created by Grayson Cox on 12/4/19.
//

#ifndef OPERATION_QUEUE_H
#define OPERATION_QUEUE_H

#include <queue>
#include <pthread.h>
#include "operation.h"

/**
 * A wrapper for std::queue that supports synchronous dequeuing.
 */
class operation_queue {
private:

	std::queue<operation> q;

	pthread_mutex_t m;

public:

	/**
	 * Initializes a thread_safe_queue with the given queue.
	 *
	 * @param q - An std::queue.
	 */
	operation_queue(const std::queue<operation> &q) : q(q) {
		pthread_mutex_init(&m, nullptr);
	}

	/**
	 * Removes and returns a pointer to the next item in the queue.
	 *
	 * @return A pointer to the next item in the queue or nullptr if
	 * the queue is empty.
	 */
	operation *dequeue();

};


#endif //OPERATION_QUEUE_H
