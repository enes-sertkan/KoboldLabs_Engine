#include "KLFileManager.hpp";
#include <fstream>;
#include <iostream>;
#include "sharedThings.h"




void PrintSceneFileError(int errorCode, bool isRead)
{
    if (isRead) printf("SceneFile reading ERROR #" + errorCode+'\n');
    else std::cout << "SceneFile writing ERROR #" + errorCode<< std::endl;
}

std::string SumTokensUntil(std::ifstream& file, char charToStop)
{
    std::string token = "";
    std::string finalString;


    while (file.peek() != '\n')
    {
        file >> token;
        finalString += token;
    }

    return finalString;
}

glm::vec3 LoadVector3Data(std::ifstream& file)
{
    std::string token = "";
    glm::vec3 finalVector;

 
    file >> finalVector.x;
    file >> finalVector.y;
    file >> finalVector.z;

    return finalVector;

}

glm::vec4 LoadVector4Data(std::ifstream& file)
{
    std::string token = "";
    glm::vec4 finalVector;


    file >> finalVector.x;
    file >> finalVector.y;
    file >> finalVector.z;
    file >> finalVector.w;

    return finalVector;
}

bool LoadBoolData(std::ifstream& file)
{
    std::string token = "";

    file >> token;
    if (token == "true") return true;
    if (token == "false") return false;

    return false;
}



Scene* KLFileManager::ReadSceneFile(std::string filePath)
{

    std::ifstream sceneFile(filePath);     // May also see .c_str() "c style string, char*"
    std::string token = "";

    Scene* scene = new Scene;
    scene->lightManager = new cLightManager();

    if (!sceneFile.is_open())
    {
        PrintSceneFileError(0, true);
        return nullptr;
    }

    sceneFile >> token;

    if (token != "<KoboldLabs>")
    {
        std::cout << "Scene File Header is wrong. Possibly not KoboldLabs File. Closing."<<std::endl;
        return nullptr;
    }

    sceneFile >> token;

    if (token != "<SceneFile>")
    {
        std::cout << "Scene File Header is wrong. Possibly not Scene File(but definetly KoboldLabs file). Closing." << std::endl;
        return nullptr;
    }

  

    sceneFile >> token;



//-----------------------------------------------------
    if (token != "<ObjectLoad-->")
    {
        std::cout << "No ObjectLoad start. So there will be no objects to load. CLosing." << std::endl;
            return nullptr;
    }

    sceneFile >> token;
    
    while(token!= "<--ObjectLoad>")
    {
        if (sceneFile.peek() == EOF)
        {
            PrintSceneFileError(4, true);
          return nullptr;
        }
           
         //filepath to objfile
        scene->modelInfos.push_back(ReadModelFile(token)); //we load the file(pls DO  OT USE SPACES IN THE PATH FOR NOW
        //TODO: FIX THE THING WITH SPACES
        

        sceneFile >> token;
    }

    sceneFile >> token;

    if (token != "<Scene-->")
    {
        PrintSceneFileError(5, true);
        return nullptr;
    }


    while (token != "<--Scene>")
    {

        if (sceneFile.peek() == EOF)
        {
            PrintSceneFileError(6, true);
            //return nullptr;
        }


        sceneFile >> token;

        //LOAD OBJECT
        if (token == "<Object-->")
        {
            sObject* object = new sObject();
            object->mesh = new sMesh();
            while (token != "<--Object>")
            {
                if (sceneFile.peek() == EOF)
                {
                    PrintSceneFileError(7, true);
                    return nullptr;
                }

                
               
               

                sceneFile >> token;

                if (token == "<Name->")
                {
                    object->name = SumTokensUntil(sceneFile, '\n'); //Object Name     //Not ssure if this will work
                    object->mesh->uniqueFriendlyName = object->name; //TODO: THis is so bad. Object should have on;y one name. Mesh Should not probably.
                }
                if (token == "<Model->")
                {
                   std::string modelName = SumTokensUntil(sceneFile, '\n'); //Model Name

                   for (sModelDrawInfo info : scene->modelInfos)
                   {
                       if (modelName == info.modelName)
                       {
                           object->mesh->modelFileName = info.meshPath;
                       }
                   }


                }
                if (token == "<Position->")   object->mesh->positionXYZ = LoadVector3Data(sceneFile);
                if (token == "<Rotation->")   object->mesh->rotationEulerXYZ = LoadVector3Data(sceneFile);
                if (token == "<Scale->")   object->mesh->uniformScale = LoadVector3Data(sceneFile).x;//For now only first float is scale
                if (token == "<Visibility->")   object->mesh->bIsVisible = LoadBoolData(sceneFile);
                if (token == "<Shading->")   object->mesh->bDoNotLight = LoadBoolData(sceneFile);
                if (token == "<Color->")
                {
                    object->mesh->bOverrideObjectColour = true;
                    glm::vec3 v = LoadVector3Data(sceneFile);
          
                    object->mesh->objectColourRGBA = glm::vec4(v, 1);


                }

             }       


            object->mesh->uniqueFriendlyName = object->name;
            scene->sceneObjects.push_back(object);

            std::cout << "Object " + object->name + " loaded." << std::endl;

        }

        //LOAD LIGHT
     // Read Scene File, including Lights
        if (token == "<Light-->")
        {
           cLightManager::sLight light;

            while (token != "<--Light>")
            {
                if (sceneFile.peek() == EOF)
                {
                    PrintSceneFileError(8, true);
                    return nullptr;
                }

                sceneFile >> token;

                if (token == "<Position->")    light.position = LoadVector4Data(sceneFile);
                if (token == "<Diffuse->")     light.diffuse = LoadVector4Data(sceneFile);
                if (token == "<Specular->")    light.specular = LoadVector4Data(sceneFile);
                if (token == "<Atten->")       light.atten = LoadVector4Data(sceneFile);
                if (token == "<Direction->")   light.direction = LoadVector4Data(sceneFile);
                if (token == "<Param1->")      light.param1 = LoadVector4Data(sceneFile);
                if (token == "<Param2->")      light.param2 = LoadVector4Data(sceneFile);
            }

            scene->lightManager->CreateNewLight(light);
            std::cout << "Light " << light.position.x <<" " << light.position.y<<" "<< light.position.z<< " loaded." << std::endl;
        }

     
        //LOAD OBJECT FILE

     

       

    }



//-----------------------------------------------------





    return scene;
}



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

