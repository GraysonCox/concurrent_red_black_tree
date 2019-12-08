//
// Created by Grayson Cox on 12/4/19.
//

#include "model/operation_queue.h"

operation_queue::operation_queue(const std::queue<operation> &q) : q(q) {
	pthread_mutex_init(&m, nullptr);
}

operation_queue::~operation_queue() {
	pthread_mutex_destroy(&m);
}

operation *operation_queue::dequeue() {
	operation *next_item;
	pthread_mutex_lock(&m);
	if (q.empty()) {
		next_item = nullptr;
	} else {
		operation tmp = q.front();
		next_item = new operation(tmp.op, tmp.arg);
		q.pop();
	}
	pthread_mutex_unlock(&m);
	return next_item;
}
