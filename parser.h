#include <stack>
#include <string>

namespace parser
{
    class Parser{
        private:
        protected:
            std::string code;
            int readmode;
        public:
            Parser(){}
            Parser(std::string c) : code(c){}
            virtual ~Parser(){}
            virtual void parse();
    };
} // namespace parser
