//
// Created by Grayson Cox on 11/27/19.
//

#ifndef CONCURRENT_RED_BLACK_TREE_THREAD_DATA_H
#define CONCURRENT_RED_BLACK_TREE_THREAD_DATA_H


#include <string>
#include "rbtree.h"

typedef enum {
	SEARCH,
	INSERT,
	DELETE,
	UNKNOWN
} operation_t;

std::string operation_to_string(operation_t op);

operation_t operation_from_string(std::string op);

typedef struct task {
	operation_t op;
	int arg;
} task_t;

class thread_data {
public:
	std::string name;
	std::vector<task_t> *tasks;

	thread_data(const std::string &name, std::vector<task_t> *tasks);
};


#endif //CONCURRENT_RED_BLACK_TREE_THREAD_DATA_H
