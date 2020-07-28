/*#include <vector>
#include <string>*/
#include <fstream>
#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc, char** argv) {
    for(int i = 1; i < argc; ++i) {
        std::ifstream input(argv[i]);
        if(!input) {
            std::cerr << mpp::make_string("Cannot open file: ", argv[i], '.');
            return 1;
        }
        mpp::parser(input).parse();
    }
    return 0;
}