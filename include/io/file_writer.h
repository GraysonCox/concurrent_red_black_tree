//
// Created by Grayson Cox on 11/7/19.
//

#ifndef FILE_WRITER_H
#define FILE_WRITER_H


#include <string>
#include <fstream>
#include <pthread.h>

/**
 * A class that writes text to files. I made this to encapsulate all
 * the ofstream business stuff. Also, this supports concurrent file
 * output.
 */
class file_writer {
public:

	/**
	 * Default constructor that opens or creates the specified file.
	 *
	 * @param file - Name of file to write.
	 */
	explicit file_writer(const std::string &file);

	/**
	 * Default destructor that closes the output file and does other
	 * destructor things.
	 */
	virtual ~file_writer();

	/**
	 * Writes the given string to the output file and appends a
	 * newline character ('\n').
	 *
	 * @param str - The text to be written to the output file.
	 */
	void write_line(const std::string &str);

private:

	std::ofstream *output_file;

	pthread_mutex_t m;

};


#endif //FILE_WRITER_H
