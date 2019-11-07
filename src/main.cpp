#include <iostream>
#include <io/file_writer.h>
#include "io/file_reader.h"

using namespace std;

const string INPUT_FILE = "/Users/administrator/Documents/ISU/COMS352/PA2/Grayson_Cox_Project2/src/input.txt";

typedef enum {
    SEARCH,
    INSERT,
    DELETE,
    UNKNOWN
} operation_t;

int main() {
    file_reader f(INPUT_FILE);
    if (!f.is_open()) {
        return 1;
    }
    while (!f.is_at_end()) {
        cout << f.read_line() << endl;
    }

    file_writer fileWriter("/Users/administrator/Documents/ISU/COMS352/PA2/Grayson_Cox_Project2/src/dong.txt");
    for (int i = 0; i < 10; i++) {
        fileWriter.write_line("This is text.");
    }

    return 0;
}