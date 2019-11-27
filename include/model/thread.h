//
// Created by Grayson Cox on 11/21/19.
//

#ifndef CONCURRENT_RED_BLACK_TREE_THREAD_H
#define CONCURRENT_RED_BLACK_TREE_THREAD_H


#include <vector>
#include <pthread.h>

class thread {
public:
	void *(*func)(void *);

	void *data; // TODO: Maybe make this a template class.

	thread(void *(*func)(void *), void *data);

	static void parbegin(std::vector<thread *> threads);
};


#endif //CONCURRENT_RED_BLACK_TREE_THREAD_H