//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <io/file_writer.h>
#include "io/file_reader.h"
#include <model/rbtree.h>
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

int main() {
	initializer_list<rbnode *> nodes = {
			new rbnode(45, BLACK),
			new rbnode(1, BLACK),
			nullptr,
			new rbnode(34, RED),
			nullptr,
			nullptr,
			new rbnode(97, BLACK),
			nullptr,
			new rbnode(110, RED),
			nullptr,
			nullptr
	};
	rbtree t(to_vector(nodes));
	cout << t.to_string() << endl;

	initializer_list<thread *> threads = {
			new thread("thread1", &t),
			new thread("thread2", &t),
			new thread("thread3", &t)
	};
	for (auto thr : threads) {
		for (int i = 0; i < 3; i++) {
			task_t task;
			task.op = (operation_t)(random() % 3);
			task.arg = random() % 25;
			thr->add_task(task);
		}
	}

	parbegin(to_vector(threads));

	cout << t.to_string() << endl;

	return 0;
}