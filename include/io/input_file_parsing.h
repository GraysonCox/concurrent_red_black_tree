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

rbtree *parse_tree(file_reader *reader);

int parse_num_threads(file_reader *reader);

void parse_tasks(file_reader *reader, std::queue<operation> *read_tasks, std::queue<operation> *write_tasks);


#endif //INPUT_FILE_PARSING_H
