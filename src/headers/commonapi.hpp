#pragma once

#ifndef __COMMONAPI_HPP__
#define __COMMONAPI_HPP__

#include <iostream>

#include "../headers/graphics.hpp"

namespace PityBoy::CommonAPI {

    void throwError(std::string errorString);  

    void drawSprite(int x, int y, int index, int scale, int transparentColor); 

    void drawPixel(int x,int y, int color); 

    extern PityBoy::PBWindow *mainWindow;
}
#endif 