#include <vector>
#include <string>
#include <fstream>
#include "parser.h"

using namespace std;

int main(int argc, char** argv) {
    for(int i = 1; i < argc; ++i) {
        parser::parser(std::ifstream(argv[i])).parse();
    }
    return 0;
}