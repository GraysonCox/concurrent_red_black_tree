//
// Created by Grayson Cox on 11/21/19.
//

#ifndef THREAD_H
#define THREAD_H


#include <vector>
#include <pthread.h>

/**
 * An object-oriented implementation of a thread using the pthread.h library.
 * A thread runs a developer-defined function with parameters from a
 * developer-defined data type.
 *
 * To create a thread object, define a function and a data type to pass to the
 * constructor. Then, run the thread with the static parbegin() method. This will
 * execute the given function with the given data type as a parameter.
 */
class thread {
public:

	/**
	 * A pointer to a developer-defined function of type void *(void *).
	 * When run, the thread will execute this function and pass data (the
	 * developer-defined data type) as a parameter.
	 */
	void *(*func)(void *);

	/**
	 * A pointer to a developer-defined data object. This object will
	 * be passed to the developer-defined function when the thread is run.
	 */
	void *data; // TODO: Maybe make this a template class.

	/**
	 * Initializes a thread object with the given parameters.
	 *
	 * @param func - A pointer to the function that this thread will execute.
	 * @param data - The developer-defined data type that the thread will access
	 * 				during execution.
	 */
	thread(void *(*func)(void *), void *data);

	/**
	 * Synchronously runs the given threads using the pthread.h library.
	 *
	 * @param threads - A vector containing the threads to be run.
	 */
	static void parbegin(std::vector<thread *> threads);
};


#endif //THREAD_H