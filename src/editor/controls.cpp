#include "../headers/controls.hpp" 

namespace PityBoy::controls {

    label::label(int x, int y,std::string text) {
        this->text = text;
        this->x = x;
        this->y = y;
    }

    void label::setup() {

    }

    void label::registerCallback_clicked(void (*callback)()) {
        callbackClicked = callback;
    }

    void label::draw(PityBoy::controls::Parent* myWindow) {
        myWindow->drawText(x, y, text, 3);
    }


    void label::handleEvent(sf::Event event) {

    }
}