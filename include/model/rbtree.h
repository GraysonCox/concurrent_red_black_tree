//
// Created by Grayson Cox on 10/28/19.
//

#ifndef RBTREE_H
#define RBTREE_H

#include <queue>
#include <string>
#include <vector>
#include "rbnode.h"

/**
 * A red-black tree implementation that allows for concurrent searching,
 * insertion, and deletion.
 */
class rbtree {
public:

	/**
	 * Initializes an empty rbtree.
	 */
	rbtree();

	/**
	 * Builds an rbtree from the given pre-order traversal.
	 *
	 * @param preorder - A vector of rbnode pointers in prefix order.
	 * 		Every leaf node should be given as nullptr.
	 */
	explicit rbtree(std::vector<rbnode *> *preorder);

	virtual ~rbtree();

	/**
	 * Synchronously searches for the given key in the tree.
	 *
	 * @param key - The key to search for in the tree
	 * @return True if the key is contained in the tree
	 */
	bool search(int key);

	/**
	 * Synchronously inserts a new rbnode with the given key into the tree.
	 *
	 * @param key - The key to be inserted.
	 */
	void insert_node(int key);

	/**
	 * Synchronously deletes the first rbnode with the given key from the tree.
	 *
	 * @param key - The key to be deleted.
	 */
	void delete_node(int key);

	/**
	 * Returns a string representation of a pre-order traversal of
	 * the rbtree. Each node is shown in the form 'kc', where 'k' is
	 * integer key value and 'c' is the color of the node represented
	 * as 'r' for red and 'b' for black, and each leaf node is represented
	 * with an 'f'.
	 *
	 * @return A pre-order traversal as a string.
	 */
	std::string to_string();

	/**
	 * Creates a sideways depiction of the tree as a string. Each node is shown
	 * in the form kc, where k is integer key value and c is the color of the node
	 * represented as r for red and b for black.
	 *
	 * @return A string representation of the contents of the tree.
	 */
	std::string to_string_pretty();

private:

	rbnode *parent_of_root;

	rbnode *root;

	rbnode *build_from_preorder(std::vector<rbnode *> *preorder, std::vector<rbnode *>::size_type *next_index);

	void insert_fixup(rbnode *z);

	void delete_fixup(rbnode *x);

	void rotate_clockwise(rbnode *x);

	void rotate_counter_clockwise(rbnode *x);

	void transplant(rbnode *u, rbnode *v);

	std::string to_string_rec(rbnode *root);

	std::string to_string_pretty_rec(rbnode *root, int space);
};

#endif //RBTREE_H
