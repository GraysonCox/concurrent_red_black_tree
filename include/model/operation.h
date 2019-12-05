//
// Created by Grayson Cox on 11/27/19.
//

#ifndef OPERATION_H
#define OPERATION_H


#include <string>

/**
 * An enumeration of the red-black tree operations that a thread
 * will perform.
 */
typedef enum {
	SEARCH,
	INSERT,
	DELETE,
	UNKNOWN
} operation_name;

/**
 * Returns a string representation of the given operation_name.
 *
 * @param op - An operation_name.
 * @return A string representation of the given operation_name.
 */
std::string operation_to_string(operation_name op);

/**
 * Returns an operation_name decoded from the given string. If the
 * string does not represent a valid operation_t, this will return
 * UNKNOWN.
 *
 * @param op - A string representing an operation_t.
 * @return The operation_name decoded from the given string.
 */
operation_name operation_from_string(std::string op);

/**
 * A data type representing a red-black tree operation with a
 * specified int argument.
 */
class operation {
public:

	operation_name op;
	int arg;

	/**
	 * Constructs a task with the given operation_t and int argument.
	 *
	 * @param op - An operation_t.
	 * @param arg - An int.
	 */
	operation(operation_name op, int arg);

	/**
 	 * Constructs a task decoded from the given string.
 	 *
 	 * @param s - A string representing a task.
 	 */
	explicit operation(std::string s);

	/**
	 * Returns a string representation of this task.
	 *
	 * @return A string representation of this task.
	 */
	std::string to_string();
};


#endif //OPERATION_H
