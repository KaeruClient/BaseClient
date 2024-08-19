#include "FileUtils.h"

#include "../GameData.h"

std::string getRoamingStatePath()
{
	char* value;
	size_t size;
	_dupenv_s(&value, &size, "AppData");

	std::string roamingPathCStr = value + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\";
	free(value);
	return roamingPathCStr;
}

std::string getClientPath()
{
	return getRoamingStatePath() + ClientData::getClientName() + "\\";
}