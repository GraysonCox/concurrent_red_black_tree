//
// Created by Grayson Cox on 11/27/19.
//

#include "model/operation.h"

std::string operation_to_string(operation_name op) {
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

operation_name operation_from_string(std::string op) {
	if (op == "search")
		return SEARCH;
	if (op == "insert")
		return INSERT;
	if (op == "delete")
		return DELETE;
	return UNKNOWN;
}

operation::operation(operation_name op, int arg) : op(op), arg(arg) {}

operation::operation(std::string task_str) {
	int index_of_left_parenthesis = task_str.find('(', 0);
	op = operation_from_string(task_str.substr(0, index_of_left_parenthesis));
	arg = stoi(task_str.substr(index_of_left_parenthesis + 1, task_str.length() - 1));
}

std::string operation::to_string() {
	return operation_to_string(op) + "(" + std::to_string(arg) + ")";
}
