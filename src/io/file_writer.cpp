//
// Created by Grayson Cox on 11/7/19.
//

#include "io/file_writer.h"

file_writer::file_writer(const std::string &file) {
	output_file = new std::ofstream(file);
	pthread_mutex_init(&m, nullptr);
}

file_writer::~file_writer() {
	pthread_mutex_destroy(&m);
	if (output_file->is_open()) {
		output_file->close();
	}
	delete output_file;
}

void file_writer::write_line(const std::string &str) {
	pthread_mutex_lock(&m);
	*output_file << str << std::endl;
	pthread_mutex_unlock(&m);
}