#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"
#include "../headers/icons.hpp"
#include "../headers/graphics.hpp"

PityBoy::PBWindow win;

int main() {
    
    win.initWindow(256, 160, "PityBoy Editor " + PityBoy::PB_Ver_Str);

    while (win.isOpen()) {
        win.eventTick();

        win.drawTick();
    }
    return 0;
}