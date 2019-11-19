//
// Created by Grayson on 11/7/19.
//

#ifndef CONCURRENT_RED_BLACK_TREE_FILE_WRITER_H
#define CONCURRENT_RED_BLACK_TREE_FILE_WRITER_H


#include <string>
#include <fstream>

class file_writer {
public:

    file_writer(std::string file);

    virtual ~file_writer();

    void write_line(std::string str);

private:
    std::ofstream output_file;
};


#endif //CONCURRENT_RED_BLACK_TREE_FILE_WRITER_H
