//
// Created by Grayson Cox on 10/28/19.
//

#include <queue>
#include "model/rbtree.h"

using namespace std;

bool is_red(rbnode *n);
rbnode *find_node(rbtree *tree, int key);
rbnode *find_min_node(rbtree *tree, rbnode *n);
rbnode *build_from_preorder(vector<rbnode *> preorder, int *next_index);
void insert_fixup(rbtree *tree, rbnode *z);
void delete_fixup(rbtree *tree, rbnode *x);
void rotate_clockwise(rbtree *tree, rbnode *x);
void rotate_counter_clockwise(rbtree *tree, rbnode *x);
void transplant(rbtree *tree, rbnode *u, rbnode *v);
string to_string_rec(rbnode *root, int space);

/*
 * PUBLIC METHODS
 */

rbtree::rbtree() {
	parent_of_root = new rbnode();
	root = nullptr;
	nil = new rbnode(-1, BLACK);
}

rbtree::rbtree(vector<rbnode *> preorder) {
	int index = 0;
	nil = new rbnode(-1, BLACK);
	root = build_from_preorder(preorder, &index);
	parent_of_root = new rbnode();
	root->set_parent(parent_of_root);
}

bool rbtree::search(int key) {
	return find_node(this, key) != nil;
}

void rbtree::insert_node(int key) {
	rbnode *z = new rbnode(key, RED);
	z->set_left(new rbnode());
	z->set_right(new rbnode());

	queue<rbnode *> locked_nodes;
	rbnode *y = parent_of_root;
	y->lock();
	locked_nodes.push(y);
	rbnode *x = root;
	x->lock();
//	locked_nodes.push(x);
	while (!x->is_nil()) {
		locked_nodes.push(x);
		y = x;
		if (z->get_key() < x->get_key()) {
			x = x->get_left();
		} else {
			x = x->get_right();
		}
		x->lock();
//		locked_nodes.push(x);
		if (y->get_color() == BLACK && x->get_color() == BLACK) {
			// If two consecutive nodes in the path are black, all preceding nodes can be unlocked.
			while (locked_nodes.front() != y) {
				locked_nodes.front()->unlock();
				locked_nodes.pop();
			}
		}
	}
	x->unlock();
	delete x;
	z->lock();
	locked_nodes.push(z);
	z->set_parent(y);
	if (y->is_nil()) {
		root = z;
	} else if (z->get_key() < y->get_key()) {
		y->set_left(z);
	} else {
		y->set_right(z);
	}
	insert_fixup(this, z);
	if (locked_nodes.front() == parent_of_root || locked_nodes.front() == root) {
		root->set_color(BLACK);
	}
	while (!locked_nodes.empty()) {
		locked_nodes.front()->unlock();
		locked_nodes.pop();
	}
}

void rbtree::delete_node(int key) {
	rbnode *z = find_node(this, key);
	if (z == nil || z == nullptr) // TODO: Do I need both?
		return;
	rbnode *y = z;
	rbnode *x = nullptr;
	rbnode_color y_original_color = y->get_color();
	if (z->get_left() == nil) {
		x = z->get_right();
		transplant(this, z, z->get_right());
	} else if (z->get_right() == nil) {
		x = z->get_left();
		transplant(this, z, z->get_left());
	} else {
		y = find_min_node(this, z->get_right());
		y_original_color = y->get_color();
		x = y->get_right();
		if (y->get_parent() == z) {
			x->set_parent(y);
		} else {
			transplant(this, y, y->get_right());
			y->set_right(z->get_right());
			y->get_right()->set_parent(y);
		}
		transplant(this, z, y);
		y->set_left(z->get_left());
		y->get_left()->set_parent(y);
		y->set_color(z->get_color());
	}
	if (y_original_color == BLACK) {
		delete_fixup(this, x);
	}
}

std::string rbtree::to_string() {
	return to_string_rec(root, 0);
}

