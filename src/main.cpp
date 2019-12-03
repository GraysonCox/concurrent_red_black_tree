//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <util/timer.h>
#include <io/file_writer.h>
#include <io/input_file_parsing.h>
#include "io/file_reader.h"
#include "model/rbtree.h"
#include "model/thread_data.h"
#include "model/thread.h"

using namespace std;

const string INPUT_FILE = "/Users/administrator/Documents/ISU/COMS352/PA2/Grayson_Cox_Project2/src/dong.txt";
const string OUTPUT_FILE = "output.txt";

file_reader *reader;
file_writer *writer;

rbtree *red_black_tree;

void *thread_func(void *data) {
	thread_data *thr_data = (thread_data *) data;
	bool search_result;
	for (task_t task : *thr_data->tasks) {
		printf("%s (%d): %s(%d)\n", thr_data->name.c_str(), pthread_self(), operation_to_string(task.op).c_str(), task.arg);
		switch (task.op) {
			case SEARCH:
				search_result = red_black_tree->search(task.arg);
				writer->write_line(
						thr_data->name + ", "
						+ operation_to_string(task.op) + "(" + to_string(task.arg) + ")"
						+ " -> " + (search_result ? "true" : "false")
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

int main() {
	reader = new file_reader(INPUT_FILE);
	writer = new file_writer(OUTPUT_FILE);

	red_black_tree = parse_tree(reader);
	vector<thread_data *> thread_data_objects = parse_thread_names(reader);
	parse_tasks(reader, &thread_data_objects);

	vector<thread *> threads;
	for (thread_data *data : thread_data_objects) {
		threads.push_back(new thread(thread_func, data));
	}

	cout << red_black_tree->to_string() << endl;

	timer t;
	t.start();
	thread::parbegin(threads);
	t.stop();

	cout << red_black_tree->to_string() << endl;

	writer->write_line("Execution time: " + to_string(t.get_time_microseconds()) + " us");
	writer->write_line(red_black_tree->to_string());

	return 0;
}