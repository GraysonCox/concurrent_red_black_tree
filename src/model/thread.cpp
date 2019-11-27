//
// Created by Grayson Cox on 11/21/19.
//

#include "model/thread.h"

thread::thread(void *(*func)(void *), void *data) : func(func), data(data) {}

void thread::parbegin(std::vector<thread *> threads) {
	pthread_t tids[threads.size()];
	size_t i = 0;
	for (auto t : threads) {
		pthread_create(&tids[i], nullptr, t->func, t->data);
		i++;
	}
	for (i = 0; i < threads.size(); i++) {
		pthread_join(tids[i], nullptr);
	}
}