rbnode *rbtree::get_root() const {
	return root;
}

void rbtree::set_root(rbnode *root) {
	rbtree::root = root;
}

rbnode *rbtree::get_nil() const {
	return nil;
}

/*
 * UTIL FUNCTIONS
 */

bool is_red(rbnode *n) {
	return n != nullptr && n->get_color() == RED; // TODO: Maybe not allow nullptr since using nil.
}

rbnode *find_node(rbtree *tree, int key) {
	rbnode *n = tree->get_root();
	while (n != tree->get_nil()) {
		if (n->get_key() == key) {
			return n;
		} else if (key < n->get_key()) {
			n = n->get_left();
		} else if (key > n->get_key()) { // TODO: Should this be just 'else'?
			n = n->get_right();
		}
	}
	return tree->get_nil();
}

rbnode *find_min_node(rbtree *tree, rbnode *n) {
	if (n == nullptr || n == tree->get_nil()) {
		return tree->get_nil();
	}
	rbnode *x = n;
	while (x->get_left() != nullptr && x->get_left() != tree->get_nil()) {
		x = x->get_left();
	}
	return x;
}

rbnode *build_from_preorder(vector<rbnode *> preorder, int *next_index) {
	if (*next_index >= preorder.size())
		return new rbnode();
	rbnode *n = preorder.at((*next_index)++);
	if (n == nullptr)
		return new rbnode();
	n->set_left(build_from_preorder(preorder, next_index));
	n->get_left()->set_parent(n);
	n->set_right(build_from_preorder(preorder, next_index));
	n->get_right()->set_parent(n);
	return n;
}

/**
 * Calling thread must have lock on z and z->get_parent() - maybe not anymore
 * @param tree
 * @param z
 */
void insert_fixup(rbtree *tree, rbnode *z) {
	rbnode *y;
	while (z->get_parent()->get_color() == RED) {
//		z->get_parent()->get_parent()->lock();
		if (z->get_parent() == z->get_parent()->get_parent()->get_left()) { // z's parent is left child TODO: Is it okay not to lock z's uncle?
			y = z->get_parent()->get_parent()->get_right();
			y->lock();
			if (is_red(y)) {
				z->get_parent()->set_color(BLACK);
				y->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
//				rbnode *tmp = z;
//				z = z->get_parent();
//				tmp->unlock(); // Unlock z
//				tmp = z;
//				z = z->get_parent();
//				tmp->unlock(); // Unlock z's parent, and now z's grandparent is the new z
//				y->unlock();
//				// Now, only z's grandparent (which is the new z) is locked.
//				z->get_parent()->lock();
				z = z->get_parent()->get_parent();
				y->unlock();
			} else {
				y->unlock();
				if (z == z->get_parent()->get_right()) { // z is right child
					z = z->get_parent();
					rotate_counter_clockwise(tree, z);
				}
				z->get_parent()->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
//				z->get_parent()->get_parent()->get_parent()->lock();
				rotate_clockwise(tree, z->get_parent()->get_parent());
//				z->get_parent()->get_right()->unlock();
//				z->get_parent()->get_parent()->unlock();
//				// Now the new z and its parent are locked.

			}
		} else { // Same as previous clause, but with 'right' and 'left' exchanged
			y = z->get_parent()->get_parent()->get_left();
			y->lock();
			if (is_red(y)) {
				z->get_parent()->set_color(BLACK);
				y->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				z = z->get_parent()->get_parent();
				y->unlock();
			} else {
				y->unlock();
				if (z == z->get_parent()->get_left()) {
					z = z->get_parent();
					rotate_clockwise(tree, z);
				}
				z->get_parent()->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
//				z->get_parent()->get_parent()->get_right()->lock();
				rotate_counter_clockwise(tree, z->get_parent()->get_parent());
			}
		}
	}
//	z->get_parent()->unlock();
//	z->unlock();
//	tree->get_root()->lock();
//	tree->get_root()->set_color(BLACK);
//	tree->get_root()->unlock();
}

