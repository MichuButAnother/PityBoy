#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <lua5.4/lua.hpp> 

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"
#include "../headers/graphics.hpp"
#include "../headers/sprite.hpp"
#include "../headers/luaapi.hpp"
#include "../headers/commonapi.hpp"

PityBoy::PBWindow win;

int main() {
    win.initWindow(160, 128, "PityBoy " + PityBoy::PB_Ver_Str);

    PityBoy::PBSprite testSprite;
    PityBoy::CommonAPI::mainWindow = &win;
    PityBoy::LuaEngine engine; 

    engine.initApi();
    
    while (win.isOpen()) {
        win.eventTick();
        win.clear();
        
        engine.execute("drawPixel(10,10,3)"); 

        win.drawTick(); 
    }
    return 0;
}