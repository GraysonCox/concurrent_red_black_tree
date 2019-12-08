//
// Created by Grayson Cox on 11/20/19.
//

#include "model/rbnode.h"

rbnode::rbnode(int key, rbnode_color color) : key(key),
											  color(color),
											  left(nullptr),
											  right(nullptr),
											  parent(nullptr),
											  is_nil_node(false),
											  write_owner(0) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m, &attr);
	pthread_cond_init(&can_read, nullptr);
	pthread_cond_init(&can_write, nullptr);
	num_readers = 0;
	num_readers_waiting = 0;
	is_busy = false;
}

rbnode::rbnode() : key(-1),
				   color(BLACK),
				   left(nullptr),
				   right(nullptr),
				   parent(nullptr),
				   is_nil_node(true),
				   write_owner(0) {
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m, &attr);
	pthread_cond_init(&can_read, nullptr);
	pthread_cond_init(&can_write, nullptr);
	num_readers = 0;
	num_readers_waiting = 0;
	is_busy = false;
}

rbnode::~rbnode() {
	pthread_mutex_destroy(&m);
}

void rbnode::read_lock() {
	pthread_mutex_lock(&m);
	num_readers_waiting++;
	while (is_busy)
		pthread_cond_wait(&can_read, &m);
	num_readers_waiting--;
	num_readers++;
	pthread_cond_signal(&can_read);
	pthread_mutex_unlock(&m);
}

void rbnode::read_unlock() {
	pthread_mutex_lock(&m);
	num_readers--;
	if (num_readers == 0)
		pthread_cond_signal(&can_write);
	pthread_mutex_unlock(&m);
}

void rbnode::write_lock() {
	pthread_mutex_lock(&m);
	pthread_t self = pthread_self();
	if (!pthread_equal(self, write_owner)) { // This is my fix for recursive calls to write_lock().
		while (is_busy || num_readers != 0)
			pthread_cond_wait(&can_write, &m);
		is_busy = true;
		write_owner = self;
	}
	pthread_mutex_unlock(&m);
}

void rbnode::write_unlock() {
	pthread_mutex_lock(&m);
	pthread_t self = pthread_self();
	if (pthread_equal(self, write_owner)) { // This is my fix for recursive calls to write_lock().
		is_busy = false;
		if (num_readers_waiting > 0)
			pthread_cond_signal(&can_read);
		else
			pthread_cond_signal(&can_write);
		write_owner = 0;
	}
	pthread_mutex_unlock(&m);
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

bool rbnode::is_nil() {
	return is_nil_node;
}