void delete_fixup(rbtree *tree, rbnode *x) {
	rbnode *w;
	while (x != tree->get_root() && !is_red(x)) {
		if (x == x->get_parent()->get_left()) {
			w = x->get_parent()->get_right();
			if (is_red(w)) {
				w->set_color(BLACK);
				x->get_parent()->set_color(RED);
				rotate_counter_clockwise(tree, x->get_parent());
				w = x->get_parent()->get_right();
			}
			if (!is_red(w->get_left()) && !is_red(w->get_right())) {
				w->set_color(RED);
				x = x->get_parent();
			} else {
				if (!is_red(w->get_right())) {
					w->get_left()->set_color(BLACK);
					w->set_color(RED);
					rotate_clockwise(tree, w);
					w = x->get_parent()->get_right();
				}
				w->set_color(x->get_parent()->get_color());
				x->get_parent()->set_color(BLACK);
				w->get_right()->set_color(BLACK);
				rotate_counter_clockwise(tree, x->get_parent());
				x = tree->get_root();
			}
		} else { // Same as previous clause, but with 'right' and 'left' exchanged
			w = x->get_parent()->get_left();
			if (is_red(w)) {
				w->set_color(BLACK);
				x->get_parent()->set_color(RED);
				rotate_clockwise(tree, x->get_parent());
				w = x->get_parent()->get_left();
			}
			if (!is_red(w->get_left()) && !is_red(w->get_right())) {
				w->set_color(RED);
				x = x->get_parent();
			} else {
				if (!is_red(w->get_left())) {
					w->get_right()->set_color(BLACK);
					w->set_color(RED);
					rotate_counter_clockwise(tree, w);
					w = x->get_parent()->get_left();
				}
				w->set_color(x->get_parent()->get_color());
				x->get_parent()->set_color(BLACK);
				w->get_left()->set_color(BLACK);
				rotate_clockwise(tree, x->get_parent());
				x = tree->get_root();
			}
		}
	}
	x->set_color(BLACK);
}

/**
 * Calling thread must have mutex lock on x, x->get_parent(), and x->get_left()
 *
 * // TODO: This may need to rotate just the keys instead of the pointers.
 *
 * @param tree
 * @param x
 */
void rotate_clockwise(rbtree *tree, rbnode *x) {
	rbnode *y = x->get_left();
	y->get_right()->lock();
	x->set_left(y->get_right());
	if (!y->get_right()->is_nil()) {
		y->get_right()->set_parent(x);
	}
	y->get_right()->unlock();
	y->set_parent(x->get_parent());
	if (x->get_parent()->is_nil()) {
		tree->set_root(y);
	} else if (x == x->get_parent()->get_right()) {
		x->get_parent()->set_right(y);
	} else {
		x->get_parent()->set_left(y);
	}
	y->set_right(x);
	x->set_parent(y);
}

/**
 * Calling thread must have mutex lock on x, x->get_parent(), and x->get_right()
 *
 * // TODO: This may need to rotate just the keys instead of the pointers.
 *
 * @param tree
 * @param x
 */
void rotate_counter_clockwise(rbtree *tree, rbnode *x) {
	rbnode *y = x->get_right();
	y->get_left()->lock();
	x->set_right(y->get_left());
	if (!y->get_left()->is_nil()) {
		y->get_left()->set_parent(x);
	}
	y->get_left()->unlock();
	y->set_parent(x->get_parent());
	if (x->get_parent()->is_nil()) {
		tree->set_root(y);
	} else if (x == x->get_parent()->get_left()) {
		x->get_parent()->set_left(y);
	} else {
		x->get_parent()->set_right(y);
	}
	y->set_left(x);
	x->set_parent(y);
}

