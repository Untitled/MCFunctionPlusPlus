#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "exceptions.hpp"

namespace parser
{
    class parser {
        protected:
        private:
            std::istream &in;

            class file_scanning_state {
            private:
                std::istream &in;
                char currentCharacter = 0;
            public:
                file_scanning_state(std::istream &in) noexcept: in(in) {}
                char get_current_character() noexcept;
                char get_next_character();
                bool eof() noexcept;
            }fileScanningState;

            enum class token_type {
                function,
                open_bracket,
                close_bracket,
                open_square_bracket,
                close_square_bracket,
                open_brace,
                close_brace,
                name
            };

            struct extra_data {};

            struct token {
                token(token_type type) : type(type), extra_data(nullptr) {}
                token(token_type type, const void * const extra_data) : type(type), extra_data(extra_data) {}
                token_type type;
                const void * const extra_data;
            };

            std::vector<token> parsed_tokens;

            void scan_tokens();
        public:
            parser(std::istream &in)  noexcept: in(in), fileScanningState(in) {}
            ~parser() {};
            void parse();
    };
} // namespace parser
