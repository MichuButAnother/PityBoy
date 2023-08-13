#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

// PityBoy Editor stuff only

namespace PityBoy::controls {
    
    class Parent {
    
        public:
            virtual void drawPixel(int x, int y, int color) = 0;
            virtual void drawChar(int x,int y, char chr, int color) = 0;
            virtual void drawText(int x, int y, std::string str, int color) = 0;

    };

    class Control {
    
        public:
            virtual void setup() = 0;
            virtual void draw(PityBoy::controls::Parent* myWindow) = 0;
            virtual void handleEvent(sf::Event event) = 0;

    };



    class label : public Control {
        public:
            label(int x, int y, std::string text);

            void registerCallback_clicked(void (*callback)());

            void setup();
            void draw(PityBoy::controls::Parent* myWindow);
            void handleEvent(sf::Event event);

        private:
            int x, y;
            std::string text;

            void (*callbackClicked)() = nullptr;
    };


}