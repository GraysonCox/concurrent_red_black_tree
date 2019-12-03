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

std::string task_to_string(task_t task) {
	return operation_to_string(task.op) + "(" + std::to_string(task.arg) + ")";
}

task_t task_from_string(std::string task) {
	task_t t;
	int index_of_left_parenthesis = task.find('(', 0);
	t.op = operation_from_string(task.substr(0, index_of_left_parenthesis));
	t.arg = stoi(task.substr(index_of_left_parenthesis + 1, task.length() - 1));
	return t;
}

thread_data::thread_data(std::string name, std::vector<task_t> *tasks) : name(name), tasks(tasks) {}
