#include "../headers/sprite.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <string.h>

namespace PityBoy {
    PBSprites::PBSprites() {
        memset(spriteData, 0, sizeof(uint8_t)*512*64);
    }

    void PBSprites::draw(PityBoy::PBWindow* window, int index, int drawX, int drawY, int scale, int transparentColor) {
        // "weird" for loops for sprite scaling support

        // sprite store ordering:
        // 0 1 2 3 
        // 4 5 6 7
        // ...

        int i=0; // pixel data index
        for(int y = 0; y < 8*scale ; y+=scale) {
            for(int x = 0; x < 8*scale; x+=scale) {
                int color=spriteData[index][i];
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

    uint8_t* PBSprites::getBytes(int index) { 
        return spriteData[index];
    }

    void PBSprites::setBytes(int index, uint8_t* data) { 
        memcpy(spriteData[index], data, sizeof(uint8_t)*64);  

    }
}