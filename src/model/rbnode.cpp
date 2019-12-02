//
// Created by Grayson Cox on 11/20/19.
//

#include <cstdio>
#include <cerrno>
#include "model/rbnode.h"

rbnode::rbnode(int key, rbnode_color color) : key(key), color(color) {
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	is_mutex_locked = false;
	is_nil_node = false;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m, &attr);
}

rbnode::rbnode() {
	key = -1;
	color = BLACK;
	left = nullptr;
	right = nullptr;
	parent = nullptr;
	is_mutex_locked = false;
	is_nil_node = true;
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m, &attr);
}

rbnode::~rbnode() {
	pthread_mutex_destroy(&m);
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

void rbnode::lock() {
	printf("%d: Locking node %d\n", pthread_self(), key); // TODO: Remove when done debugging
	int error = pthread_mutex_lock(&m);
	if (error != 0) {
		// TODO: Error handling
	}
	is_mutex_locked = true;
}

void rbnode::unlock() {
	printf("%d: Unlocking node %d\n", pthread_self(), key); // TODO: Remove when done debugging
	is_mutex_locked = false;
	int error = pthread_mutex_unlock(&m);
	if (error != 0) {
		// TODO: Error handling
	}
}

bool rbnode::is_nil() {
	return is_nil_node;
}
