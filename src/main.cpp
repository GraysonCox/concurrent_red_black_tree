//
// Created by Grayson Cox on 10/28/19.
//

#include <iostream>
#include <io/file_writer.h>
#include "io/file_reader.h"
#include <model/rbtree.h>

using namespace std;

const string INPUT_FILE = "/Users/administrator/Documents/ISU/COMS352/PA2/Grayson_Cox_Project2/src/input.txt";

typedef enum {
	SEARCH,
	INSERT,
	DELETE,
	UNKNOWN
} operation_t;

void insert_test(initializer_list<int> keys) {
	rbtree t;
	for (auto k : keys) {
		cout << "Inserting " << k << " ..." << endl;
		t.insert_node(k);
		cout << t.to_string() << endl;
	}
	for (auto k : keys) {
		int x = k + random() % 2;
		cout << "Searching for " << x << " ... ";
		if (t.search(x)) {
			cout << "found" << endl;
		} else {
			cout << "not found" << endl;
		}
	}
}

template<typename T>
vector<T> to_vector(initializer_list<T> list) {
	vector<T> v;
	for (auto t : list) {
		v.push_back(t);
	}
	return v;
}

//
// 45b,1b,f,34r,f,f,97b,f,110r,f,f
//

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

	return 0;
}