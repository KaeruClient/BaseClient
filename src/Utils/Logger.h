#pragma once
#include "FileUtils.h"
#include "../GameData.h"
#include "../include/xorstr.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <cstdlib>
class Logger {
public:
    Logger() {};
    ~Logger() {};
    static void writelog(const char* format, ...) {
        /*char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf_s(buffer, sizeof(buffer), format, args);
        va_end(args);

        std::string filePath = getClientPath() + "\\Logs\\logs.txt";

        std::ofstream outputFile(filePath, std::ios::app);
        if (outputFile.is_open()) {
            outputFile << buffer << std::endl;
            outputFile.close();
        }*/
    }
};

#define DebugLog(str) OutputDebugStringA(str);