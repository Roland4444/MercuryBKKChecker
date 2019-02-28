#include "def.h"



int main(int argc, char *argv[]) {
    Checker * checker = new Checker();
    std::cout<<checker->checkFile("./tested.wav");
    for (int i=1; i<argc; i++)
        std::cout<< checker->checkFile(argv[i]);
}