#ifndef DATAREAD_H
#define DATAREAD_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

namespace dread{

///data read global variables
extern char DEF_BEGIN_SYM;
extern char DEF_END_SYM;
extern std::ifstream* DEF_FILE;
extern std::string DEF_FILE_NAME;

///data read global functions
void setDefaults(std::string defFileName,std::ifstream* defFile,char beginSym,char endSym);
extern bool checkEofError(std::string fileName,std::ifstream* f);
extern void skipUntil(std::string fileName,std::ifstream* f,char skipSym);
extern void skipUntil(char skipSym);
extern std::string readSingle(std::string fileName,std::ifstream* f,char beginSym,char endSym);
extern std::string readSingle(std::string fileName,std::ifstream* f);
extern std::string readSingle(char beginSym,char endSym);
extern std::string readSingle();

}



#endif // DATAREAD_H
