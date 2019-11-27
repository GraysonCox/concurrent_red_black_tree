//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <io/file_writer.h>
#include "io/file_reader.h"
#include <model/rbtree.h>
#include <sstream>
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

vector<thread *> parse_threads(file_reader *reader, rbtree *t) {
	vector<thread *> threads;
	string name = reader->read_line();
	name = reader->read_line();
	while (name != "") {
		threads.push_back(new thread(name, t));
		name = reader->read_line();
	}
	return threads;
}

void parse_tasks(file_reader *reader, vector<thread *> *threads) {
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
		for (thread *t : *threads) {
			if (t->get_name() == thread_name) {
				t->add_task(task);
				break;
			}
		}

		if (!reader->is_end_of_file()) {
			reader->read_token();
		}
	}
}

int main() {
	file_reader reader(INPUT_FILE);

	rbtree t = parse_tree(&reader);
	vector<thread *> threads = parse_threads(&reader, &t);
	parse_tasks(&reader, &threads);

	cout << t.to_string() << endl;

	parbegin(threads);

	cout << t.to_string() << endl;

	return 0;
}