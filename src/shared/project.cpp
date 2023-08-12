#include "../headers/project.hpp" 

namespace fs = std::filesystem;

namespace PityBoy {
    int PBProjectManager::openProject(std::string path) {

        fs::path projectFolderPath = fs::path(path);

        if(!fs::is_directory(projectFolderPath)) { // check does the folder exists
            return -1;
        }

        fs::path projectInfoFile = projectFolderPath / fs::path("project.pity");

        if(!fs::exists(projectInfoFile)) { // check do project.pity exists
            return -2;
        }

        std::ifstream istreamInfoFile(projectInfoFile, std::ios::binary);
        if(istreamInfoFile.bad()) {
            return -3;
        }

        istreamInfoFile.read(reinterpret_cast<char*>(&projectStruct), sizeof(projectStruct));

        istreamInfoFile.close();
        

        return 1;
    }


}