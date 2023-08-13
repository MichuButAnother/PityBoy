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

    bool LuaEngine::execute(std::string code) {
        int preresult = luaL_loadstring(L, code.c_str()); 

        if (preresult != 0) {
            fprintf(stderr, "An error occurred: %s\n", lua_tostring(L,-1));
            return false;
        }

        for(auto const& [key,value] : envMap) { 
            lua_register(L, key.c_str(), value);
        } 
        
        int result = lua_pcall(L, 0, LUA_MULTRET, 0);
        
        if (result == 0) {
            lua_pop(L, -1);
            return true;
        } else {
            fprintf(stderr, "(2) An error occurred: %s\n", lua_tostring(L,-1));
            return false; 
        }
    }

    void LuaEngine::initApi() {
        std::map<std::string, lua_CFunction> env = {
            {"pixel", LuaEngine::l_drawPixel},
            {"text", LuaEngine::l_drawText},
        }; 
        loadEnv(env); 
    }

    bool LuaEngine::call(std::string fn) {
        lua_getglobal(L, fn.c_str());

        bool s = true;

        if (lua_isfunction(L, -1)) {
            int result = lua_pcall(L, 0,0,0);
            if (result!=0) {
                s = false;
                CommonAPI::throwError((std::string)lua_tostring(L, -1)); 
            }
        } else {
            s = false;
            CommonAPI::throwError("'"+fn+"' does not exist / is not a function."); 
        }

        lua_pop(L, -1);
        return s;
    }

    bool LuaEngine::pcall(std::string fn) { 
        lua_getglobal(L, fn.c_str());

        if (lua_isfunction(L, -1)) {
            int result = lua_pcall(L, 0,0,0);
            if (result==0) {
                return true;
            } else {
                //CommonAPI::throwError("Pcall error: ");
                return false;
            }
        } else {
            //CommonAPI::throwError("");
            return false; 
        } // dude just use bools then :/
    }

    int LuaEngine::getCurrentLine(lua_State *L) {
        lua_Debug ar;
        lua_getstack(L, 1, &ar);
        lua_getinfo(L, "nSl", &ar);
        return ar.currentline;
    }

    /////// API ////////

    void LuaEngine::assertArgs(lua_State *L, int n) {
        int args = lua_gettop(L);
        if (args!=n) {
            CommonAPI::throwCodeError("Invalid arguments",getCurrentLine(L));
        }
    }

    int LuaEngine::l_drawPixel(lua_State *L) {
        assertArgs(L, 3);
        int x=lua_tointeger(L, 1);
        int y=lua_tointeger(L, 2);
        int c=lua_tointeger(L, 3);
        CommonAPI::drawPixel(x,y,c);

        return 0;
    }

    int LuaEngine::l_drawText(lua_State *L) {
        assertArgs(L, 4);

        int x = lua_tointeger(L, 1);
        int y = lua_tointeger(L, 2);
        const char *str = lua_tostring(L, 3);
        int color = lua_tointeger(L, 4);

        CommonAPI::drawText(x,y,str,color);
    } 
}