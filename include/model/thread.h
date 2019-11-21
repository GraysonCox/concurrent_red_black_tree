//
// Created by Grayson Cox on 11/21/19.
//

#ifndef CONCURRENT_RED_BLACK_TREE_THREAD_H
#define CONCURRENT_RED_BLACK_TREE_THREAD_H


#include <string>
#include <vector>
#include <pthread.h>
#include "rbtree.h"

typedef enum {
	SEARCH,
	INSERT,
	DELETE,
	UNKNOWN
} operation_t;

std::string operation_to_string(operation_t op);

typedef struct task {
	operation_t op;
	int arg;
} task_t;

class thread {
private:
	std::string name;
	rbtree *tree;
	std::vector<task_t> tasks;
public:
	thread(const std::string &name, rbtree *tree);

	const std::string &get_name() const;

	rbtree *get_tree() const;

	void add_task(task_t t);

	const std::vector<task_t> &get_tasks() const;
};

void *start_thread(void *t);

void parbegin(std::vector<thread *> threads);


#endif //CONCURRENT_RED_BLACK_TREE_THREAD_H