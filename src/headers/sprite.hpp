#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <string.h>

#include "../headers/graphics.hpp"

namespace PityBoy { // but without PBSprite class like this?
    class PBSprites {
        public:

            void draw(PityBoy::PBWindow* window, int index, int drawX, int drawY, int scale=1, int transparentColor=-1); // draw sprite on screen
            uint8_t* getBytes(int index); // TO DO: as PityBoy is 4 color (2 bits per pixel), we should store sprites in 2bpp (bits per pixel) format. We don't store in program memory using 2bpp because it's simpler to operate on default data types like uint8_t
            void setBytes(int index, uint8_t* data); // these functions will do that (convert spriteData to 16 byte char array, because 8*8*2=128 bits and 128 bits = 16 bytes)
            
        private: 

            // 512 sprites, each sprite has 8x8 so 64 bytes for each
            // on map you can draw tiles (0-255) and on screen sprites (256-511)
            uint8_t spriteData[512][64]; 

    };    
}
