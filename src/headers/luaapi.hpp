#pragma once

#include <lua5.4/lua.hpp>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <optional>

#include "../headers/commonapi.hpp"
#include "../headers/commonapi.hpp"

namespace PityBoy {
    enum LuaType {
        nil,
        boolean,
        lightuserdata,
        number,
        string,
        table,
        function,
        userdata,
        thread
    };

    class LuaEngine {
        public:
            LuaEngine();
            bool state;

            void loadEnv(std::map<std::string, lua_CFunction>); 
            bool execute(std::string);
            bool call(std::string);
            bool pcall(std::string);
            LuaType type(std::string);
            
            void initApi();


            // API //
            static void assertArgs(lua_State *L,int n);

            static int getCurrentLine(lua_State *L);

            static int l_drawPixel(lua_State *L); 

            static int l_drawText(lua_State *L);

        private:  
            lua_State* L;
            std::map<std::string, lua_CFunction> envMap;
            
            std::vector<std::string> excludeGlobal = {"io", "os", "dofile", "getfenv", "load", "loadfile", "loadstring", "rawget", "rawset", "rawequal",
                                          "coroutine", "package", "require", "debug", "getmetatable", "setmetatable", "newproxy"};
            
    };
}