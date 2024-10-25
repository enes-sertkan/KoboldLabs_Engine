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

sModelDrawInfo KLFileManager::ReadModelFile(const std::string& filePath)
{
    std::ifstream modelFile(filePath);
    std::string token;

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
    }

    // Read until we find the <name> tag
    while (token != "<name>" && !modelFile.eof())
    {
        modelFile >> token;
    }

    if (!modelFile.eof())
    {
        modelFile >> modelInfo.modelName;  // Read the model name
    }

    // Read until we find the <filePath> tag
    while (token != "<filePath>" && !modelFile.eof())
    {
        modelFile >> token;
    }

    if (!modelFile.eof())
    {
        modelFile >> modelInfo.meshPath;  // Read the file path
    }

    // Read until we find the <fileType> tag
    while (token != "<fileType>" && !modelFile.eof())
    {
        modelFile >> token;
    }

    if (!modelFile.eof())
    {
        modelFile >> modelInfo.fileType;  // Read the file type
    }

    modelFile.close();  // Always good practice to close files
    return modelInfo;
}

void AddLineToModelFile(std::ofstream& myfile, const std::string& line)
{
    if (myfile.is_open())
    {
        myfile << line << "\n";
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }
}


void KLFileManager::WriteModelFile(const sModelDrawInfo* model, std::string fileName, std::string type)
{
    // Open the file in truncation mode to overwrite the previous content
    std::ofstream myfile(fileName, std::ios::trunc);

    if (myfile.is_open())
    {
        myfile << "<KoboldLabs>\n";
        myfile << "<ModelFile>\n";

        // Add spaces or newlines to make it more readable
        myfile << "<name> " << model->modelName << "\n";     // Added space after tag
        myfile << "<filePath> " << model->meshPath << "\n";  // Added space after tag

        // Directly write the fileType as a string
        myfile << "<fileType> " << type << "\n"; // Use the string directly

        myfile << "</ModelFile>\n";
        myfile << "</KoboldLabs>\n";

        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }
}
