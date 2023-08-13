#include "../headers/commonapi.hpp"

namespace PityBoy::CommonAPI { 
    void throwError(std::string error, int line) {
        std::cout<<"Lua error at line "<<line<<": "<<error<<std::endl;
        exit(0);
    } 

    void drawPixel(int x,int y, int color) {
        mainWindow->drawPixel(x, y, color);
    }

    PityBoy::PBWindow *mainWindow = nullptr;
}