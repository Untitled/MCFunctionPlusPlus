#include <stack>
#include <string>

namespace parser
{
    class IParser{
        private:
        protected:
            std::string code;
            int readmode;
        public:
            IParser(){}
            IParser(std::string c) : code(c){}
            virtual ~IParser(){}
            virtual void parse() = 0;
    };

    class FileParser : public IParser{
        private:
        protected:
        public:
            FileParser(){}
            FileParser(std::string c) : IParser(c){}
            virtual ~FileParser(){}
            void parse();
    };
} // namespace parser
