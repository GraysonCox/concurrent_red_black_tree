//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <unistd.h>
#include "io/file_reader.h"
#include "io/file_writer.h"
#include "io/input_file_parsing.h"
#include "model/rbtree.h"
#include "model/thread.h"
#include "model/thread_data.h"
#include "util/timer.h"

using namespace std;

const string SAMPLE_INPUT_FILE = "sample_input.txt";
const string OUTPUT_FILE = "output.txt";

file_reader *reader;
file_writer *writer;

rbtree *red_black_tree;

/**
 * This is the function that each thread runs.
 *
 * @param data - A thread_data object that holds the thread's name
 * 				and list of tasks to perform on the red-black tree.
 */
void *thread_func(void *data) {
	thread_data *thr_data = (thread_data *) data;
	bool search_result;
	for (task_t task : *thr_data->tasks) {
		printf("%s: %s\n", thr_data->name.c_str(), task_to_string(task).c_str());
		switch (task.op) {
			case SEARCH:
				search_result = red_black_tree->search(task.arg);
				writer->write_line(
						thr_data->name + ", "
						+ task_to_string(task)
						+ " -> " + (search_result ? "true" : "false") // TODO: Remove all magic strings.
				);
				printf("%s: Done -> %s\n", thr_data->name.c_str(), search_result ? "true" : "false");
				break;
			case INSERT:
				red_black_tree->insert_node(task.arg);
				printf("%s: Done\n", thr_data->name.c_str());
				break;
			case DELETE:
				red_black_tree->delete_node(task.arg);
				printf("%s: Done\n", thr_data->name.c_str());
				break;
			default:;
				// TODO: Error handling
		}
	}
	return nullptr;
}

int main(int argc, char **argv) {
	if (argc >= 2) {
		reader = new file_reader(argv[1]);
	} else {
		cout << "No input file specified. Reading from " << SAMPLE_INPUT_FILE << "." << endl;
		reader = new file_reader(SAMPLE_INPUT_FILE);
	}
	writer = new file_writer(OUTPUT_FILE);

	red_black_tree = parse_tree(reader);
	vector<thread_data *> thread_data_objects = parse_thread_names(reader);
	parse_tasks(reader, &thread_data_objects);

	vector<thread *> threads;
	for (thread_data *data : thread_data_objects) {
		threads.push_back(new thread(thread_func, data));
	}

	writer->write_line("Initial red-black tree:\n");
	writer->write_line(red_black_tree->to_string());

	cout << red_black_tree->to_string() << endl;

	timer t;
	t.start();
	thread::parbegin(threads);
	t.stop();

	cout << red_black_tree->to_string() << endl;

	writer->write_line("Execution time: " + to_string(t.get_time_microseconds()) + " us\n");

	writer->write_line("Final red-black tree:\n");
	writer->write_line(red_black_tree->to_string());

	return 0;
}