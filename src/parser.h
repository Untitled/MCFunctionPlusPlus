#pragma once
#include <any>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstddef>

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
        namesp,
        folder,
        mpp,
        cmd,
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

    struct token {
        using extra_data_index_t = std::size_t;
        token(unsigned int line, token_type type)
            :line(line),
             type(type),
             extra_data_index(0)
        {
        }
        token(unsigned int line, token_type type, extra_data_index_t extra_data_index)
            :type(type),
             line(line),
             extra_data_index(extra_data_index)
        {
        }
        ~token()
        {
        }
        unsigned int line;
        token_type type;
        extra_data_index_t extra_data_index;

        static std::vector<std::string> parsed_names;
        static std::vector<std::string> parsed_strings;
        template <typename T>
        static token::extra_data_index_t store_extra_data(std::vector<T> &storage, const T &data) {
            storage.push_back(data);
            return storage.size() - 1;
        }

        template <typename T>
        static token::extra_data_index_t store_extra_data(std::vector<T> &storage, T &&data) {
            storage.push_back(std::move(data));
            return storage.size() - 1;
        }
    };

    std::vector<token> parsed_tokens;

    void scan_tokens();

    struct code_block {
        enum class type {
            mpp,
            cmd
        }ty;
        std::vector<token> tokens;
    };

    struct function {
        std::string name;
        std::string namesp;
        std::vector<std::string> path;
        std::vector<std::string> arguments;
        code_block content;
    };

    struct {
        std::vector<function> funcs;
    }global_values;

    void scan_global_values();
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
