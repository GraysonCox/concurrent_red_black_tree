//
// Created by Grayson Cox on 10/28/19.
//

#ifndef RBTREE_H
#define RBTREE_H

#include <string>
#include <vector>
#include "rbnode.h"


class rbtree {
public:
	rbtree();

	explicit rbtree(std::vector<rbnode *> preorder);

	bool search(int key);

	void insert_node(int key);

	void delete_node(int key);

	std::string to_string();

	rbnode *get_root() const;

	void set_root(rbnode *root);

	rbnode *get_nil() const;

private:
	rbnode *parent_of_root;
	rbnode *root;
	rbnode *nil;
};

#endif //RBTREE_H
