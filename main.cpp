#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

int main(){
    std::fstream instream("helloworld.fpp");
    if(!instream.is_open()){
        return 1; //Can't open file
    }
    std::string code;
    char c;
    /*while(!instream.eof()){
        instream >> c;
        code.push_back(c);
    }*/
    //std::cout << code;
    return 0;
}