#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <vector>

#include "../headers/controls.hpp"

namespace PityBoy {
    class PBWindow : PityBoy::controls::Parent {
        public:
            void initWindow(int resX, int resY, std::string windowName);

            void refreshPixelArray();

            bool isOpen();

            void eventTick(); // before draw
            void drawTick(); // draw and flush to screen

            // pityboy editor methods (control subsystem)
            template <typename T> void registerControl(T* t);

            sf::Vector2i getMousePos(); // get mouse pos after to pixel conversion
            bool isFocused();
            void setMouseCursor(sf::Cursor *cursor);

            // drawing methods

            void clear(int color=0);
            void drawPixel(int x, int y, int color);
            void drawChar(int x,int y, char chr, int color=3);
            void drawText(int x, int y, std::string str, int color=3);
            void drawBox(int x, int y, int w, int h, int color=3, bool fill = false);

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

            std::vector<PityBoy::controls::Control*> controls;
    };

    template <typename T> void PBWindow::registerControl(T* t) { // This cannot be defined in .cpp file due to C++ standards
        int i=0;
        bool putInNull=false;

        for(auto && e : controls) {
            if(e==nullptr) {
                putInNull=true;
                controls[i] = t;
                break;
            }
            i++;
        }

        if(!putInNull) {
            controls.push_back(t); 
        }
    }

}


