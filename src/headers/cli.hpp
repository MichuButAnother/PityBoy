#pragma once
#include <vector>
#include <string>
#include <map>

namespace PityBoy {
    class CLI {
        public:
            template <typename Function>
            CLI bindArgument(Function func); // hello please update!!!!

            template <typename Function>
            CLI bindFlag(char name, Function func);

            void parse(int argc, char * argv[]);
        private:
            std::vector<void (**)()> args;
            std::map<char,void (*)()> flags;
    };
}