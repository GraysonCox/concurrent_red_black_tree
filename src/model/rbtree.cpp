//
// Created by Grayson Cox on 10/28/19.
//

#include "model/rbtree.h"

using namespace std;

bool is_red(rbnode *n);
bool contains(rbnode *root, int key);
void recolor_ancestors(rbnode *n);
rbnode *build_from_preorder(vector<rbnode *> preorder, int *next_index);
rbnode *move_red_left(rbnode *n);
rbnode *move_red_right(rbnode *n);
rbnode *rotate_clockwise(rbnode *n);
rbnode *rotate_counter_clockwise(rbnode *n);
rbnode *balance(rbnode *n);
rbnode *insert_node_rec(rbnode *h, int key);
rbnode *delete_node_rec(rbnode *h, int key);
string to_string_rec(rbnode *root, int space);

/*
 * PUBLIC METHODS
 */

rbtree::rbtree() {
	root = nullptr;
}

rbtree::rbtree(vector<rbnode *> preorder) {
	int index = 0;
	root = build_from_preorder(preorder, &index);
}

bool rbtree::search(int key) {
	return contains(root, key);
}

void rbtree::insert_node(int key) {
	root = insert_node_rec(root, key);
	root->set_color(BLACK);
}

void rbtree::delete_node(int key) {
	if (!contains(root, key)) {
		return;
	}

	if (!is_red(root->get_left()) && is_red(root->get_right())) {
		root->set_color(RED);
	}

	root = delete_node_rec(root, key);
	if (root != nullptr) {
		root->set_color(BLACK);
	}
}

std::string rbtree::to_string() {
	return to_string_rec(root, 0);
}

/*
 * UTIL FUNCTIONS
 */

bool is_red(rbnode *n) {
	return n != nullptr && n->get_color() == RED;
}

bool contains(rbnode *root, int key) {
	rbnode *n = root;
	while (n != nullptr) {
		if (n->get_key() == key) {
			return true;
		} else if (key < n->get_key()) {
			n = n->get_left();
		} else if (key > n->get_key()) { // TODO: Should this be just 'else'?
			n = n->get_right();
		}
	}
	return false;
}

void recolor_ancestors(rbnode *n) { // TODO: This is ugly.
	n->set_color(n->get_color() == RED ? BLACK : RED);
	n->get_left()->set_color(n->get_left()->get_color() == RED ? BLACK : RED);
	n->get_right()->set_color(n->get_right()->get_color() == RED ? BLACK : RED);
}

rbnode *build_from_preorder(vector<rbnode *> preorder, int *next_index) {
	if (*next_index >= preorder.size())
		return nullptr;
	rbnode *n = preorder.at((*next_index)++);
	if (n == nullptr)
		return nullptr;
	n->set_left(build_from_preorder(preorder, next_index));
	n->set_right(build_from_preorder(preorder, next_index));
	return n;
}

rbnode *move_red_left(rbnode *n) {
	recolor_ancestors(n);
	if (is_red(n->get_right()->get_left())) {
		n->set_right(rotate_clockwise(n->get_right()));
		n = rotate_counter_clockwise(n);
		recolor_ancestors(n);
	}
	return n;
}

rbnode *move_red_right(rbnode *n) {
	recolor_ancestors(n);
	if (is_red(n->get_left()->get_left())) {
		n = rotate_clockwise(n);
		recolor_ancestors(n);
	}
	return n;
}

rbnode *rotate_clockwise(rbnode *n) {
	rbnode *x = n->get_left();
	n->set_left(x->get_right());
	x->set_right(n);
	x->set_color(x->get_right()->get_color());
	x->get_right()->set_color(RED);
	return x;
}

rbnode *rotate_counter_clockwise(rbnode *n) {
	rbnode *x = n->get_right();
	n->set_right(x->get_left());
	x->set_left(n);
	x->set_color(x->get_left()->get_color());
	x->get_left()->set_color(RED);
	return x;
}

rbnode *balance(rbnode *n) {
	if (is_red(n->get_right())) {
		n = rotate_counter_clockwise(n);
	}
	if (is_red(n->get_left()) && is_red(n->get_left()->get_left())) {
		n = rotate_clockwise(n);
	}
	if (is_red(n->get_left()) && is_red(n->get_right())) {
		recolor_ancestors(n);
	}
	return n;
}

rbnode *insert_node_rec(rbnode *h, int key) {
	if (h == nullptr) {
		return new rbnode(key, RED);
	}

	if (key < h->get_key()) {
		h->set_left(insert_node_rec(h->get_left(), key));
	} else if (key > h->get_key()) {
		h->set_right(insert_node_rec(h->get_right(), key));
	} else {
		// They're equal.
	}

	if (!is_red(h->get_left()) && is_red(h->get_right())) {
		h = rotate_counter_clockwise(h);
	}
	if (is_red(h->get_left()) && is_red(h->get_left()->get_left())) {
		h = rotate_clockwise(h);
	}
	if (is_red(h->get_left()) && is_red(h->get_right())) {
		recolor_ancestors(h);
	}

	return h;
}

rbnode *delete_node_rec(rbnode *h, int key) {
	if (key < h->get_key()) {
		if (!is_red(h->get_left()) && !is_red(h->get_left()->get_left())) {
			h = move_red_left(h);
		}
		h->set_left(delete_node_rec(h->get_left(), key));
	} else {
		if (is_red(h->get_left())) {
			h = rotate_clockwise(h);
		}
		if (key == h->get_key() && h->get_right() == nullptr) {
			return nullptr;
		}
		if (!is_red(h->get_right()) && !is_red(h->get_right()->get_right())) {
			h = move_red_right(h);
		}
		if (key == h->get_key()) {
			// TODO: Remove the min node from right subtree and put it where h is.
		} else {
			h->set_right(delete_node_rec(h->get_right(), key));
		}
	}
	return balance(h);
}

string to_string_rec(rbnode *root, int space) {
	if (root == nullptr) {
		return "";
	}
	string str = to_string_rec(root->get_right(), space + 10);
	str += "\n";
	for (int i = 0; i < space; i++) {
		str += " ";
	}
	str += ::to_string(root->get_key());
	str += root->get_color() == RED ? "r" : "b";
	str += "\n";
	str += to_string_rec(root->get_left(), space + 10);
	return str;
}