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
	 * Acquires a mutex lock on this rbnode. This implementation uses
	 * a recursive mutex mechanism, so if a single thread locks the node
	 * multiple times recursively, that thread will have to call rbnode.unlock()
	 * the same number of times before another thread can acquire the lock.
	 */
	void lock();

	/**
	 * Releases the mutex lock on this rbnode. If the calling thread has
	 * multiple recursive locks on this node, the thread will have to call this
	 * method the same number of times before another thread can acquire the lock.
	 */
	void unlock();

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

	pthread_mutex_t m;

	bool is_nil_node;
};

#endif //RBNODE_H
