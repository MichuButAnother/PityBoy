#include "../headers/commonapi.hpp"

namespace PityBoy::CommonAPI { 
    void throwCodeError(std::string error, int line) {
        std::cout<<"Lua error at line "<<line<<": "<<error<<std::endl;
        exit(0);
    } 
    void throwError(std::string error) {
        std::cout<<error<<std::endl;
        exit(0);
    } 

    void drawPixel(int x,int y, int color) {
        mainWindow->drawPixel(x, y, color);
    }

    void drawText(int x, int y, const char* str, int color) {
        mainWindow->drawText(x,y,(std::string)str, color);
    }

    PityBoy::PBWindow *mainWindow = nullptr;
}