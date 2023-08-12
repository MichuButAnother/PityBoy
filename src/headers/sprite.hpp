#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <string.h>

#include "../headers/graphics.hpp"


namespace PityBoy {
    class PBSprite {
        public:
            PBSprite() {
                memset(spriteData,3,8*8); // test sprite draw
            }

            void draw(PityBoy::PBWindow* window, int drawX, int drawY, int scale=1, int transparentColor=-1); // draw sprite on screen

            void returnBytes(); // TO DO: as PityBoy is 4 color (2 bits per pixel), we should store sprites in 2bpp (bits per pixel) format. We don't store in program memory using 2bpp because it's simpler to operate on default data types like uint8_t
            void setBytes(); // these functions will do that (convert spriteData to 16 byte char array, because 8*8*2=128 bits and 128 bits = 16 bytes)

            uint8_t spriteData[8*8];

        private:

            

    };    
}
