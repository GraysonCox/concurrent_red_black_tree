//
// Created by Grayson Cox on 11/21/19.
//

#include <iostream>
#include "model/thread.h"

using namespace std;

std::string operation_to_string(operation_t op) {
	switch (op) {
		case SEARCH:
			return "search";
		case INSERT:
			return "insert";
		case DELETE:
			return "delete";
		case UNKNOWN:
		default:
			return "Invalid Operation";
	}
}

thread::thread(const std::string &name, rbtree *tree) : name(name), tree(tree) {}

const std::string &thread::get_name() const {
	return name;
}

void thread::add_task(task_t t) {
	tasks.push_back(t);
}

const std::vector<task_t> &thread::get_tasks() const {
	return tasks;
}

rbtree *thread::get_tree() const {
	return tree;
}

void *start_thread(void *t) {
	thread *thr = (thread *)t;
	for (task_t task : thr->get_tasks()) {
		printf("%s: %s(%d)\n", thr->get_name().c_str(), operation_to_string(task.op).c_str(), task.arg);
//		cout << thr->get_name() << ": " << task.op << "(" << task.arg << ")" << endl;
		switch (task.op) {
			case SEARCH:
//				thr->get_tree()->search(task.arg);
				break;
			case INSERT:
//				thr->get_tree()->insert_node(task.arg);
				break;
			case DELETE:
//				thr->get_tree()->delete_node(task.arg);
				break;
			default: ;
				// TODO: Error handling
		}
	}
	return nullptr;
}

void parbegin(std::vector<thread *> threads) {
	pthread_t tids[threads.size()];
	size_t i = 0;
	for (auto t : threads) {
		pthread_create(&tids[i], nullptr, start_thread, t);
		i++;
	}
	for (i = 0; i < threads.size(); i++) {
		pthread_join(tids[i], nullptr);
	}
}
