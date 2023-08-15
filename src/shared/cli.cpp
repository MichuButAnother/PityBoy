#include "../headers/cli.hpp"

namespace PityBoy {
    template <typename Function>
    CLI CLI::bindArgument(Function func) {
        args.push_back(func)
        return this
    }

    template <typename Function>
    CLI CLI::bindFlag(char name, Function func) {
        flags.insert({name, func})
        return this
    }

    void CLI::parse(int argc, char **argv) {
        for (int i = 1; i<argc; i++) {
            std::string arg = (std::string)argv[i];
            bool isFlag = (arg.at(0) == '-' && arg.length() == 2 && flags.find(arg.at(1))!=flags.end());

            if (isFlag) {
                flags[arg.at(1)]();
            } else {
                
            }
        }
    }
}