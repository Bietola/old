#include "data_read.h"

///data read global variables initialization
char dread::DEF_BEGIN_SYM = '[';
char dread::DEF_END_SYM = ']';
std::ifstream* dread::DEF_FILE;
std::string dread::DEF_FILE_NAME;

///data read global functions
void dread::setDefaults(std::string defFileName,std::ifstream* defFile,char defBeginSym,char defEndSym){
    DEF_FILE_NAME = defFileName;
    DEF_FILE = defFile;
    DEF_BEGIN_SYM = defBeginSym;
    DEF_END_SYM = defEndSym;
}
bool dread::checkEofError(std::string fileName,std::ifstream* f){
    if(f->eof()){
        std::cout << "ERR: reached end of file while reading [" << fileName << "]" << std::endl;
        return true;
    }
    return false;
}
void dread::skipUntil(std::string fileName,std::ifstream* f,char skipSym){
    while(f->get() != skipSym) checkEofError(fileName,f);
}
void dread::skipUntil(char skipSym){
    skipUntil(DEF_FILE_NAME,DEF_FILE,skipSym);
}
std::string dread::readSingle(std::string fileName,std::ifstream* f,char beginSym,char endSym){
    std::string retString;
    std::function<bool()> checkEofErr = std::bind(checkEofError,fileName,f);
    //get to the begin sym
    while(f->get() != beginSym) checkEofErr();
    //return content as single string
    while(f->peek() != endSym){
        checkEofErr();
        retString.push_back(f->get());
    }
    //advance past the end sym
    f->get();
    //return result
    return retString;
}
std::string dread::readSingle(std::string fileName,std::ifstream* f){
    return readSingle(fileName,f,DEF_BEGIN_SYM,DEF_END_SYM);
}
std::string dread::readSingle(char beginSym,char endSym){
    return readSingle(DEF_FILE_NAME,DEF_FILE,beginSym,endSym);
}
std::string dread::readSingle(){
    return readSingle(DEF_FILE_NAME,DEF_FILE,DEF_BEGIN_SYM,DEF_END_SYM);
}
