#include "KLFileManager.hpp";
#include <fstream>;
#include <iostream>;
#include "sharedThings.h"
#include "Transform.h"




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

glm::vec2 LoadVector2Data(std::ifstream& file)
{
    std::string token = "";
    glm::vec2 finalVector;


    file >> finalVector.x;
    file >> finalVector.y;

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

std::vector<std::string> LoadTags(std::ifstream& file) {
    std::vector<std::string> tags;
    std::string tag;

    // Read tags until the end of the line
    while (file.peek() != '\n') {
        {
            file >> tag;
                if (tag != "<StaticCollision->" || "false" || "true") {
                    tags.push_back(tag);
                }
        }
    }

    return tags;
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
        std::string path = token;
        scene->modelPaths.push_back(path);
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


    while (token != "<--Scene>" && token != "</SceneFile>" && token != "</KoboldLabs>")
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
            Object* object = new Object();
            object->mesh = new sMesh();
            int textureID = 0;
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
                if (token == "<Tags->") { object->tags = LoadTags(sceneFile); }
                if (token == "<StaticCollision->")   object->isCollisionStatic = LoadBoolData(sceneFile);
                if (token == "<Position->")   object->startTranform->position = LoadVector3Data(sceneFile);
                if (token == "<Rotation->")    object->startTranform->rotation = LoadVector3Data(sceneFile);
                if (token == "<Scale->")    object->startTranform->scale.x = LoadVector3Data(sceneFile).x;//For now only first float is scale
                if (token == "<Visibility->")   object->mesh->bIsVisible = LoadBoolData(sceneFile);
                if (token == "<Shading->")   object->mesh->bDoNotLight = !LoadBoolData(sceneFile);
                if (token == "<Texture->") {
                    sceneFile >> object->mesh->textures[textureID]; textureID++; //TODO: Put this into function or smt
                }
                if (token == "<Color->") 
                {
                    object->mesh->bOverrideObjectColour = true;
                    glm::vec3 v = LoadVector3Data(sceneFile);
          
                    object->mesh->objectColourRGBA = glm::vec4(v, 1);
                }
             }       


            object->mesh->uniqueFriendlyName = object->name;
            scene->sceneObjects.push_back(object);

            //TODO: I think we should originally give value to start transform and THEN give it to mesh. SO shoueld fix it later.
           
            object->mesh->positionXYZ = object->startTranform->position;
            object->mesh->rotationEulerXYZ = object->startTranform->rotation;
            object->mesh->uniformScale  = object->startTranform->scale.x; //TODO: if you need proper scale - fix this.

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

        if (token == "<Camera-->")
        //LOAD OBJECT FILE
        {

            CameraPoint cameraPoint;
            while (token != "<--Camera>")
            {
                sceneFile >> token;

                if (token == "<Position->")   cameraPoint.position = LoadVector3Data(sceneFile);
                if (token == "<Rotation->")  cameraPoint.rotation = LoadVector2Data(sceneFile);
            }

            scene->cameraPoints.push_back(cameraPoint);

        }
    }



//-----------------------------------------------------





    return scene;
}


void KLFileManager::WriteSceneFile(const Scene* scene, std::string fileName) {
    std::ofstream myfile(fileName, std::ios::trunc);

    if (myfile.is_open()) {
        myfile << "<KoboldLabs>\n";
        myfile << "<SceneFile>\n";
        myfile << "<ObjectLoad-->\n";

        // Write model files
        for (std::string path : scene->modelPaths)
        {
            myfile << path << "\n";
        }
        myfile << "<--ObjectLoad>\n";

        myfile << "<Scene-->\n\n";

        // Write objects
        for (size_t j = 0; j < scene->sceneObjects.size(); ++j) {
            Object* object = scene->sceneObjects[j];

            // Skip temporary objects like the select box
            if (object->isTemporary) {
                continue;
            }

            myfile << "<Object-->\n";
            myfile << "<Name-> " << object->name << "\n";
            myfile << "<Model-> " << object->mesh->uniqueFriendlyName << "\n";
            myfile << "<Tags->";//No space bc space is in for loop
            for (const std::string& tag : object->tags) {
                myfile << " " << tag ;
            }
            myfile << "\n";
            myfile << "<StaticCollision-> " << (object->isCollisionStatic? "true" : "false") << "\n";
            myfile << "<Position-> " << object->startTranform->position.x << " "
                << object->startTranform->position.y << " "
                << object->startTranform->position.z << "\n";
            myfile << "<Rotation-> " << object->startTranform->rotation.x << " "
                << object->startTranform->rotation.y << " "
                << object->startTranform->rotation.z << "\n";
            myfile << "<Scale-> " << 
                object->startTranform->scale.x << " " << 
                object->startTranform->scale.y << " " <<
                object->startTranform->scale.z << "\n";
            myfile << "<Visibility-> " << (object->mesh->bIsVisible ? "true" : "false") << "\n";
            myfile << "<Shading-> " << (!object->mesh->bDoNotLight ? "true" : "false") << "\n";
            if (object->mesh->bOverrideObjectColour) {
                myfile << "<Color-> "
                    << object->mesh->objectColourRGBA.r << " "
                    << object->mesh->objectColourRGBA.g << " "
                    << object->mesh->objectColourRGBA.b << "\n";
            }
            myfile << "<--Object>\n\n";
        }

        // Write lights
        for (int j = 0; j <= scene->lightManager->lastLightIndex; ++j) {
            const cLightManager::sLight& light = scene->lightManager->theLights[j];
            myfile << "<Light-->\n";

            // Write Position
            myfile << "<Position-> " << light.position.x << " "
                << light.position.y << " "
                << light.position.z << " "
                << light.position.w << "\n";

            // Write Diffuse
            myfile << "<Diffuse-> " << light.diffuse.r << " "
                << light.diffuse.g << " "
                << light.diffuse.b << " "
                << light.diffuse.a << "\n";

            // Write Specular
            myfile << "<Specular-> " << light.specular.r << " "
                << light.specular.g << " "
                << light.specular.b << " "
                << light.specular.a << "\n";

            // Write Attenuation
            myfile << "<Atten-> " << light.atten.x << " "
                << light.atten.y << " "
                << light.atten.z << " "
                << light.atten.w << "\n";

            // Write Direction
            myfile << "<Direction-> " << light.direction.x << " "
                << light.direction.y << " "
                << light.direction.z << " "
                << light.direction.w << "\n";

            // Write Param1 and Param2
            myfile << "<Param1-> " << light.param1.x << " "
                << light.param1.y << " "
                << light.param1.z << " "
                << light.param1.w << "\n";
            myfile << "<Param2-> " << light.param2.x << " "
                << light.param2.y << " "
                << light.param2.z << " "
                << light.param2.w << "\n";

            myfile << "<--Light>\n\n";
        }

        // Write camera data
        for (size_t i = 0; i < scene->cameraPoints.size(); ++i) {
            CameraPoint cameraPoint = scene->cameraPoints[i];
            myfile << "<Camera-->\n";
            myfile << "<Position-> " << cameraPoint.position.x << " "
                << cameraPoint.position.y << " "
                << cameraPoint.position.z << "\n";
            myfile << "<Rotation-> " << cameraPoint.rotation.x << " "
                << cameraPoint.rotation.y << "\n";
            myfile << "<--Camera>\n\n";
        }

        myfile << "<--Scene>\n";
        myfile << "</SceneFile>\n";
        myfile << "</KoboldLabs>\n";

        myfile.close();
    }
    else {
        std::cout << "Unable to open file" << std::endl;
    }
}


