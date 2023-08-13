#include "../headers/project.hpp" 

namespace fs = std::filesystem;

namespace PityBoy {
    int PBProjectManager::openProject(std::string path) {

        fs::path projectFolderPath = fs::path(path);

        if(!fs::is_directory(projectFolderPath)) { // check does the folder exists
            return -1;
        }

        fs::path projectInfoFile = projectFolderPath / fs::path("project.pity"); // <- look here, path merging

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

    int PBProjectManager::createProject(std::string name, std::string path) { // you just convert strint to fs::path() and merge these fs::path() with `/` operator name is the project name
        fs::path projectFolderPath = fs::path(path) / fs::path(name);

        if(!fs::is_directory(projectFolderPath)) {
            return -1;
        }

        if (!fs::create_directory(projectFolderPath)) {
            return -2;
        }

        
    }
}