#include "KLFileManager.hpp";
#include <fstream>;
#include <iostream>;





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

                if (token == "<Name->")  object->name =  SumTokensUntil(sceneFile, '\n'); //Object Name     //Not ssure if this will work
                if (token == "<Model->") object->mesh->modelFileName = SumTokensUntil(sceneFile, '\n'); //Model Name
                if (token == "<Position->")   object->mesh->positionXYZ = LoadVector3Data(sceneFile);
                if (token == "<Rotation->")   object->mesh->rotationEulerXYZ = LoadVector3Data(sceneFile);
                if (token == "<Scale->")   object->mesh->uniformScale = LoadVector3Data(sceneFile).x;//For now only first float is scale
                if (token == "<Visibility->")   object->mesh->bIsVisible = LoadBoolData(sceneFile);
             }       

            object->mesh->uniqueFriendlyName = object->name;
            scene->sceneObjects.push_back(object);

        }

        //LOAD LIGHT
        if (token == "<Light-->")
        {

        }

     
        //LOAD OBJECT FILE

     

       

    }



//-----------------------------------------------------





    return scene;
}

