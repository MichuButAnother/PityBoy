#pragma once

#include <iostream>

#include "../headers/graphics.hpp"

namespace PityBoy::CommonAPI {

    void throwCodeError(std::string errorString, int line);  
    void throwError(std::string errorString);  
    
    void drawSprite(int x, int y, int index, int scale, int transparentColor); 

    void drawPixel(int x,int y, int color); 

    void drawText(int x, int y, const char *str, int color);

    extern PityBoy::PBWindow *mainWindow;
}
