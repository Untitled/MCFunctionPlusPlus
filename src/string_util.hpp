#pragma once
#include <string>
#include <sstream>

namespace mpp {
    template <typename T>
    void append_to_string_buffer(std::stringstream &buffer, T argument) {
        buffer << argument;
    }
    
    template <typename T, typename ...Args>
    void append_to_string_buffer(std::stringstream &buffer, T argument, Args... arguments) {
        buffer << argument;
        append_to_string_buffer(buffer, arguments...);
    }

    template <typename ...Args>
    std::string make_string(Args... arguments) {
        static std::stringstream buffer;
        buffer.clear();
        buffer.str("");
        append_to_string_buffer(buffer, arguments...);
        return buffer.str();
    }
} //  namespace mpp

