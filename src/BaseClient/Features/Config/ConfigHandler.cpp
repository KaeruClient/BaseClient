#include "ConfigHandler.h"
#include "../Module/ModuleHandler.h"
#include "../../../Utils/JsonUtils.h"
#include "../../../Utils/Utils.h"
nlohmann::json ConfigHandler::jsonConfig;
bool ConfigHandler::Save() {
    modules.SaveConfig(&jsonConfig);
    const std::string fullPath = Utils::getClientPath() + "Configs\\" + fileName + ".json";
    std::ofstream writing_file(fullPath.c_str(), std::ifstream::binary);
    logF("Saving config in \"%s\"", fullPath.c_str());
    writing_file << std::setw(4) << jsonConfig << std::endl;
    writing_file.flush();
    writing_file.close(); 
    return true;
}

bool ConfigHandler::Load(std::string config, bool create) {
    const std::string fullPath = Utils::getClientPath() + "Configs\\" + config + ".json";
    const bool hasConfig = std::filesystem::exists(fullPath);
    if (hasConfig || create) {
        if (fileName != config)
        {
            Save();
            fileName = config;
        }
        if (hasConfig) {
            std::ifstream writing_file(fullPath.c_str(), std::ifstream::binary);
            try {
                jsonConfig.clear();
                writing_file >> jsonConfig;
            }
            catch (nlohmann::json::parse_error& e) {
                logF("Json parse error : %s", e.what());
                return false;
            }

            modules.LoadConfig(&jsonConfig);
        } else if (create) Save();
        
        const std::string configName = Utils::getClientPath() + "Configs\\LatestConfig.txt";

        std::ofstream confName(configName.c_str());
        confName << config;
        confName.flush();
        confName.close();
    }
    return false;
}

void ConfigHandler::Initialize() {
    const std::string fullPath = Utils::getClientPath() +  "Configs\\LatestConfig.txt";
    const bool hasConfig = std::filesystem::exists(fullPath);
    std::string config = defaultConfig;
    if (hasConfig) {
        std::ifstream writing_file(fullPath.c_str());
        writing_file >> config;
        writing_file.close();
    }

    logF("Initializing ConfigHandler...\nLoaded:%s", config);
    Load(config, true);
}