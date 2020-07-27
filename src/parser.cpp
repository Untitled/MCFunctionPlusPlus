#include "parser.h"
#include "string_util.hpp"
#include "character_util.hpp"
#include <iostream>

using namespace parser;

namespace parser {
    char parser::file_scanning_state::get_current_character() noexcept {
        return currentCharacter;
    }

    char parser::file_scanning_state::get_next_character() {
        if(in.eof()) {
            throw end_of_file("Read failed: end of stream.");
        }
        currentCharacter = in.get();
        return currentCharacter;
    }

    bool parser::file_scanning_state::eof() noexcept {return in.eof();}

    void parser::scan_tokens() {
        unsigned int line = 1;

        auto &skip_blanks = [&]() -> unsigned int {
            unsigned int result = 0;
            char c = fileScanningState.get_current_character();
            while(is_blank(c)) {
                ++result;
                if(c == '\n') {
                    ++line;
                }
                c = fileScanningState.get_next_character();
            }
            return result;
        };
        

        auto &read_name = [&]() -> std::string {
            std::string result = "";
            char c = fileScanningState.get_current_character();
            if(is_letter(c) || c == '_') {
                result += c;
                c = fileScanningState.get_next_character();
            }
            else {
                throw std::runtime_error("Is not reading a name.");
            }
            while(is_letter(c) || c == '_' || is_number_character(c)) {
                result += c;
                c = fileScanningState.get_next_character();
            }
            return result;
        };

        try {
            fileScanningState.get_next_character();
            while(!fileScanningState.eof()) {
                skip_blanks();
                char c = fileScanningState.get_current_character();
                if(is_letter(c) || c == '_') {
                    auto name = read_name();
                    if(name == "func") {
                        parsed_tokens.push_back(token_type::function);
                    }
                    else {
                        parsed_tokens.push_back(token_type::name);
                    }
                    continue;
                }
                else {
                    switch(c) {
                    case '[':
                        parsed_tokens.push_back(token_type::open_square_bracket);
                        break;
                    case ']':
                        parsed_tokens.push_back(token_type::close_square_bracket);
                        break;
                    case '(':
                        parsed_tokens.push_back(token_type::open_bracket);
                        break;
                    case ')':
                        parsed_tokens.push_back(token_type::close_bracket);
                        break;
                    case '{':
                        parsed_tokens.push_back(token_type::open_brace);
                        break;
                    case '}':
                        parsed_tokens.push_back(token_type::close_brace);
                        break;
                    default:
                        throw compile_error(line, make_string("Unexpected character \'", c, "\'."));
                    }
                }
                c = fileScanningState.get_next_character();
            }
        }
        catch (std::exception &e) {
            std::cerr << "[error] " << e.what() << std::endl;
        } 
    }

    void parser::parse() {
        scan_tokens();
        for(auto &token : parsed_tokens) {
            switch(token.type) {
            case token_type::function:
                std::cout << "function ";
                break;
            case token_type::open_bracket:
                std::cout << "( ";
                break;
            case token_type::close_bracket:
                std::cout << ") ";
                break;
            case token_type::open_square_bracket:
                std::cout << "[ ";
                break;
            case token_type::close_square_bracket:
                std::cout << "] ";
                break;
            case token_type::open_brace:
                std::cout << "{ ";
                break;
            case token_type::close_brace:
                std::cout << "} ";
                break;
            case token_type::name:
                std::cout << "name ";
            }
        }
        std::cout << std::flush;
    }
} // namespace parser