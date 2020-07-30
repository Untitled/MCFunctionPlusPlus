/*#include <vector>*/
#include "parser.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            std::ifstream input(argv[i]);
            if (!input) {
                std::cerr << mpp::make_string("Cannot open file: ", argv[i], '.');
                return 1;
            }
            mpp::parser(input).parse();
        }else{
            //args
        }
    }
    return 0;
}