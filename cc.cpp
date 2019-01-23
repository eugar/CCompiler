#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int count(std::ifstream &input)
{
    int c = 0;
    std::string line;

    while (getline(input, line)) {
        c += line.length();
    }

    return c;
}

int main(int argc, char** argv)
{
    int c;
    int totalChars;

    if (argc != 2) { // just the file or help option
HELP:
        std::cout << "cc - print character count for a file\n"
            << "Usage:\n" 
            << argv[0] << " -h to display this help menu\n"
            << "or\n" 
            << argv[0] << " [FILE]\n";
        return 1;
    }

    while ((c = getopt(argc, argv, "?:h")) != -1) {
        switch (c) {
            case 'h':
                goto HELP; //print usage               
                break;
        }
    }

    std::string filename = argv[1];
    std::ifstream input(filename);
    if (input.is_open()) {
        totalChars = count(input);
        std::cout << "The file has " << totalChars << " characters.\n";
        input.close();
    } else {
        std::cout << "File not found.\n";
    }

    return 0;
}


