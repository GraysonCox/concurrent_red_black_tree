//
// Created by Grayson Cox on 11/27/19.
//

#include "model/thread_data.h"

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

operation_t operation_from_string(std::string op) {
	if (op == "search")
		return SEARCH;
	if (op == "insert")
		return INSERT;
	if (op == "delete")
		return DELETE;
	return UNKNOWN;
}

thread_data::thread_data(std::string name, rbtree *tree, std::vector<task_t> *tasks) : name(name), tree(tree),
																					   tasks(tasks) {}
