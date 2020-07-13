#include "parser.h"

namespace parser
{
    void FileParser::parse()
    {
        for (char ch : code)
        {
            if (ch == ' ' || ch == '\r' || ch == '\n')
                continue;
            if(ch=='f')
                ;
        }
    }
} // namespace parser