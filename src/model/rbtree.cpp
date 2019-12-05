//
// Created by Grayson Cox on 10/28/19.
//

#include "model/rbtree.h"

using namespace std;

rbtree::rbtree() {
	parent_of_root = new rbnode();
	root = new rbnode();
	root->set_parent(parent_of_root);
}

rbtree::rbtree(vector<rbnode *> *preorder) {
	vector<rbnode *>::size_type index = 0;
	root = build_from_preorder(preorder, &index);
	parent_of_root = new rbnode();
	root->set_parent(parent_of_root);
}

rbtree::~rbtree() {
	delete parent_of_root;
	auto *nodes = new vector<rbnode *>();
	nodes->push_back(root);
	for (auto n : *nodes) {
		if (n == nullptr)
			continue;
		if (n->get_left() != nullptr)
			nodes->push_back(n->get_left());
		if (n->get_right() != nullptr)
			nodes->push_back(n->get_right());
		delete n;
	}
	delete nodes;
}

bool rbtree::search(int key) {
	rbnode *x = parent_of_root;
	x->read_lock();
	rbnode *y = root;
	y->read_lock();
	while (!y->is_nil() && y->get_key() != key) {
		x->read_unlock();
		x = y;
		if (key < y->get_key()) {
			y = y->get_left();
		} else {
			y = y->get_right();
		}
		y->read_lock();
	}
	bool is_found = y->get_key() == key;
	x->read_unlock();
	y->read_unlock();
	return is_found;
}

void rbtree::insert_node(int key) {
	rbnode *z = new rbnode(key, RED);
	z->set_left(new rbnode());
	z->set_right(new rbnode());
	z->get_left()->set_parent(z);
	z->get_right()->set_parent(z);
	queue<rbnode *> locked_nodes;
	rbnode *y = parent_of_root;
	y->write_lock();
	locked_nodes.push(y);
	rbnode *x = root;
	x->write_lock();
	while (!x->is_nil()) {
		locked_nodes.push(x);
		y = x;
		if (z->get_key() < x->get_key()) {
			x = x->get_left();
		} else {
			x = x->get_right();
		}
		x->write_lock();
		if (y->get_color() == BLACK && x->get_color() == BLACK) {
			// If two consecutive nodes in the path are black, all preceding nodes can be unlocked.
			while (locked_nodes.front() != y) {
				locked_nodes.front()->write_unlock();
				locked_nodes.pop();
			}
		}
	}
	x->write_unlock();
	delete x;
	z->write_lock();
	locked_nodes.push(z);
	z->set_parent(y);
	if (y->is_nil()) {
		root = z;
	} else if (z->get_key() < y->get_key()) {
		y->set_left(z);
	} else {
		y->set_right(z);
	}
	insert_fixup(z);
	if (locked_nodes.front() == parent_of_root || locked_nodes.front() == root) {
		root->set_color(BLACK);
	}
	while (!locked_nodes.empty()) {
		locked_nodes.front()->write_unlock();
		locked_nodes.pop();
	}
}

void rbtree::delete_node(int key) {
	queue<rbnode *> locked_nodes;
	rbnode *z = parent_of_root;
	z->write_lock();
	locked_nodes.push(z);
	z = root;
	z->write_lock();
	locked_nodes.push(z);
	while (!z->is_nil() && z->get_key() != key) {
		if (key < z->get_key()) {
			z = z->get_left();
		} else {
			z = z->get_right();
		}
		z->write_lock();
		locked_nodes.push(z);
		// TODO: Is this the tightest bound required for restructuring?
		if (z->get_parent()->get_color() == RED && z->get_color() == RED) {
			// If two consecutive nodes in the path are red, all preceding nodes can be unlocked, I think.
			while (locked_nodes.front() != z->get_parent()) {
				locked_nodes.front()->write_unlock();
				locked_nodes.pop();
			}
		}
	}
	if (z->is_nil()) { // If key is not found, unlock all nodes and return.
		while (!locked_nodes.empty()) {
			locked_nodes.front()->write_unlock();
			locked_nodes.pop();
		}
		return;
	}
	rbnode *y = z;
	rbnode *x = nullptr;
	rbnode_color y_original_color = y->get_color();
	if (z->get_left()->is_nil()) {
		x = z->get_right();
		x->write_lock();
		locked_nodes.push(x);
		transplant(z, z->get_right());
	} else if (z->get_right()->is_nil()) {
		x = z->get_left(); // TODO: Maybe delete z->get_right().
		x->write_lock();
		locked_nodes.push(x);
		transplant(z, z->get_left());
	} else {
		z->get_right()->write_lock();
		locked_nodes.push(z->get_right());
		y = z->get_right(); // Find min node in subtree at z->get_right().
		while (!z->get_right()->is_nil() && !y->get_left()->is_nil()) {
			y = y->get_left();
			y->write_lock();
			locked_nodes.push(y);
		}
		delete y->get_left(); // Delete the nil node.
		y_original_color = y->get_color();
		x = y->get_right();
		x->write_lock();
		locked_nodes.push(x);
		if (y->get_parent() == z) {
			x->set_parent(y);
		} else {
			transplant(y, y->get_right());
			y->set_right(z->get_right());
			y->get_right()->set_parent(y);
		}
		transplant(z, y);
		z->get_left()->write_lock();
		locked_nodes.push(z->get_left());
		y->set_left(z->get_left());
		y->get_left()->set_parent(y);
		y->set_color(z->get_color());
	}
	if (y_original_color == BLACK) {
		delete_fixup(x);
	}
	while (!locked_nodes.empty()) { // Unlock all nodes.
		locked_nodes.front()->write_unlock();
		locked_nodes.pop(); // TODO: De-allocate z.
	}
}

