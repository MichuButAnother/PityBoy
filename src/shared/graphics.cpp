#include "../headers/graphics.hpp"
#include "../headers/shared.hpp" 

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 
#include <string.h>

namespace PityBoy {
    void PBWindow::initWindow(int resX, int resY, std::string windowName) {
        windowSizeX=resX;
        windowSizeY=resY;
        
        // copy the global palette into our palette
        memcpy(this->palette,PityBoy::palette,sizeof(PityBoy::palette));

        // Pixel array init
        pixels = new uint8_t[resX * resY * 4];

        for(int i=0; i < resX*resY*4; i=i+4) { // clear the pixell array with first palette color and set alpha too
            pixels[i] = palette[0][0]; // R
            pixels[i+1] = palette[0][1]; // G
            pixels[i+2] = palette[0][2]; // B
            pixels[i+3] = 255; // A
        }
        
        // Sprite and texture init to draw the pixel array
        renderTexture = new sf::Texture();
        renderTexture->create(resX,resY);

        renderSprite = new sf::Sprite();
        renderSprite->setTexture(*renderTexture);

        refreshPixelArray();

        // Window creation
        window = new sf::RenderWindow(sf::VideoMode(resX,resY), windowName);

        // set some window properties
        window->setFramerateLimit(60);

        window->setSize(sf::Vector2u(resX*defaultScaling,resY*defaultScaling));
    }

    void PBWindow::refreshPixelArray() { // flush pixels to the render texture
        renderTexture->update(pixels);   
    }

    bool PBWindow::isOpen() {
        return window->isOpen();
    }

    void PBWindow::eventTick() {
        sf::Event event;

        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            }
        }

    }


    void PBWindow::drawTick() {
        window->draw(*renderSprite);
        window->display();    
    }


    ////// drawing methods //////

    void PBWindow::clear(int color) {
        if(color < 0 || color >= 4) return; // check valid color index
        
        for(int i=0; i < windowSizeX*windowSizeY*4; i=i+4) { 
            pixels[i] = palette[color][0]; // R
            pixels[i+1] = palette[color][1]; // G
            pixels[i+2] = palette[color][2]; // B
            pixels[i+3] = 255; // A
        }
    }

    void PBWindow::drawPixel(int x, int y, int color) {
        if(x < 0 || x >= windowSizeX || y < 0 || y > windowSizeY) return; // check boundaries
        if(color < 0 || color >= 4) return; // check valid color index

        // set the pixel
        pixels[((x + (y * windowSizeX)) * 4) + 0] = this->palette[color][0];
        pixels[((x + (y * windowSizeX)) * 4) + 1] = this->palette[color][1];
        pixels[((x + (y * windowSizeX)) * 4) + 2] = this->palette[color][2];
    }
}

