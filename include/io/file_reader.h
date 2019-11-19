//
// Created by Grayson Cox on 11/7/19.
//

#ifndef CONCURRENT_RED_BLACK_TREE_FILE_READER_H
#define CONCURRENT_RED_BLACK_TREE_FILE_READER_H


#include <string>
#include <fstream>
#include <iostream>

class file_reader {
public:

	file_reader(std::string file);

	virtual ~file_reader();

	std::string read_line();

	std::string read_token();

	bool is_open();

	bool is_at_end();

private:
	std::ifstream input_file;
};


#endif //CONCURRENT_RED_BLACK_TREE_FILE_READER_H
