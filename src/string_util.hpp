#pragma once
#include <string>
#include <sstream>

namespace parser {
    template <typename ...Args>
    std::string make_string(Args... arguments) {
        static std::stringstream buffer;
        buffer.clear();
        buffer.str("");
        append_string_buffer(buffer, arguments...);
        return buffer.str();
    }
    template <typename T, typename ...Args>
    void append_string_buffer(std::stringstream &buffer, T argument, Args... arguments) {
        buffer << argument;
        append_string_buffer(buffer, arguments...);
    }

    template <typename T>
    void append_string_buffer(std::stringstream &buffer, T argument) {
        buffer << argument;
    }
} //  namespace parser

