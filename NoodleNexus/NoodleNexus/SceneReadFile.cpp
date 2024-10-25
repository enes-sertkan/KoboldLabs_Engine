#pragma once
#include "KLFileManager.hpp"
#include <fstream>;
#include <iostream>;

void KLFileManager::WriteSceneFile(const Scene* scene, std::string fileName)
{
    // Open the file in truncation mode to overwrite the previous content
    std::ofstream myfile(fileName, std::ios::trunc);

    if (myfile.is_open())
    {
        myfile << "<KoboldLabs>\n";
        myfile << "<SceneFile>\n";
        myfile << "<ObjectLoad-->\n";

        // Write model files
        for (size_t i = 0; i < scene->modelInfos.size(); ++i)
        {
            const sModelDrawInfo& modelInfo = scene->modelInfos[i];
            myfile << modelInfo.meshPath << "\n";
        }
        myfile << "<--ObjectLoad>\n";

        myfile << "<Scene-->\n";

        for (size_t j = 0; j < scene->sceneObjects.size(); ++j)
        {
            sObject* object = scene->sceneObjects[j];
            myfile << "<Object-->\n";
            myfile << "<Name-> " << object->name << "\n";
            myfile << "<Model-> " << object->mesh->modelFileName << "\n";
            myfile << "<Position-> " << object->mesh->positionXYZ.x << " "
                << object->mesh->positionXYZ.y << " "
                << object->mesh->positionXYZ.z << "\n";
            myfile << "<Rotation-> " << object->mesh->rotationEulerXYZ.x << " "
                << object->mesh->rotationEulerXYZ.y << " "
                << object->mesh->rotationEulerXYZ.z << "\n";
            myfile << "<Scale-> " << object->mesh->uniformScale << "\n";
            myfile << "<Visibility-> " << (object->mesh->bIsVisible ? "true" : "false") << "\n";
            myfile << "<--Object>\n";
        }

        myfile << "<--Scene>\n";
        myfile << "</SceneFile>\n";
        myfile << "</KoboldLabs>\n";

        myfile.close();
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }
}