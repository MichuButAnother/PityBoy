#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"  // we dont need icons in player
#include "../headers/graphics.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800,600), "PityBoy Editor");

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.display();
    }
    return 0;
}