//
// Created by Grayson Cox on 11/27/19.
//

#ifndef THREAD_DATA_H
#define THREAD_DATA_H


#include <string>
#include "rbtree.h"

/**
 * An enumeration of the red-black tree operations that a thread
 * will perform.
 */
typedef enum {
	SEARCH,
	INSERT,
	DELETE,
	UNKNOWN
} operation_t;

/**
 * Returns a string representation of the given operation_t.
 *
 * @param op - An operation_t.
 * @return A string representation of the given operation_t.
 */
std::string operation_to_string(operation_t op);

/**
 * Returns an operation_t decoded from the given string. If the
 * string does not represent a valid operation_t, this will return
 * UNKNOWN.
 *
 * @param op - A string representing an operation_t.
 * @return The operation_t decoded from the given string.
 */
operation_t operation_from_string(std::string op);

/**
 * A data type representing a red-black tree operation with a
 * specified int argument.
 */
typedef struct task {
	operation_t op;
	int arg;
} task_t;

/**
 * Returns a string representation of the given task_t.
 *
 * @param task - A task_t.
 * @return A string representation of the given task_t.
 */
std::string task_to_string(task_t task);

/**
 * Returns a task_t decoded from the given string.
 *
 * @param task - A string representing a task_t.
 * @return A task_t decoded from the given string.
 */
task_t task_from_string(std::string task);

/**
 * Developer-defined data type for passing arguments to threads.
 */
class thread_data {
public:

	/**
	 * The name of the thread using this thread_data.
	 */
	std::string name;

	/**
	 * A vector containing the tasks that this thread will
	 * perform when run.
	 */
	std::vector<task_t> *tasks;

	/**
	 * Initializes a thread_data object with the given name and tasks.
	 *
	 * @param name - The name of the thread using this thread_data.
	 * @param tasks - The tasks that the thread will perform when run.
	 */
	thread_data(std::string name, std::vector<task_t> *tasks);
};


#endif //THREAD_DATA_H
