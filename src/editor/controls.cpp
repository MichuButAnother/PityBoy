#include "../headers/controls.hpp" 

namespace PityBoy::controls {

    // label //

    label::label(int x, int y,std::string text, int color) {
        this->text = text;
        this->x = x;
        this->y = y;
        this->color = color;
    }

    void label::setup() {

    }

    void label::draw(PityBoy::controls::Parent* myWindow) {
        myWindow->drawText(x, y, text, color);
    }

    void label::handleEvent(sf::Event event) {

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

        this->color = 3;
        this->enabled = 1;
    }

    void button::setup() {

    }

    void button::draw(PityBoy::controls::Parent* myWindow) {
        myWindow->drawText(x, y, text, color);
    }

    void button::handleEvent(sf::Event event) {

    }

    void button::registerCallback_clicked(void (*callback)()) {
        callbackClicked = callback;
    }

}