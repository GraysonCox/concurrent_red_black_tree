//
// Created by Grayson Cox on 11/20/19.
//

#ifndef RBNODE_H
#define RBNODE_H

#include <pthread.h>

/**
 * Represents the color value of an rbnode.
 */
enum rbnode_color {
	RED,
	BLACK
};

/**
 * A red-black tree node implementation with locking/unlocking
 * capabilities for mutual exclusion.
 */
class rbnode {
public:

	/**
	 * Initializes an rbnode with the given key and color.
	 * All pointers to left, right, and parent nodes will be
	 * set to nullptr.
	 *
	 * @param key - The key expressed as an int.
	 * @param color - The color expressed as type rbnode_color.
	 */
	rbnode(int key, rbnode_color color);

	/**
	 * Initializes a nil (or sentinel) rbnode for representing a leaf
	 * in the tree. This node will have the color BLACK and key value -1,
	 * and is_nil() will always return true. All pointers to left, right,
	 * and parent nodes will be set to nullptr.
	 */
	rbnode();

	/**
	 * The default destructor for freeing the data associated with
	 * this rbnode.
	 */
	virtual ~rbnode();

	/**
	 * Acquires a read lock on this rbnode. Multiple threads can have
	 * a read lock on the same rbnode as long as no other threads have
	 * a write lock on that rbnode.
	 */
	void read_lock();

	/**
	 * Releases the read lock on this rbnode.
	 */
	void read_unlock();

	/**
	 * Acquires a write lock on this rbnode. The lock is acquired when
	 * no other thread has a read lock or write lock on this rbnode.
	 */
	void write_lock();

	/**
	 * Releases the write lock on this rbnode.
	 */
	void write_unlock();

	/**
	 * Returns the integer key associated with this rbnode.
	 *
	 * @return The integer key associated with this rbnode.
	 */
	int get_key() const;

	/**
	 * Sets the integer key associated with this rbnode.
	 *
	 * @param key - The integer key associated with this rbnode.
	 */
	void set_key(int key);

	/**
	 * Returns the color value of this rbnode.
	 *
	 * @return The color value of this rbnode.
	 */
	rbnode_color get_color() const;

	/**
	 * Sets the color value of this rbnode.
	 *
	 * @param color - The color value of this rbnode.
	 */
	void set_color(rbnode_color color);

	/**
	 * Returns a pointer to the left child of this rbnode.
	 *
	 * @return A pointer to the left child of this rbnode.
	 */
	rbnode *get_left() const;

	/**
	 * Sets the given pointer as this rbnode's left child.
	 *
	 * @param n - A pointer to to another rbnode.
	 */
	void set_left(rbnode *n);

	/**
	 * Returns a pointer to the right child of this rbnode.
	 *
	 * @return A pointer to the right child of this rbnode.
	 */
	rbnode *get_right() const;

	/**
	 * Sets the given pointer as this rbnode's right child.
	 *
	 * @param n - A pointer to to another rbnode.
	 */
	void set_right(rbnode *n);

	/**
	 * Returns a pointer to the parent of this rbnode.
	 *
	 * @return A pointer to the parent of this rbnode.
	 */
	rbnode *get_parent() const;

	/**
	 * Sets the given pointer as this rbnode's parent.
	 *
	 * @param n - A pointer to to another rbnode.
	 */
	void set_parent(rbnode *n);

	/**
	 * Returns true if this rbnode represents a nil (or sentinel) node.
	 *
	 * @return - true if this rbnode represents a nil (or sentinel) node.
	 */
	bool is_nil();

private:

	int key;

	rbnode_color color;

	rbnode *left;

	rbnode *right;

	rbnode *parent;

	bool is_nil_node;

	pthread_mutex_t m;

	pthread_cond_t can_read, can_write;

	int num_readers, num_readers_waiting;

	bool is_busy;

	pthread_t write_owner;
};

#endif //RBNODE_H