// For Imgui

std::string KLFileManager::SerializeSceneToString(const Scene* scene)
{
    std::stringstream ss;

    // Write header
    ss << "<KoboldLabs>\n";
    ss << "<SceneFile>\n";
    ss << "<ObjectLoad-->\n";

    // Write model files
    for (const std::string& path : scene->modelPaths) {
        ss << path << "\n";
    }
    ss << "<--ObjectLoad>\n";
    ss << "<Scene-->\n\n";

    // Write objects
    for (const Object* object : scene->sceneObjects) {
        if (object->isTemporary) continue;

        ss << "<Object-->\n";
        ss << "<Name-> " << object->name << "\n";
        ss << "<Model-> " << object->mesh->uniqueFriendlyName << "\n";
        ss << "<Tags->";
        for (const std::string& tag : object->tags) {
            ss << " " << tag;
        }
        ss << "\n";
        ss << "<StaticCollision-> " << (object->isCollisionStatic ? "true" : "false") << "\n";
        ss << "<Position-> " << object->startTranform->position.x << " "
            << object->startTranform->position.y << " "
            << object->startTranform->position.z << "\n";
        ss << "<Rotation-> " << object->startTranform->rotation.x << " "
            << object->startTranform->rotation.y << " "
            << object->startTranform->rotation.z << "\n";
        ss << "<Scale-> " << object->startTranform->scale.x << " "
            << object->startTranform->scale.y << " "
            << object->startTranform->scale.z << "\n";
        ss << "<Visibility-> " << (object->mesh->bIsVisible ? "true" : "false") << "\n";
        ss << "<Shading-> " << (!object->mesh->bDoNotLight ? "true" : "false") << "\n";
        if (object->mesh->bOverrideObjectColour) {
            ss << "<Color-> " << object->mesh->objectColourRGBA.r << " "
                << object->mesh->objectColourRGBA.g << " "
                << object->mesh->objectColourRGBA.b << "\n";
        }
        ss << "<--Object>\n\n";
    }

    // Write lights
    for (int j = 0; j <= scene->lightManager->lastLightIndex; ++j) {
        const cLightManager::sLight& light = scene->lightManager->theLights[j];
        ss << "<Light-->\n";
        ss << "<Position-> " << light.position.x << " "
            << light.position.y << " "
            << light.position.z << " "
            << light.position.w << "\n";
        ss << "<Diffuse-> " << light.diffuse.r << " "
            << light.diffuse.g << " "
            << light.diffuse.b << " "
            << light.diffuse.a << "\n";
        // ... continue with all light properties ...
        ss << "<--Light>\n\n";
    }

    // Write camera data
    for (const CameraPoint& cameraPoint : scene->cameraPoints) {
        ss << "<Camera-->\n";
        ss << "<Position-> " << cameraPoint.position.x << " "
            << cameraPoint.position.y << " "
            << cameraPoint.position.z << "\n";
        ss << "<Rotation-> " << cameraPoint.rotation.x << " "
            << cameraPoint.rotation.y << "\n";
        ss << "<--Camera>\n\n";
    }

    ss << "<--Scene>\n";
    ss << "</SceneFile>\n";
    ss << "</KoboldLabs>\n";

    return ss.str();
}

bool KLFileManager::CompareSceneToSavedData(const Scene* scene, const std::string& savedData)
{
    std::string currentData = SerializeSceneToString(scene);
    return currentData == savedData;
}