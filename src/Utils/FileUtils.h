#pragma once

#include <stdlib.h>
#include <string>
#include <direct.h>

static void makeFolder() {
    char* value;
    size_t size;
    _dupenv_s(&value, &size, "APPDATA");
    std::string path = value + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\SulfurV2");
    _mkdir(path.c_str());
    free(value);
}

static void makeAssetsFolder(std::string name) {
    char* value;
    size_t size;
    _dupenv_s(&value, &size, "APPDATA");
    std::string path = value + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\SulfurV2\\") + name;
    _mkdir(path.c_str());
    free(value);
}


inline static std::string getRoamingStatePath();

inline static std::string getClientPath();