//
// Created by Grayson on 11/7/19.
//

#include "io/file_writer.h"

file_writer::file_writer(std::string file) {
    output_file.open(file);
}

file_writer::~file_writer() {
    if (output_file.is_open()) {
        output_file.close();
    }
}

void file_writer::write_line(std::string str) {
    output_file << str << std::endl;
}
