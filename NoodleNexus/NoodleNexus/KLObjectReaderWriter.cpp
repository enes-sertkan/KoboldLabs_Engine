#pragma once

#include <fstream>
#include <iostream>
#include "KLFileManager.hpp" 

void PrintSceneFileError(int errorCode, bool isRead)
{
    if (isRead)
        std::cout << "SceneFile reading ERROR #" << errorCode << std::endl;
    else
        std::cout << "SceneFile writing ERROR #" << errorCode << std::endl;
}

sModelDrawInfo ReadObjectFile(const std::string& filePath)
{
    std::ifstream objectFile(filePath);
    std::string token = "";

    sModelDrawInfo modelInfo;

    if (!objectFile.is_open())
    {
        PrintSceneFileError(1, true);
        return modelInfo; 
    }

    objectFile >> token;

    if (token != "<KoboldLabs>")
    {
        PrintSceneFileError(2, true);
        return modelInfo;
    }

    // Read until we find the <ObjectFile> tag
    while (token != "<ObjectFile>" && !objectFile.eof())
    {
        objectFile >> token;
    };

    // Read until we find the <name> tag
    while (token != "<name>" && !objectFile.eof())
    {
        objectFile >> token;
    };

    if (objectFile.eof())
    {
        PrintSceneFileError(3, true);
        return modelInfo;
    }

    objectFile >> modelInfo.modelName;

    // Read until we find the <filePath> tag
    while (token != "<filePath>" && !objectFile.eof())
    {
        objectFile >> token;
    };

    if (!objectFile.eof())
    {
        objectFile >> modelInfo.meshPath;
    }

    return modelInfo;
}
