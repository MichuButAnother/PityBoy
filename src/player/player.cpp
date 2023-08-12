#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"  // we dont need icons in player
#include "../headers/graphics.hpp"
#include "../headers/sprite.hpp"

PityBoy::PBWindow win;

int main() {
    
    win.initWindow(160, 128, "PityBoy " + PityBoy::PB_Ver_Str);

    PityBoy::PBSprite testSprite;


    while (win.isOpen()) {
        win.eventTick();
        win.clear();

        testSprite.draw(&win, 10, 10);
        
        win.refreshPixelArray();
        win.drawTick();
    }
    return 0;
}