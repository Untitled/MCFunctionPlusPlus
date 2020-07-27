#include "parser.h"

namespace parser{
    void Parser::parse(){
        std::string lastPattern;
        int i = 0;
        for (int i = 0; i < code.length(); i++){
            char ch = code[i];
            if (ch == ' ' || ch == '\r' || ch == '\n'){
                if(!lastPattern.empty()){
                    //Throw error of invalid keyword
                }
                lastPattern = "";
                return;
            }
            if ((!lastPattern.empty())&&((std::string)"function").substr(0,lastPattern.length()-1)==lastPattern){
                lastPattern.push_back(ch);
            }else{
                //Throw error of invalid keyword
            }
        }
    }
} // namespace parser