#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <lua5.4/lua.hpp> 
#include <filesystem>

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"
#include "../headers/graphics.hpp"
#include "../headers/sprite.hpp"
#include "../headers/luaapi.hpp"
#include "../headers/commonapi.hpp"

PityBoy::PBWindow win;

int main(int argc, char* argv[]) {
    win.initWindow(160, 128, "PityBoy " + PityBoy::PB_Ver_Str);

    PityBoy::PBSprite testSprite;
    PityBoy::CommonAPI::mainWindow = &win;
    PityBoy::LuaEngine engine; 

    if (argv[0] == "-h") { // it was index 0 right compile again    
        std::cout << "placeholder" << std::endl;
    } else {
        std::filesystem::path cartPath = std::filesystem::path(argv[1]);

        if (std::filesystem::exists(cartPath) && std::filesystem::is_regular_file(cartPath) && cartPath.extension() == "\".pit\"") {
            std::cout << "yeah its fineeee" << std::endl;
        } else {
            std::cout << "invalid path for cartridge" << std::endl;
        }
    }

    engine.initApi();
    engine.execute("function render() text(10,10,'im so fucking cool', 1) end");

    while (win.isOpen()) {
        win.eventTick();
        win.clear();
        
        engine.call("render");

        win.drawTick(); 
    }
    return 0;
}