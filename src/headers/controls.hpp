#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

#include <vector>
#include <sstream>
#include <math.h>

// PityBoy Editor stuff only

namespace PityBoy::controls {
    
    class Parent {
    
        public:
            virtual void drawPixel(int x, int y, int color) = 0;
            virtual void drawChar(int x,int y, char chr, int color) = 0;
            virtual void drawText(int x, int y, std::string str, int color) = 0;
            virtual void drawBox(int x, int y, int w, int h, int color, bool fill=false) = 0;

            virtual sf::Vector2i getMousePos() = 0;
            virtual bool isFocused() = 0;
            virtual void setMouseCursor(sf::Cursor *cursor) = 0;

    };

    class Control {
    
        public:
            virtual void draw(PityBoy::controls::Parent* myWindow) = 0;
            virtual void handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow) = 0;
    };

    // helper functions //
    bool isMouseOver(int x, int y, int w, int h, int mx, int my);
    //////


    class label : public Control {
        public:
            label(int x, int y, std::string text, int color=3);

            void draw(PityBoy::controls::Parent* myWindow);
            void handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow);

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

            void draw(PityBoy::controls::Parent* myWindow);
            void handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow);

            void setDimensions(int x,int y, int w,int h);
            void setText(std::string text);
            void setEnabled(bool enabled);
            void setColors(int textColor, int buttonColor, int buttonFillColor, int buttonPressedFillColor);

        private:
            int x, y, w, h;
            int textColor, buttonColor, buttonFillColor, buttonPressedFillColor ;
            bool enabled=true;
            bool held=false;
            std::string text;

            void (*callbackClicked)() = nullptr;
    };

    class textBox : public Control {
        public:
            textBox(int x, int y, int w,int h);

            void draw(PityBoy::controls::Parent* myWindow);
            void handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow);

            void setDimensions(int x,int y, int w,int h);
            void setConfig(bool allowNewLines, int maxLength);

        private:
            int x, y, w, h;
            int realW, realH;

            bool focused=false;
            bool held=false;

            sf::Cursor defaultCursor;
            sf::Cursor textCursor;
            bool lastCursor;

            int frameCounter=0;
            int cursorPosX=0;
            int cursorPosY=0;

            int scrollX=0;
            int scrollY=0;

            bool controlHeld=false;
            bool shiftHeld=false;

            int rememberXPos;

            // configuration
            bool allowNewLines=true; // if false, the textbox is one line input
            int maxLength=0; // 0 - infinite

            std::string text;
            std::vector<std::string> textLines;

            void convertTextIntoVector();
            int getTextLongestX(); // get "width of textbox" that is just length of longest line in textLines
            bool isTextCursorOnScreen();
            
            bool isCursorAtTheEnd();
            bool isCursorAtTheBegin();
            bool isCursorAtTheEndOfLine();

            void scrollToCursor();

            enum moveDirection {
                left,
                right,
                up,
                down
            };

            void typeChar(char c);
            void moveCursor(moveDirection dir);
            void moveCursorToWord(moveDirection dir, bool deleteWord=false); // ctrl+arrow left/right

            

    };


}