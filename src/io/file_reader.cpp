//
// Created by Grayson Cox on 11/7/19.
//

#include "io/file_reader.h"

file_reader::file_reader(std::string file) {
	input_file.open(file);
}

file_reader::~file_reader() {
	if (input_file.is_open()) {
		input_file.close();
	}
}

std::string file_reader::read_line() {
	std::string str;
	getline(input_file, str);
	return str;
}

std::string file_reader::read_token() {
	while (input_file.peek() == ' ') {
		input_file.ignore();
	}
	std::string str;
	input_file >> str;
	return str;
}

std::string file_reader::read_to(char delimiter) {
	while (input_file.peek() == ' ') {
		input_file.ignore();
	}
	std::string str;
	getline(input_file, str, delimiter);
	return str;
}

bool file_reader::is_open() {
	return input_file.is_open();
}

bool file_reader::is_end_of_file() {
	return input_file.eof();
}

bool file_reader::is_end_of_line() {
	char i = input_file.peek();
	char j = '\n';
	bool k = i == j;
	return k;
}
