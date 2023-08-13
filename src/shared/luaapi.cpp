#include <lua5.4/lua.hpp>
#include <filesystem>
#include <iostream>

#include "../headers/luaapi.hpp"

// the file structure would be like this:
    // 1. pityboy logo as header and display at boot
    // 2. sprites
    // 3. maps
    // 4. sfx
    // 5. music
    // 6. lua code

// for start lets just make that main.lua will be at the end and rest files will be added before
    // .pityboy <- pitboy project
    // .spr.pit <- uncompiled raw sprites
    // .map.pit <- uncompiled raw map
    // .sfx.pit <- uncompiled raw sfx
    // all this ^^ would be packed into .rom
    // .rom <- cartridge

namespace PityBoy {

    LuaEngine::LuaEngine() {
        L = luaL_newstate();
        luaL_openlibs(L);

        for(auto &fname : excludeGlobal) {
            lua_setglobal(L, fname.c_str());
            lua_pushnil(L);
        }
    }

    void LuaEngine::loadEnv(std::map<std::string, lua_CFunction> Enviorment) {
        envMap = Enviorment;
    }

    int LuaEngine::execute(std::string code) {
        int preresult = luaL_loadstring(L, code.c_str()); 

        if (preresult != 0) {
            fprintf(stderr, "An error occurred: %s\n", lua_tostring(L,-1));
            return 1;
        }

        for(auto const& [key,value] : envMap) { 
            lua_register(L, key.c_str(), value);
        } 
        
        int result = lua_pcall(L, 0, LUA_MULTRET, 0);
        
        if (result == 0) {
            lua_pop(L, -1);
            return 0;
        } else {
            fprintf(stderr, "(2) An error occurred: %s\n", lua_tostring(L,-1));
            return 1; 
        }
    }

    void LuaEngine::initApi() {
        std::map<std::string, lua_CFunction> env = {
            {"drawPixel", LuaEngine::l_drawPixel} 
        }; 
        loadEnv(env); 
    } // we would have also to allow only calling drawing functions inside render(), is that possible?

    int LuaEngine::call(std::string fn) {
        lua_getglobal(L, fn.c_str());
        switch (lua_type(L, -1)) {
            case LUA_TFUNCTION:
            {
                int result = lua_pcall(L, 0,0,0);
                if (result==0) {
                    return 0;
                } else {
                    
                }
                break;
            }
            default: 
                CommonAPI::throwError("attempt to call a value other than function in LuaEngine::call"); 
                break; 
        }
    }

    int LuaEngine::getCurrentLine(lua_State *L) {
        lua_Debug ar;
        lua_getstack(L, 1, &ar);
        lua_getinfo(L, "nSl", &ar);
        return ar.currentline;
    }

    /////// API ////////

    int LuaEngine::l_drawPixel(lua_State *L) {
        int args = lua_gettop(L);
        if(args!=3) { 
            CommonAPI::throwCodeError("Invalid arguments",getCurrentLine(L));
        }
        int x=lua_tointeger(L, 1);
        int y=lua_tointeger(L, 2);
        int c=lua_tointeger(L, 3);
        CommonAPI::drawPixel(x,y,c);

        return 0;
    }
}