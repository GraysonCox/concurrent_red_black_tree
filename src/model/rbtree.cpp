//
// Created by Grayson Cox on 10/28/19.
//

#include "model/rbtree.h"

using namespace std;

bool is_red(rbnode *n);
rbnode *find_node(rbtree *tree, int key);
rbnode *find_min_node(rbtree *tree, rbnode *n);
rbnode *build_from_preorder(vector<rbnode *> preorder, int *next_index, rbnode *nil);
void insert_fixup(rbtree *tree, rbnode *z);
void delete_fixup(rbtree *tree, rbnode *x);
void rotate_clockwise(rbtree *tree, rbnode *x);
void rotate_counter_clockwise(rbtree *tree, rbnode *x);
void transplant(rbtree *tree, rbnode *u, rbnode *v);
string to_string_rec(rbnode *root, int space, rbnode *nil);

/*
 * PUBLIC METHODS
 */

rbtree::rbtree() {
	root = nullptr;
	nil = new rbnode(-1, BLACK);
}

rbtree::rbtree(vector<rbnode *> preorder) {
	int index = 0;
	nil = new rbnode(-1, BLACK);
	root = build_from_preorder(preorder, &index, nil);
	root->set_parent(nil);
}

bool rbtree::search(int key) {
	return find_node(this, key) != nil;
}

void rbtree::insert_node(int key) {
	rbnode *z = new rbnode(key, RED);
	z->set_left(nil);
	z->set_right(nil);
	rbnode *y = nil;
	rbnode *x = root;
	while (x != nil) {
		y = x;
		if (z->get_key() < x->get_key()) {
			x = x->get_left();
		} else {
			x = x->get_right();
		}
	}
	z->set_parent(y);
	if (y == nil) {
		root = z;
	} else if (z->get_key() < y->get_key()) {
		y->set_left(z);
	} else {
		y->set_right(z);
	}
	insert_fixup(this, z);
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
	return to_string_rec(root, 0, nil);
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

rbnode *build_from_preorder(vector<rbnode *> preorder, int *next_index, rbnode *nil) {
	if (*next_index >= preorder.size())
		return nil;
	rbnode *n = preorder.at((*next_index)++);
	if (n == nullptr)
		return nil;
	n->set_left(build_from_preorder(preorder, next_index, nil));
	n->get_left()->set_parent(n);
	n->set_right(build_from_preorder(preorder, next_index, nil));
	n->get_right()->set_parent(n);
	return n;
}

void insert_fixup(rbtree *tree, rbnode *z) {
	rbnode *y;
	while (z->get_parent()->get_color() == RED) {
		if (z->get_parent() == z->get_parent()->get_parent()->get_left()) {
			y = z->get_parent()->get_parent()->get_right();
			if (is_red(y)) {
				z->get_parent()->set_color(BLACK);
				y->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				z = z->get_parent()->get_parent();
			} else {
				if (z == z->get_parent()->get_right()) {
					z = z->get_parent();
					rotate_counter_clockwise(tree, z);
				}
				z->get_parent()->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				rotate_clockwise(tree, z->get_parent()->get_parent());
			}
		} else { // Same as previous clause, but with 'right' and 'left' exchanged
			y = z->get_parent()->get_parent()->get_left();
			if (is_red(y)) {
				z->get_parent()->set_color(BLACK);
				y->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				z = z->get_parent()->get_parent();
			} else {
				if (z == z->get_parent()->get_left()) {
					z = z->get_parent();
					rotate_clockwise(tree, z);
				}
				z->get_parent()->set_color(BLACK);
				z->get_parent()->get_parent()->set_color(RED);
				rotate_counter_clockwise(tree, z->get_parent()->get_parent());
			}
		}
	}
	tree->get_root()->set_color(BLACK);
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

void rotate_clockwise(rbtree *tree, rbnode *x) {
	rbnode *y = x->get_left();
	x->set_left(y->get_right());
	if (y->get_right() != tree->get_nil()) {
		y->get_right()->set_parent(x);
	}
	y->set_parent(x->get_parent());
	if (x->get_parent() == tree->get_nil()) {
		tree->set_root(y);
	} else if (x == x->get_parent()->get_right()) {
		x->get_parent()->set_right(y);
	} else {
		x->get_parent()->set_left(y);
	}
	y->set_right(x);
	x->set_parent(y);
}

void rotate_counter_clockwise(rbtree *tree, rbnode *x) {
	rbnode *y = x->get_right();
	x->set_right(y->get_left());
	if (y->get_left() != tree->get_nil()) {
		y->get_left()->set_parent(x);
	}
	y->set_parent(x->get_parent());
	if (x->get_parent() == tree->get_nil()) {
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

string to_string_rec(rbnode *root, int space, rbnode *nil) {
	if (root == nil) {
		return "";
	}
	string str = to_string_rec(root->get_right(), space + 10, nil);
	str += "\n";
	for (int i = 0; i < space; i++) {
		str += " ";
	}
	str += ::to_string(root->get_key());
	str += root->get_color() == RED ? "r" : "b";
	str += "\n";
	str += to_string_rec(root->get_left(), space + 10, nil);
	return str;
}