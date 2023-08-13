#pragma once

#include <lua5.4/lua.hpp>
#include <map>
#include <string>
#include <vector>
#include <functional>

#include "../headers/commonapi.hpp"
#include "../headers/commonapi.hpp"

namespace PityBoy {
    class LuaEngine {
        public:
            LuaEngine();
            bool state;

            void loadEnv(std::map<std::string, lua_CFunction> env); 
            int execute(std::string compile);
        
            void initApi();

            // API //
            static int getCurrentLine(lua_State *L);

            static int l_drawPixel(lua_State *L);

        private:  
            lua_State* L;
            std::map<std::string, lua_CFunction> envMap;
            
            std::vector<std::string> excludeGlobal = {"io", "os", "dofile", "getfenv", "load", "loadfile", "loadstring", "rawget", "rawset", "rawequal",
                                          "coroutine", "package", "require", "debug", "getmetatable", "setmetatable", "newproxy"};
            
    };
}