#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"
#include "../headers/icons.hpp"
#include "../headers/graphics.hpp"
#include "../headers/sprite.hpp"

#include "../headers/luaapi.hpp"
#include "../headers/commonapi.hpp"


PityBoy::PBWindow win;
PityBoy::controls::label testLabel(10,10,"Hello world!");

int main() {
    win.initWindow(256, 160, "PityBoy Editor " + PityBoy::PB_Ver_Str);
    win.registerControl(&testLabel);

    while (win.isOpen()) {
        win.eventTick();

        win.drawTick();
    }
    return 0;
}