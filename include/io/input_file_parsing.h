//
// Created by Grayson Cox on 12/1/19.
//

#ifndef INPUT_FILE_PARSING_H
#define INPUT_FILE_PARSING_H


#include <iostream>
#include <vector>
#include <sstream>
#include <io/file_reader.h>
#include <model/rbtree.h>
#include <model/operation.h>
#include <model/operation_queue.h>

/**
 * Uses the given file_reader to read the preorder tree traversal from
 * the input file and creates the rbtree object.
 *
 * @param reader - A file_reader for the input file.
 * @return The rbtree parsed from the input file.
 */
rbtree *parse_tree(file_reader *reader);

/**
 * Uses the given file_reader to read and return the integer value
 * preceded by "Search threads:" or "Modify threads:".
 *
 * @param reader - A file_reader for the input file.
 * @return The integer value preceded by "Search threads:" or
 * 			"Modify threads:".
 */
int parse_num_threads(file_reader *reader);

/**
 * Uses the given file_reader to parse the red-black tree operations
 * from the input file and place each one in its respective queue.
 *
 * @param reader - A file_reader for the input file.
 * @param read_tasks - The queue holding the search/read operations.
 * @param write_tasks - The queue holding the modify/write operations.
 */
void parse_tasks(file_reader *reader, std::queue<operation> *read_tasks, std::queue<operation> *write_tasks);


#endif //INPUT_FILE_PARSING_H
