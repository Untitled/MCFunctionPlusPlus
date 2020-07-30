#include "parser.h"
#include "string_util.hpp"
#include "character_util.hpp"
#include <iostream>
#include <map>

using namespace mpp;
std::vector<std::string> mpp::parser::token::parsed_names;
std::vector<std::string> mpp::parser::token::parsed_strings;

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

        auto push_token_without_extra_data = [&](token_type t) {
            parsed_tokens.push_back(token(line, t));
        };

        auto push_token_with_extra_data = [&](token_type t, token::extra_data_index_t i) {
            parsed_tokens.push_back(token(line, t, i));
        };

        try {
            fileScanningState.get_next_character();
            while(!fileScanningState.eof()) {
                skip_blanks();
                char c = fileScanningState.get_current_character();
                if(is_letter(c) || c == '_') {
                    auto value = read_name();

                    static std::map<std::string, token_type> keyword_tokentype_mappings {
                        std::make_pair("func", token_type::function),
                        std::make_pair("const", token_type::const_type),
                        std::make_pair("say", token_type::say),
                        std::make_pair("namesp", token_type::namesp),
                        std::make_pair("folder", token_type::folder),
                        std::make_pair("mpp", token_type::mpp),
                        std::make_pair("cmd", token_type::cmd)
                    };

                    try {
                        push_token_without_extra_data(keyword_tokentype_mappings.at(value));
                    }
                    catch(std::out_of_range&) {
                        push_token_with_extra_data(token_type::name, token::store_extra_data(token::parsed_names, value));
                    }
                    continue;
                }
                else {
                    switch(c) {
                    case '[':
                        push_token_without_extra_data(token_type::open_square_bracket);
                        break;
                    case ']':
                        push_token_without_extra_data(token_type::close_square_bracket);
                        break;
                    case '(':
                        push_token_without_extra_data(token_type::open_bracket);
                        break;
                    case ')':
                        push_token_without_extra_data(token_type::close_bracket);
                        break;
                    case '{':
                        push_token_without_extra_data(token_type::open_brace);
                        break;
                    case '}':
                        push_token_without_extra_data(token_type::close_brace);
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

                            push_token_with_extra_data(token_type::string, token::store_extra_data(token::parsed_strings, string_value));
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
                            push_token_without_extra_data(token_type::div);
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

    void parser::scan_global_values() {
        std::size_t i = 0;
        auto get_next_token = [&]() ->token&{
            try {
                return parsed_tokens.at(i++);
            }
            catch(...) {
                throw compile_error(parsed_tokens.at(parsed_tokens.size() - 1).line, "No more tokens.");
            }
        };

        std::string namesp;
        std::vector<std::string> path;
        unsigned int level = 0;
        while(i < parsed_tokens.size()) {
            token &current_token = get_next_token();
            //namesp
            if(current_token.type == token_type::namesp) {
                if(level == 0) {
                    auto &name_tok = get_next_token();
                    if(name_tok.type == token_type::name) {
                        namesp = token::parsed_names.at(name_tok.extra_data_index);
                        auto &brace_tok = get_next_token();
                        if(brace_tok.type == token_type::open_brace) {
                            ++level;
                        }
                        else {
                            throw compile_error(brace_tok.line, "After \"namespace <name>\", there should be a open-brace.");
                        }
                    }
                    else {
                        throw compile_error(name_tok.line, "After \"namespace\", there should be a name.");
                    }
                }
                else {
                    throw compile_error(current_token.line, "Keyword \"namespace\" should be placed at the outside.");
                }
            }
            //folder
            else if(current_token.type == token_type::folder) {
                if(level > 0) {
                    auto &name_tok = get_next_token();
                    if(name_tok.type == token_type::name) {
                        path.push_back(token::parsed_names.at(name_tok.extra_data_index));
                        auto &brace_tok = get_next_token();
                        if(brace_tok.type == token_type::open_brace) {
                            ++level;
                        }
                        else {
                            throw compile_error(brace_tok.line, "After \"folder <name>\", there should be a open-brace.");
                        }
                    }
                    else {
                        throw compile_error(name_tok.line, "After \"folder\", there should be a name.");
                    }
                }
                else {
                    throw compile_error(current_token.line, "A folder should be in a namespace.");
                }
            }
            //function
            else if(current_token.type == token_type::function) {
                if(level > 0) {
                    auto &name_tok = get_next_token();
                    if(name_tok.type == token_type::name) {
                        std::string func_name = token::parsed_names.at(name_tok.extra_data_index);
                        //arguments
                        bool specified_arguments = true;
                        auto &brace_tok = get_next_token();
                        if(brace_tok.type == token_type::open_bracket) {
                            while(true) {
                                auto &token = get_next_token();
                                if(token.type == token_type::close_bracket) {
                                    break;
                                }
                                else {
                                    throw std::runtime_error("Have not supported arguments");
                                }
                            }
                        }
                        else {
                            throw compile_error(brace_tok.line, "After function <name>, there should be ().");
                        }
                        code_block code_bl;
                        //code block
                        auto &sq_bracket_tok = get_next_token();
                        if(sq_bracket_tok.type == token_type::open_square_bracket) {
                            auto &code_block_type_tok = get_next_token();
                            switch(code_block_type_tok.type)
                            {
                            case token_type::mpp:
                                code_bl.ty = code_block::type::mpp;
                                break;
                            case token_type::cmd:
                                code_bl.ty = code_block::type::cmd;
                                break;
                            default:
                                throw compile_error(code_block_type_tok.line, "Unsupported code block type.");
                            }
                            auto &sq_bracket_close_tk = get_next_token();
                            if(sq_bracket_close_tk.type == token_type::close_square_bracket) {
                                auto &brace_op_tk = get_next_token();
                                if(brace_op_tk.type == token_type::open_brace) {
                                    std::vector<token> tokens;
                                    unsigned int level = 1;
                                    while(level > 0) {
                                        auto &token = get_next_token();
                                        switch(token.type) {
                                        case token_type::open_brace:
                                            ++level;
                                            break;
                                        case token_type::close_brace:
                                            --level;
                                            break;
                                        }
                                        tokens.push_back(std::move(token));
                                    }

                                    global_values.funcs.push_back({func_name, namesp, path, {}});
                                }
                                else {
                                    throw compile_error(brace_op_tk.line, "There should be a { after [<type>].");
                                }
                            }
                            else {
                                throw compile_error(sq_bracket_close_tk.line, "While making a code block, after [<type>, there should be a ].");
                            }
                        }
                        else {
                            throw compile_error(sq_bracket_tok.line, "After \"function <name>\", there should be a code block.");
                        }
                    }
                    else {
                        throw compile_error(name_tok.line, "After \"folder\", there should be a name.");
                    }
                }
                else {
                    throw compile_error(current_token.line, "A function should be in a namespace or a folder.");
                }
            }
            else if(current_token.type == token_type::close_brace) {
                if(level == 1) {
                    namesp = "";
                }
                else if(level > 1) {
                    path.pop_back();
                }
                else {
                    throw compile_error(current_token.line, "There shouldn\'t be a close-brace here.");
                }
                --level;
            }
            else {
                throw compile_error(current_token.line, "There shouldn\'t be this token.");
            }
        }

        //free memory
        {
            std::vector<token> buffer;
            buffer.swap(parsed_tokens);
        }
    };

    void parser::parse() {
        scan_tokens();

        //now incomplete
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
                std::cout << "<name:" << token::parsed_names.at(token.extra_data_index) << "> ";
                break;
            case token_type::say:
                std::cout << "say ";
                break;
            case token_type::string:
                std::cout << "<string:\"" << token::parsed_strings.at(token.extra_data_index) << "\">";
                break;
            }
        }
        std::cout << std::endl;
        scan_global_values();
        for(auto &func : global_values.funcs) {
            std::cout << func.namesp << ':';
            for(auto &folder : func.path) {
                std::cout << folder << '/';
            } 
            std::cout << func.name << std::endl;;
        }
    }
} // namespace mpp