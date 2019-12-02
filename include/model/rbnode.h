//
// Created by Grayson Cox on 11/20/19.
//

#ifndef RBNODE_H
#define RBNODE_H

#include <pthread.h>

enum rbnode_color {
	RED,
	BLACK
};

class rbnode {
public:
	rbnode(int key, rbnode_color color);

	rbnode();

	virtual ~rbnode();

	int get_key() const;

	void set_key(int key);

	rbnode_color get_color() const;

	void set_color(rbnode_color color);

	rbnode *get_left() const;

	void set_left(rbnode *n);

	rbnode *get_right() const;

	void set_right(rbnode *n);

	rbnode *get_parent() const;

	void set_parent(rbnode *n);

	void lock();

	void unlock();

	bool is_nil();

private:
	int key;
	rbnode *left;
	rbnode *right;
	rbnode *parent;
	rbnode_color color;
	// TODO: Add mutex attribute and methods
	pthread_mutex_t m;
	bool is_mutex_locked;
	bool is_nil_node;
};

#endif //RBNODE_H
