//
// Created by Grayson Cox on 12/1/19.
//

#include "io/input_file_parsing.h"

using namespace std;

rbtree *parse_tree(file_reader *reader) {
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
	return new rbtree(nodes);
}

vector<thread_data *> parse_thread_names(file_reader *reader) {
	vector<thread_data *> threads;
	string name = reader->read_line();
	name = reader->read_line();
	while (name != "") {
		threads.push_back(new thread_data(name, new vector<task_t>()));
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