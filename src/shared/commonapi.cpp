#include "../headers/commonapi.hpp"

namespace PityBoy::CommonAPI { 
    void throwError(std::string error) {
        std::cout<<"Lua error: "<<error<<std::endl;
        exit(0);
    } 

    void drawPixel(int x,int y, int color) {
        mainWindow->drawPixel(x, y, color);
    }

    PityBoy::PBWindow *mainWindow = nullptr;
}