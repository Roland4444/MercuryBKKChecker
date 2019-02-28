#include <fstream>
#include "def.h"

void foreach(char * filename, Checker* cau)
{
    std::ifstream ifs(filename);
    std::string line;
    while(std::getline(ifs, line))
        cau ->checkFile(line.c_str());
}

int main(int argc, char *argv[]) {
    Checker * checker = new Checker();
    for (int i=1; i<argc; i++)
        if (argc>1)
            foreach(argv[1], checker);
}