std::string rbtree::to_string() {
	return to_string_rec(root);
}

std::string rbtree::to_string_pretty() {
	return to_string_pretty_rec(root, 0);
}

/**
 * Recursively builds and returns an rbtree with the given pre-order traversal.
 *
 * @param preorder - A vector of rbnode pointers in prefix order.
 * 					Every leaf node should be given as nullptr.
 * @param next_index - The index of the next entry in preorder.
 * @return An rbtree with the contents of the given pre-order traversal.
 */
rbnode *rbtree::build_from_preorder(vector<rbnode *> *preorder, vector<rbnode *>::size_type *next_index) {
	if (*next_index >= preorder->size())
		return new rbnode();
	rbnode *n = preorder->at((*next_index)++);
	if (n == nullptr)
		return new rbnode();
	n->set_left(build_from_preorder(preorder, next_index));
	n->get_left()->set_parent(n);
	n->set_right(build_from_preorder(preorder, next_index));
	n->get_right()->set_parent(n);
	return n;
}

/**
 * Calling thread must have lock on z and z->get_parent().
 *
 * @param z - The newly inserted node.
 */
void rbtree::insert_fixup(rbnode *z) {
	rbnode *y;
	while (z->get_parent()->get_color() == RED) {
		if (z->get_parent() ==
			z->get_parent()->get_parent()->get_left()) { // z's parent is left child. TODO: Is it okay not to lock z's uncle?
			y = z->get_parent()->get_parent()->get_right();
			y->write_lock();
			if (y->get_color() == RED) {
				z->get_parent()->set_color(BLACK);
				y->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				z = z->get_parent()->get_parent();
				y->write_unlock();
			} else {
				y->write_unlock();
				if (z == z->get_parent()->get_right()) { // z is right child.
					z = z->get_parent();
					rotate_counter_clockwise(z);
				}
				z->get_parent()->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				rotate_clockwise(z->get_parent()->get_parent());
			}
		} else { // Same as previous clause, but with 'right' and 'left' exchanged.
			y = z->get_parent()->get_parent()->get_left();
			y->write_lock();
			if (y->get_color() == RED) {
				z->get_parent()->set_color(BLACK);
				y->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				z = z->get_parent()->get_parent();
				y->write_unlock();
			} else {
				y->write_unlock();
				if (z == z->get_parent()->get_left()) {
					z = z->get_parent();
					rotate_clockwise(z);
				}
				z->get_parent()->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				rotate_counter_clockwise(z->get_parent()->get_parent());
			}
		}
	}
}

/**
 * Calling thread must have mutex lock on x and all ancestors involved
 * in the restructuring.
 *
 * @param x - The node that took the place of the deleted node.
 */
void rbtree::delete_fixup(rbnode *x) {
	rbnode *w;
	while (x->get_parent() != parent_of_root && x->get_color() == BLACK) {
		if (x == x->get_parent()->get_left()) {
			w = x->get_parent()->get_right();
			w->write_lock();
			if (w->get_color() == RED) {
				w->set_color(BLACK);
				x->get_parent()->set_color(RED);
				rotate_counter_clockwise(x->get_parent());
				w->write_unlock();
				w = x->get_parent()->get_right();
				w->write_lock();
			}
			w->get_left()->write_lock();
			w->get_right()->write_lock();
			if (w->get_left()->get_color() == BLACK && w->get_right()->get_color() == BLACK) {
				w->set_color(RED);
				x = x->get_parent();
				w->get_left()->write_unlock();
				w->get_right()->write_unlock();
				w->write_unlock();
			} else {
				if (w->get_right()->get_color() == BLACK) {
					w->get_left()->set_color(BLACK);
					w->set_color(RED);
					rotate_clockwise(w);
					w->get_left()->write_unlock();
					w->get_right()->write_unlock();
					w->write_unlock();
					w = x->get_parent()->get_right();
					w->write_lock();
					w->get_left()->write_lock();
					w->get_right()->write_lock();
				}
				w->set_color(x->get_parent()->get_color());
				x->get_parent()->set_color(BLACK);
				w->get_right()->set_color(BLACK);
				w->get_left()->write_unlock();
				rotate_counter_clockwise(x->get_parent());
				w->get_right()->write_unlock();
				w->write_unlock();
				return;
			}
		} else { // Same as previous clause, but with 'right' and 'left' exchanged
			w = x->get_parent()->get_left();
			w->write_lock();
			if (w->get_color() == RED) {
				w->set_color(BLACK);
				x->get_parent()->set_color(RED);
				rotate_clockwise(x->get_parent());
				w->write_unlock();
				w = x->get_parent()->get_left();
				w->write_lock();
			}
			w->get_left()->write_lock();
			w->get_right()->write_lock();
			if (w->get_left()->get_color() == BLACK && w->get_right()->get_color() == BLACK) {
				w->set_color(RED);
				x = x->get_parent();
				w->get_left()->write_unlock();
				w->get_right()->write_unlock();
				w->write_unlock();
			} else {
				if (w->get_left()->get_color() == BLACK) {
					w->get_right()->set_color(BLACK);
					w->set_color(RED);
					rotate_counter_clockwise(w);
					w->get_left()->write_unlock();
					w->get_right()->write_unlock();
					w->write_unlock();
					w = x->get_parent()->get_left();
					w->write_lock();
					w->get_left()->write_lock();
					w->get_right()->write_lock();
				}
				w->set_color(x->get_parent()->get_color());
				x->get_parent()->set_color(BLACK);
				w->get_left()->set_color(BLACK);
				w->get_right()->write_unlock();
				rotate_clockwise(x->get_parent());
				w->get_left()->write_unlock();
				w->write_unlock();
				return;
			}
		}
	}
	x->set_color(BLACK);
}

