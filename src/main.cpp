//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <queue>
#include "io/file_reader.h"
#include "io/file_writer.h"
#include "io/input_file_parsing.h"
#include "model/rbtree.h"
#include "model/thread.h"
#include "model/operation.h"
#include "model/operation_queue.h"
#include "util/pthread_util.h"
#include "util/timer.h"

using namespace std;

const string SAMPLE_INPUT_FILE = "sample_input.txt";
const string OUTPUT_FILE = "output.txt";

file_reader *input_file_reader;
file_writer *output_file_writer;

operation_queue *read_tasks, *write_tasks;

rbtree *red_black_tree;

/**
 * The function run by each read thread.
 *
 * @param name - An identifier for the thread. This is nicer than
 * 				using just the hexadecimal pthread_t id.
 */
void *reader_thread_func(void *name) {
	string thread_name = *(string *) name;
	string thread_id = get_thread_id_string();
	operation *op;
	string search_result_string;
	while ((op = read_tasks->dequeue()) != nullptr) {
		printf("%s (%s): %s\n", thread_name.c_str(), thread_id.c_str(), op->to_string().c_str());
		switch (op->op) {
			case SEARCH:
				if (red_black_tree->search(op->arg))
					search_result_string = "true"; // TODO: Remove all magic strings.
				else
					search_result_string = "false";
				output_file_writer->write_line(
						op->to_string()
						+ " -> " + search_result_string
						+ ", performed by thread: " + thread_id
				);
				printf("%s (%s): Done -> %s\n", thread_name.c_str(), thread_id.c_str(), search_result_string.c_str());
				break;
			default:;
				// Ignore
		}
	}
	return nullptr;
}

/**
 * The function run by each write thread.
 *
 * @param name - An identifier for the thread. This is nicer than
 * 				just using the hexadecimal pthread_t id.
 */
void *writer_thread_func(void *name) {
	string thread_name_tag = *(string *) name;
	thread_name_tag += " (" + get_thread_id_string() + "): ";
	operation *op;
	while ((op = write_tasks->dequeue()) != nullptr) {
		printf("%s%s\n", thread_name_tag.c_str(), op->to_string().c_str());
		switch (op->op) {
			case INSERT:
				red_black_tree->insert_node(op->arg);
				break;
			case DELETE:
				red_black_tree->delete_node(op->arg);
				break;
			default:;
				// Ignore
		}
		printf("%sDone\n", thread_name_tag.c_str());
	}
	return nullptr;
}

int main(int argc, char **argv) {

	// Set up file i/o
	string file_name;
	if (argc >= 2) {
		file_name = argv[1];
	} else {
		cout << "No input file specified. Reading from " << SAMPLE_INPUT_FILE << "." << endl;
		file_name = SAMPLE_INPUT_FILE;
	}
	input_file_reader = new file_reader(file_name);
	output_file_writer = new file_writer(OUTPUT_FILE);

	// Parse and initialize red-black tree
	red_black_tree = parse_tree(input_file_reader);

	// Create threads
	vector<thread *> threads;
	string *thread_name;
	int num_reader_threads = parse_num_threads(input_file_reader);
	for (int i = 0; i < num_reader_threads; i++) {
		thread_name = new string();
		*thread_name = "reader_thread_" + to_string(i);
		threads.push_back(new thread(reader_thread_func, thread_name));
	}
	int num_writer_threads = parse_num_threads(input_file_reader);
	for (int i = 0; i < num_writer_threads; i++) {
		thread_name = new string();
		*thread_name = "writer_thread_" + to_string(i);
		threads.push_back(new thread(writer_thread_func, thread_name));
	}

	// Parse tasks and store in respective queues
	auto *tmp_read_tasks = new queue<operation>();
	auto *tmp_write_tasks = new queue<operation>();
	parse_tasks(input_file_reader, tmp_read_tasks, tmp_write_tasks);
	read_tasks = new operation_queue(*tmp_read_tasks);
	write_tasks = new operation_queue(*tmp_write_tasks);

	cout << red_black_tree->to_string_pretty() << endl;

	// Execute the threads and record execution time
	timer t;
	t.start();
	thread::parbegin(threads);
	t.stop();

	cout << red_black_tree->to_string_pretty() << endl;

	// Write execution time to output file
	output_file_writer->write_line("\nExecution time: " + to_string(t.get_time_microseconds()) + " μs\n");

	// Write final red-black tree to output file
	output_file_writer->write_line("Final red-black tree:\n");
	output_file_writer->write_line("\t" + red_black_tree->to_string() + "\n");

	return 0;
}