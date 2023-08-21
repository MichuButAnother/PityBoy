#include <iostream>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> 

#include "../headers/shared.hpp" 
#include "../headers/font.hpp"
#include "../headers/icons.hpp"
#include "../headers/graphics.hpp"
//#include "../headers/sprite.hpp"

#include "../headers/luaapi.hpp"
#include "../headers/commonapi.hpp"


PityBoy::PBWindow win;
PityBoy::controls::label testLabel(10,10,"Hello world!",2);
PityBoy::controls::button testButton(10,40,120,20,"Test button");
PityBoy::controls::textBox testTextBox(0,64,30,10);

int a=0;

void callbackTest() {
    a++;
    testLabel.setText("You clicked the button " + to_string(a) + "times!");
}

int main() {
    win.initWindow(256, 160, "PityBoy Editor " + PityBoy::PB_Ver_Str);
    win.registerControl(&testLabel);
    win.registerControl(&testButton);
    win.registerControl(&testTextBox);

    //testTextBox.setConfig(false, 10);

    testButton.registerCallback_clicked(callbackTest);

    while (win.isOpen()) {
        win.eventTick();

        win.clear();

        win.drawTick();
    }
    return 0;
}