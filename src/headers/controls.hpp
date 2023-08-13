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
            virtual void drawBox(int x, int y, int w, int h, int color, bool fill) = 0;

            virtual sf::Vector2i getMousePos() = 0;

    };

    class Control {
    
        public:
            virtual void setup() = 0;
            virtual void draw(PityBoy::controls::Parent* myWindow) = 0;
            virtual void handleEvent(sf::Event event) = 0;
    };



    class label : public Control {
        public:
            label(int x, int y, std::string text, int color=3);

            void setup();
            void draw(PityBoy::controls::Parent* myWindow);
            void handleEvent(sf::Event event);

            void setPosition(int x,int y);
            void setText(std::string text);
            void setColor(int color);

        private:
            int x, y, color;
            std::string text;
    };

    class button : public Control {
        public:
            button(int x, int y, int w,int h, std::string text);

            void registerCallback_clicked(void (*callback)());

            void setup();
            void draw(PityBoy::controls::Parent* myWindow);
            void handleEvent(sf::Event event);

            void setDimensions(int x,int y, int w,int h);
            void setText(std::string text);
            void setColor(int color);

        private:
            int x, y, w, h, color;
            bool enabled=true;
            std::string text;

            void (*callbackClicked)() = nullptr;
    };


}