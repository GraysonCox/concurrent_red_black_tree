//
// Created by Grayson Cox on 11/20/19.
//

#include "model/rbnode.h"

rbnode::rbnode(int key, rbnode_color color) : key(key), color(color) {
	left = nullptr;
	right = nullptr;
	parent = nullptr;
}

int rbnode::get_key() const {
	return key;
}

void rbnode::set_key(int key) {
	rbnode::key = key;
}

rbnode_color rbnode::get_color() const {
	return color;
}

void rbnode::set_color(rbnode_color color) {
	rbnode::color = color;
}

rbnode *rbnode::get_left() const {
	return left;
}

void rbnode::set_left(rbnode *n) {
	rbnode::left = n;
}

rbnode *rbnode::get_right() const {
	return right;
}

void rbnode::set_right(rbnode *n) {
	rbnode::right = n;
}

rbnode *rbnode::get_parent() const {
	return parent;
}

void rbnode::set_parent(rbnode *n) {
	rbnode::parent = n;
}