/**
 * Calling thread must have mutex lock on x, x->get_parent(), and x->get_left().
 *
 * @param x - The node that will be rotated to take the place of its right child.
 */
void rbtree::rotate_clockwise(rbnode *x) {
	rbnode *y = x->get_left();
	y->get_right()->write_lock();
	x->set_left(y->get_right());
	if (!y->get_right()->is_nil()) {
		y->get_right()->set_parent(x);
	}
	y->get_right()->write_unlock();
	y->set_parent(x->get_parent());
	if (x->get_parent()->is_nil()) {
		root = y;
	} else if (x == x->get_parent()->get_right()) {
		x->get_parent()->set_right(y);
	} else {
		x->get_parent()->set_left(y);
	}
	y->set_right(x);
	x->set_parent(y);
}

/**
 * Calling thread must have mutex lock on x, x->get_parent(), and x->get_right().
 *
 * @param x - The node that will be rotated to take the place of its left child.
 */
void rbtree::rotate_counter_clockwise(rbnode *x) {
	rbnode *y = x->get_right();
	y->get_left()->write_lock();
	x->set_right(y->get_left());
	if (!y->get_left()->is_nil()) {
		y->get_left()->set_parent(x);
	}
	y->get_left()->write_unlock();
	y->set_parent(x->get_parent());
	if (x->get_parent()->is_nil()) {
		root = y;
	} else if (x == x->get_parent()->get_left()) {
		x->get_parent()->set_left(y);
	} else {
		x->get_parent()->set_right(y);
	}
	y->set_left(x);
	x->set_parent(y);
}

/**
 * Replaces the subtree rooted at u with the subtree rooted at v.
 * Calling thread must have lock on u, u->get_parent(), v, and maybe v->get_parent(). // TODO
 *
 * @param u - The node to be replaced by v.
 * @param v - The node to take the place of u.
 */
void rbtree::transplant(rbnode *u, rbnode *v) {
	if (u->get_parent()->is_nil()) {
		root = v; // TODO: Is it okay for the root to be null?
	} else if (u == u->get_parent()->get_left()) {
		u->get_parent()->set_left(v);
	} else {
		u->get_parent()->set_right(v);
	}
	if (v != nullptr) { // TODO: Do I need this check?
		v->set_parent(u->get_parent());
	}
}

/**
 * Recursively creates a string representing a pre-order traversal
 * of the red-black tree rooted at the given node.
 *
 * @param root - The root of the subtree to convert to a string.
 * @return A string representing a pre-order traversal of the
 * 			red-black tree rooted at the given node.
 */
string rbtree::to_string_rec(rbnode *root) {
	if (root->is_nil())
		return "f";
	string str = std::to_string(root->get_key());
	str += root->get_color() == RED ? "r" : "b";
	str += ",";
	str += to_string_rec(root->get_left());
	str += ",";
	str += to_string_rec(root->get_right());
	return str;
}

/**
 * Recursively creates a string representation of the red-black tree
 * rooted at the given node by doing an in-order traversal.
 *
 * @param root - The root of the subtree to traverse.
 * @param space - The number of tabs to indent the current node.
 * @return A string representation of the subtree rooted at root.
 */
string rbtree::to_string_pretty_rec(rbnode *root, int space) {
	if (root->is_nil()) {
		return "";
	}
	string str = to_string_pretty_rec(root->get_right(), space + 2);
	str += "\n";
	for (int i = 0; i < space; i++) {
		str += "\t";
	}
	str += ::to_string(root->get_key());
	str += root->get_color() == RED ? "r" : "b";
	str += "\n";
	str += to_string_pretty_rec(root->get_left(), space + 2);
	return str;
}