void transplant(rbtree *tree, rbnode *u, rbnode *v) {
	if (u->get_parent() == tree->get_nil()) {
		tree->set_root(v); // TODO: Is it okay for the root to be null?
	} else if (u == u->get_parent()->get_left()) {
		u->get_parent()->set_left(v);
	} else {
		u->get_parent()->set_right(v);
	}
	if (v != nullptr) { // TODO: Do I need this check?
		v->set_parent(u->get_parent());
	}
}

string to_string_rec(rbnode *root, int space) {
	if (root->is_nil()) {
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

/* EDGE CASE THAT CAUSES DEADLOCK:

                    110r

          97b

45b

                    34r

          1b

thread2: insert(100)
thread3: insert(210)
thread1: insert(111)
186114048: Locking node -1
186650624: Locking node -1
187187200: Locking node -1
186114048: Locking node 45
186114048: Locking node 97
186114048: Unlocking node -1
186114048: Locking node 110
186114048: Locking node -1
186114048: Unlocking node -1
186650624: Locking node 45
186114048: Locking node 111
186114048: Locking node -1
186114048: Unlocking node -1
186114048: Locking node -1
186114048: Unlocking node -1
186114048: Unlocking node 45
186114048: Unlocking node 97
186114048: Unlocking node 110
186114048: Unlocking node 111
186650624: Locking node 110
186650624: Unlocking node -1
thread1: Done

                    111r

          110b

                    97r

45b*

                    34r

          1b

187187200: Locking node 45
thread1: insert(420)
186114048: Locking node -1
186650624: Locking node 97
186650624: Locking node -1
186650624: Unlocking node -1
186650624: Locking node 100
186650624: Locking node 111
186650624: Unlocking node 111
186650624: Unlocking node 45
186650624: Unlocking node 110
186650624: Unlocking node 97
186650624: Unlocking node 100
187187200: Locking node 110
187187200: Locking node 111
187187200: Locking node -1
187187200: Unlocking node -1
thread2: Done

                    111b*

          110r*

                              100r

                    97b

45b*

                    34r

          1b

thread2: insert(69)
186650624: Locking node -1
186114048: Locking node 45
187187200: Unlocking node 45
187187200: Unlocking node 110
187187200: Unlocking node -1
186114048: Locking node 110
186114048: Locking node 111
187187200: Locking node 210
187187200: Unlocking node 111
187187200: Unlocking node 210
186114048: Locking node 210
186114048: Locking node -1
186114048: Unlocking node -1
186114048: Locking node 420
186114048: Locking node -1
186114048: Unlocking node -1
186114048: Locking node -1
186114048: Unlocking node -1
186114048: Unlocking node -1
thread3: Done

                              210r

                    111b*

          110r*

                              100r

                    97b

45b*

                    34r

          1b

186114048: Unlocking node 45
186114048: Unlocking node 110
186114048: Unlocking node 111
186114048: Unlocking node 210
186114048: Unlocking node 420
186650624: Locking node 45
186650624: Locking node 110
186650624: Locking node 97
186650624: Locking node -1
186650624: Unlocking node -1
186650624: Unlocking node 45
186650624: Unlocking node 110
186650624: Unlocking node -1
186650624: Locking node 69
186650624: Unlocking node 97
186650624: Unlocking node 69
thread1: Done

                              420r

                    210b

                              111r

          110r*

                              100r

                    97b*

45b

                    34r

          1b

thread2: Done

                              420r

                    210b

                              111r

          110r

                              100r

                    97b

                              69r

45b

                    34r

          1b

thread2: insert(46)
186650624: Locking node -1
186650624: Locking node 45
186650624: Locking node 110
186650624: Locking node 97
186650624: Locking node 69
186650624: Locking node -1
186650624: Unlocking node -1
186650624: Locking node 46
186650624: Locking node 100
186650624: Unlocking node 100
186650624: Locking node 1
186650624: Unlocking node 1
186650624: Locking node 100
186650624: Unlocking node 100
186650624: Locking node 69

 */