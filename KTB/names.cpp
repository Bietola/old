#include "names.h"
#include "utils.h"

namespace names{
    ///constants
    ///names
    const char *NAMES="names.txt";
    const char *ADJ="adjectives.txt";
    const char *FIRST="first names.txt";
    const char *LAST="last names.txt";
    const char *OPENSEA="open sea.txt";
    const char *ISLAND="island.txt";
    ///phrases
    const char *FIRSTFIRE="firstfire.txt";
    const char *FIRE="fire.txt";

    ///global variables initialization
    std::string path="";

    ///global functions
    //get a random string inside of a file
    std::string getRand(const char *fileName){
        std::string p(fileName);
        p=path+p;
        std::ifstream f(fileName,std::ifstream::binary);
        std::string s;
        int lines=0;
        while(std::getline(f,s)) lines++;
        f.clear();
        f.seekg(0);
        int r=rand()%lines;
        for(int j=0;j<r+1;j++) std::getline(f,s);
        s.erase(s.size()-1);
        f.close();
        return s;
    }
    //make a single name out of two
    std::string getRand(const char *fileName1,const char *fileName2){
        std::string n1=getRand(fileName1),n2;
        do{n2=getRand(fileName2);}while(n1==n2);
        return n1+n2;
    }
}
