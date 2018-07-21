#ifndef NAMES_H
#define NAMES_H

#include <string>
#include <fstream>
#include <cstdlib>

namespace names{
    ///constants
    extern const char *NAMES;
    extern const char *ADJ;
    extern const char *FIRST;
    extern const char *LAST;
    extern const char *OPENSEA;
    extern const char *ISLAND;
    extern const char *FIRSTFIRE;
    extern const char *FIRE;
    ///global variables
    extern std::string path;
    ///global functions
    std::string getRand(const char *fileName);
    std::string getRand(const char *fileName1,const char *fileName2);
}

#endif
