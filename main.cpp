#include "def.h"

int main(int argc, char *argv[]) {
    Checker * checker = new Checker();
    for (int i=1; i<argc; i++)
        std::cout<< checker->checkFile(argv[i]);
}