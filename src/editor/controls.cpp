#include "../headers/controls.hpp" 

#include <iostream>

namespace PityBoy::controls {

    // helper functions // 

    bool isMouseOver(int x, int y, int w, int h, int mx, int my) {
        if(mx>=x && my>y && mx < x+w && my < y+h) return true;
        return false;
    }

    // label //

    label::label(int x, int y,std::string text, int color) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->color = color;
    }


    void label::draw(PityBoy::controls::Parent* myWindow) {
        myWindow->drawText(x, y, text, color);
    }

    void label::handleEvent(__attribute__((unused)) sf::Event event, __attribute__((unused)) PityBoy::controls::Parent* myWindow) { // to hide "unused variable" error 

    }

    void label::setPosition(int x,int y) {
        this->x = x;
        this->y = y;
    }

    void label::setText(std::string text) {
        this->text = text;
    }
    
    void label::setColor(int color) {
        this->color = color;
    }

    // button //

    button::button(int x, int y, int w,int h, std::string text) {
        this->text = text;
        
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;

        this->textColor = 3;
        this->buttonColor = 3;
        this->buttonFillColor = 1;
        this->buttonPressedFillColor = 2;
        this->enabled = 1;
    }


    void button::draw(PityBoy::controls::Parent* myWindow) {
        sf::Vector2i mousePos = myWindow->getMousePos();

        if(held && isMouseOver(x, y, w, h, mousePos.x, mousePos.y)) { // fill draw
            myWindow->drawBox(x, y, w, h, buttonPressedFillColor, true); 
        } else {
            if(enabled) {
                myWindow->drawBox(x, y, w, h, buttonFillColor, true); 
            } else {
                myWindow->drawBox(x, y, w, h, 2, true); 
            }
        }
        if(enabled) { // outline
            myWindow->drawBox(x, y, w, h, buttonColor); 
        } else {
            myWindow->drawBox(x, y, w, h, 1); 
        }

        // draw text at middle
        int textPixelLength = text.length() * 8;
        int dx = x + (w / 2) - (textPixelLength / 2);
        int dy = y + (h / 2) - 4;
        if(enabled) {
            myWindow->drawText(dx, dy, text, textColor);
        } else {
            myWindow->drawText(dx, dy, text, 1);
        }
    }
   

    void button::handleEvent(sf::Event event, PityBoy::controls::Parent* myWindow) {

        if(event.type == sf::Event::MouseButtonPressed) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                if(!enabled) {
                    held = false;
                    return;
                }
                sf::Vector2i mousePos = myWindow->getMousePos();
                
                if(isMouseOver(x, y, w, h, mousePos.x, mousePos.y)) {
                    held = true;
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased) {
            if(event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = myWindow->getMousePos();
                if(isMouseOver(x, y, w, h, mousePos.x, mousePos.y) && held) {
                    if(callbackClicked!=nullptr) callbackClicked();
                }
                held = false;
            }
        }
    }

    void button::registerCallback_clicked(void (*callback)()) {
        callbackClicked = callback;
    }

    void button::setColors(int textColor, int buttonColor, int buttonFillColor, int buttonPressedFillColor) {
        this->textColor = textColor;
        this->buttonColor = buttonColor;
        this->buttonFillColor = buttonFillColor;
        this->buttonPressedFillColor = buttonPressedFillColor;
    }

    void button::setDimensions(int x,int y, int w,int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    void button::setText(std::string text) {
        this->text = text;
    }
    void button::setEnabled(bool enabled) {
        this->enabled = enabled;
    }


    
}