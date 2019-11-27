//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <sstream>
#include "io/file_reader.h"
#include "model/rbtree.h"
#include "model/thread_data.h"
#include "model/thread.h"

using namespace std;

const string INPUT_FILE = "/Users/administrator/Documents/ISU/COMS352/PA2/Grayson_Cox_Project2/src/input.txt";

template<typename T>
vector<T> to_vector(initializer_list<T> list) {
	vector<T> v;
	for (auto t : list) {
		v.push_back(t);
	}
	return v;
}

rbtree parse_tree(file_reader *reader) { // TODO: This isn't good modularity.
	vector<rbnode *> nodes;
	string str = reader->read_line();
	str.push_back(',');
	stringstream stream(str);
	string token;
	int key;
	char color_code;
	while (std::getline(stream, token, ',')) {
		if (token == "f") {
			nodes.push_back(nullptr);
		} else {
			stringstream ss(token);
			ss >> key;
			ss >> color_code;
			if (color_code == 'r')
				nodes.push_back(new rbnode(key, RED));
			else
				nodes.push_back(new rbnode(key, BLACK));
		}
	}
	return rbtree(nodes);
}

vector<thread_data *> parse_threads(file_reader *reader, rbtree *t) {
	vector<thread_data *> threads;
	string name = reader->read_line();
	name = reader->read_line();
	while (name != "") {
		threads.push_back(new thread_data(name, t, new vector<task_t>()));
		name = reader->read_line();
	}
	return threads;
}

void parse_tasks(file_reader *reader, vector<thread_data *> *threads) {
	string thread_name;
	string op;
	string arg;
	while (!reader->is_end_of_file()) {
		thread_name = reader->read_token();
		thread_name.pop_back(); // Remove comma
		op = reader->read_to('(');
		arg = reader->read_to(')');

		task_t task;
		task.op = operation_from_string(op);
		task.arg = atoi(arg.c_str());
		for (thread_data *t : *threads) {
			if (t->name == thread_name) {
				t->tasks->push_back(task);
				break;
			}
		}

		if (!reader->is_end_of_file()) {
			reader->read_token();
		}
	}
}

void *thread_func(void *data) {
	thread_data *thr_data = (thread_data *) data;
	for (task_t task : *thr_data->tasks) {
		printf("%s: %s(%d)\n", thr_data->name.c_str(), operation_to_string(task.op).c_str(), task.arg);
		switch (task.op) {
			case SEARCH:
//				thr->get_tree()->search(task.arg);
				// TODO: Output result.
				break;
			case INSERT:
//				thr->get_tree()->insert_node(task.arg);
				break;
			case DELETE:
//				thr->get_tree()->delete_node(task.arg);
				break;
			default:;
				// TODO: Error handling
		}
	}
	return nullptr;
}

int main() {
	file_reader reader(INPUT_FILE);

	rbtree tree = parse_tree(&reader);
	vector<thread_data *> thread_data_objects = parse_threads(&reader, &tree); // TODO: Rename all these functions.
	parse_tasks(&reader, &thread_data_objects);

	vector<thread *> threads;
	for (thread_data *data : thread_data_objects) {
		threads.push_back(new thread(thread_func, data));
	}

	cout << tree.to_string() << endl;

	thread::parbegin(threads);

	cout << tree.to_string() << endl;

	return 0;
}