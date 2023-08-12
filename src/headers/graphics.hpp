#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

namespace PityBoy {
    class PBWindow {
        public:
            void initWindow(int resX, int resY, std::string windowName);

            void refreshPixelArray();

            bool isOpen();

            void eventTick(); // before draw
            void drawTick(); // draw and flush to screen

            // drawing methods

            void clear(int color=0);
            void drawPixel(int x, int y, int color);
            void drawChar(int x,int y, char chr, int color=3);
            void drawText(int x, int y, std::string str, int color=3);

        private:
            sf::RenderWindow* window;

            // Pixel drawing of PityBoy
            uint8_t *pixels;
            // Texture and sprite to render the pixel array
            sf::Texture* renderTexture; 
            sf::Sprite* renderSprite;

            uint8_t palette[4][3]; // local palette

            int defaultScaling=3;

            int windowSizeX;
            int windowSizeY;
    };
    
}