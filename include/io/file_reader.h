//
// Created by Grayson Cox on 11/7/19.
//

#ifndef FILE_READER_H
#define FILE_READER_H


#include <string>
#include <fstream>
#include <iostream>

/**
 * A class that reads from text files. This seemed like a good idea
 * when I made it.
 */
class file_reader {
public:

	/**
	 * Initializes a file_reader for the given file.
	 *
	 * @param file - Name of file to read.
	 */
	explicit file_reader(const std::string &file);

	/**
	 * Default destructor that closes the input file and
	 * what not.
	 */
	virtual ~file_reader();

	/**
	 * Reads and returns all text from the input file up to
	 * the next newline character ('\n').
	 *
	 * @return A string containing all text from the input file up to
	 * 			the next newline character ('\n').
	 */
	std::string read_line();

	/**
	 * Reads and returns all text from the input file up to the next
	 * white space or newline character. This method ignores preceding
	 * spaces.
	 *
	 * @return A string containing the next token in the file.
	 */
	std::string read_token();

	/**
	 * Returns true if the input file was opened successfully.
	 *
	 * @return true if the input file was opened successfully.
	 */
	bool is_open();

	/**
	 * Returns true if the input file has no more text to read.
	 *
	 * @return true if the input file has no more text to read.
	 */
	bool is_end_of_file();

private:

	std::ifstream *input_file;

};


#endif //FILE_READER_H
