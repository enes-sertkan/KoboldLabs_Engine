#pragma once

#include <fstream>
#include <iostream>
#include "KLFileManager.hpp" 

void PrintModelFileError(int errorCode, bool isRead)
{
    if (isRead)
        std::cout << "ModelFile reading ERROR #" << errorCode << std::endl;
    else
        std::cout << "ModelFile writing ERROR #" << errorCode << std::endl;
}

sModelDrawInfo ReadModelFile(const std::string& filePath)
{
    std::ifstream modelFile(filePath);
    std::string token = "";

    sModelDrawInfo modelInfo;

    if (!modelFile.is_open())
    {
        PrintModelFileError(1, true);
        return modelInfo; 
    }

    modelFile >> token;

    if (token != "<KoboldLabs>")
    {
        PrintModelFileError(2, true);
        return modelInfo;
    }

    // Read until we find the <ModelFile> tag
    while (token != "<ModelFile>" && !modelFile.eof())
    {
        modelFile >> token;
    };

    // Read until we find the <name> tag
    while (token != "<name>" && !modelFile.eof())
    {
        modelFile >> token;
    };

    if (modelFile.eof())
    {
        PrintModelFileError(3, true);
        return modelInfo;
    }

    modelFile >> modelInfo.modelName;

    // Read until we find the <filePath> tag
    while (token != "<filePath>" && !modelFile.eof())
    {
        modelFile >> token;
    };

    if (!modelFile.eof())
    {
        modelFile >> modelInfo.meshPath;
    }

    return modelInfo;
}

//void WriteModelFile(sModelDrawInfo* model)
//{
//
//}