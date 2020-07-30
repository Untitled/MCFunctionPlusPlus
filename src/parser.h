#pragma once
#include <any>
#include <iostream>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>
#include <stack>

#include "exceptions.hpp"

namespace mpp {
class parser {
protected:
private:
    std::istream& in;

    class file_scanning_state {
    private:
        std::istream& in;
        char currentCharacter = 0;

    public:
        file_scanning_state(std::istream& in)
            : in(in)
        {
        }
        char get_current_character();
        char get_next_character();
        bool eof();
    } fileScanningState;

    enum class token_type {
        function,
        open_bracket,
        close_bracket,
        open_square_bracket,
        close_square_bracket,
        open_brace,
        close_brace,
        name,
        string,
        const_type,
        say,
        div
    };
    /*
    struct extra_data {
        void* data;
        extra_data(data_type type, void* data)
        {
        }
    };
*/
    struct token {
        token(token_type type)
            : type(type)
            , extra_data(nullptr)
        {
        }
        token(token_type type, std::any extra_data)
            : type(type)
            , extra_data(extra_data)
        {
        }
        ~token()
        {
        }
        token_type type;
        std::any extra_data;
    };

    std::vector<token> parsed_tokens;

    void scan_tokens();

public:
    parser(std::istream& in)
        : in(in)
        , fileScanningState(in)
    {
    }
    ~parser() {};
    void parse();
};
} // namespace mpp
