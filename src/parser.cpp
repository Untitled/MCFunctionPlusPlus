#include "parser.h"
#include "string_util.hpp"
#include "character_util.hpp"
#include <iostream>

using namespace mpp;

namespace mpp {
    char parser::file_scanning_state::get_current_character() {
        return currentCharacter;
    }

    char parser::file_scanning_state::get_next_character() {
        if(in.eof()) {
            throw end_of_file("Read failed: end of stream.");
        }
        currentCharacter = in.get();
        return currentCharacter;
    }

    bool parser::file_scanning_state::eof() {return in.eof();}

    void parser::scan_tokens() {
        unsigned int line = 1;

        auto skip_blanks = [&]() -> unsigned int {
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
        

        auto read_name = [&]() -> std::string {
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
                    auto value = read_name();
                    if(value == "func") {
                        parsed_tokens.push_back(token_type::function);
                    }
                    else if(value == "const") {
                        parsed_tokens.push_back(token_type::const_type);
                    }
                    else if(value == "say") {
                        parsed_tokens.push_back(token_type::say);
                    }
                    else {
                        //TODO: put extra data
                        parsed_tokens.push_back(token(token_type::name,std::any(value/*.data()*/)));
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
                    case '\'':
                    case '\"':
                        {
                            std::string string_value = "";
                            char buffer = 0;
                            while((buffer = fileScanningState.get_next_character()) != c) {
                                if(buffer == '\\') {
                                    switch(fileScanningState.get_next_character()) {
                                    case 'n':
                                        buffer = '\n';
                                        break;
                                    case 'u':
                                        //TODO: make unicode character
                                        break;
                                    case 't':
                                        buffer = '\t';
                                        break;
                                    default:
                                        throw compile_error(line, make_string("Unexpected ", fileScanningState.get_current_character(), " after \\ in string."));
                                    }
                                }
                                string_value += buffer;
                            }
                            //TODO: put extra data
                            parsed_tokens.push_back(token(token_type::string,std::any(string_value)));
                        }
                        break;
                    case '/':
                        switch(fileScanningState.get_next_character()) {
                        case '/':
                            while(fileScanningState.get_next_character() != '\n');
                            ++line;
                            break;
                        case '*':
                            {
                                char buffer = 0;
                                while(true) {
                                    buffer = fileScanningState.get_next_character();
                                    if(buffer == '*' && fileScanningState.get_next_character() == '/') {
                                        break;
                                    }
                                    else if(fileScanningState.get_current_character() == '\n') {
                                        ++line;
                                    }
                                }
                            }                   
                            break;
                        default:
                            parsed_tokens.push_back(token_type::div);
                            continue;
                        }
                        break;
                    default:
                        throw compile_error(line, make_string("Unexpected character \'", c, "\'."));
                    }
                }
                try {c = fileScanningState.get_next_character();}
                catch(end_of_file) {break;}
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
                std::cout << "<name ";
                if(token.extra_data.has_value())
                    if(token.extra_data.type()==typeid(std::string))
                        std::cout << std::any_cast<std::string>(token.extra_data);
                std::cout << "> ";
                break;
            case token_type::say:
                std::cout << "say ";
                break;
            case token_type::string:
                std::cout << "<string ";
                if(token.extra_data.has_value()&&token.extra_data.type()==typeid(std::string))
                    std::cout << std::any_cast<std::string>(token.extra_data);
                std::cout << "> ";
                break;
            }
        }
        std::cout << std::flush;
    }
} // namespace mpp