#include "def.h"
using namespace std;

Checker* GlobChecker;


 
extern "C"{
	int summ(int a, int b)
	{
    		return a+b;
	}

	void init(){
    		GlobChecker = new Checker();
	}

	int checkFile(char* filename)
	{
    		return GlobChecker->checkFile(filename);
	}
}


int main(int argc, char *argv[])
{
    Checker * checker = new Checker();
    std::cout<<checker->checkFile("./tested.wav");
    for (int i=1; i<argc; i++)
        std::cout<< checker->checkFile(argv[i]);
}
