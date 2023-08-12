#include "../headers/sprite.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <string.h>

namespace PityBoy {
    void PBSprite::draw(PityBoy::PBWindow* window, int drawX, int drawY, int scale, int transparentColor) {
        // "weird" for loops for sprite scaling support

        // sprite store ordering:
        // 0 1 2 3 
        // 4 5 6 7
        // ...

        int i=0; // pixel data index
        for(int y = 0; y < 8*scale ; y=y+scale) {
            for(int x = 0; x < 8*scale; x=x+scale) {
                int color=spriteData[i];
                i++;

                if(color!=transparentColor) { // is the pixel color market as transparent? if yes then dont' draw

                    for(int sx=0; sx<scale; sx++) { // scaled drawing logic
                        for(int sy=0; sy<scale; sy++) {
                            window->drawPixel(drawX+x+sx, drawY+y+sy, color); 
                        }
                    }
                    
                }

            }   
        }

    }
}