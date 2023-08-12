#pragma once

#include <stdint.h>
#include <string>
#include <filesystem>
#include <fstream>

namespace PityBoy {

    struct PBProjectStruct {
        uint8_t PBLogo[32]; 
        int16_t PBVersion;
        char PBProjectName[32];
    };

    class PBProjectManager {
        public:
            int openProject(std::string path);

            int compileToCartridge();

        private:
            bool projectLoaded;

            // later copied to PBProjectStruct
            std::string projectName; 

            std::string folderPath;

            PBProjectStruct projectStruct;

    };

    
}