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
	return new rbtree(&nodes);
}

int parse_num_threads(file_reader *reader) {
	reader->read_token();
	reader->read_token();
	string str = reader->read_token();
	int num_threads = stoi(str);
	return num_threads;
}

void parse_tasks(file_reader *reader, queue<operation> *read_tasks, queue<operation> *write_tasks) {
	string str;
	operation *task;
	while (!reader->is_end_of_file()) {
		str = reader->read_token();
		if (str == "||")
			continue;

		task = new operation(str);
		switch (task->op) {
			case SEARCH:
				read_tasks->push(*task);
				break;
			case INSERT:
			case DELETE:
				write_tasks->push(*task);
				break;
			default:
				cerr << "Invalid operation: " + str << endl;
		}
	}
}