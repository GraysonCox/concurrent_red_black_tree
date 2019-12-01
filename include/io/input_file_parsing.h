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
#include <model/thread_data.h>

rbtree *parse_tree(file_reader *reader);

std::vector<thread_data *> parse_thread_names(file_reader *reader);

void parse_tasks(file_reader *reader, std::vector<thread_data *> *threads);


#endif //INPUT_FILE_PARSING_H
