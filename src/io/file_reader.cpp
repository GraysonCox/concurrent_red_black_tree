//
// Created by Grayson on 11/7/19.
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
//    getline(input_file, str);
    input_file >> str;
    return str;
}

bool file_reader::is_open() {
    return input_file.is_open();
}

bool file_reader::is_at_end() {
    return input_file.eof();
}
