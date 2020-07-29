#include "string_util.hpp"
#include <stdexcept>
#include <string>

#define DEFINE_EXCEPTION(ClassName)           \
    class ClassName : std::runtime_error {    \
    public:                                   \
        ClassName(const std::string& message) \
            : std::runtime_error(message)     \
        {                                     \
        }                                     \
    };

namespace mpp {

DEFINE_EXCEPTION(end_of_file);

class compile_error
    : public std::runtime_error {
public:
    compile_error(unsigned int line, const std::string& message)
        : std::runtime_error(make_string("compile error at line ", line, ": ", message))
    {
    }
};

} //  namespace mpp
