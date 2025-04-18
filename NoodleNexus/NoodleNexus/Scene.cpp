#include "Scene.hpp"

int lua_SetObjectPos(lua_State* L);


void Scene::DrawMesh(sMesh* pCurMesh, GLuint program)
{
    //std::cout << "Drawing " << pCurMesh->uniqueFriendlyName << ". Position: " << pCurMesh->positionXYZ.x << " " << pCurMesh->positionXYZ.y << " " << pCurMesh->positionXYZ.z << " " << std::endl;

    // Is it visible? 
    if (!pCurMesh->bIsVisible)
    {
        // Continue in loops jumps to the end of this loop
        // (for, while, do)
        return;
    }

    // Use lighting or not
    // uniform bool bDoNotLight;	
    GLint bDoNotLight_UL = glGetUniformLocation(program, "bDoNotLight");
    if (pCurMesh->bDoNotLight)
    {
        //glUniform1f(bDoNotLight_UL, 1.0f);  // True
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_TRUE);  // True
    }
    else
    {
        //                glUniform1f(bDoNotLight_UL, 0.0f);  // False
        glUniform1f(bDoNotLight_UL, (GLfloat)GL_FALSE);  // False
    }


    // Could be called the "model" or "world" matrix
    glm::mat4 matModel = glm::mat4(1.0f);

    // Translation (movement, position, placement...)
    glm::mat4 matTranslate
        = glm::translate(glm::mat4(1.0f),
            glm::vec3(pCurMesh->positionXYZ.x,
                pCurMesh->positionXYZ.y,
                pCurMesh->positionXYZ.z));

    // Rotation...
    // Caculate 3 Euler acix matrices...
    glm::mat4 matRotateX =
        glm::rotate(glm::mat4(1.0f),
            glm::radians(pCurMesh->rotationEulerXYZ.x), // Angle in radians
            glm::vec3(1.0f, 0.0, 0.0f));

    glm::mat4 matRotateY =
        glm::rotate(glm::mat4(1.0f),
            glm::radians(pCurMesh->rotationEulerXYZ.y), // Angle in radians
            glm::vec3(0.0f, 1.0, 0.0f));

    glm::mat4 matRotateZ =
        glm::rotate(glm::mat4(1.0f),
            glm::radians(pCurMesh->rotationEulerXYZ.z), // Angle in radians
            glm::vec3(0.0f, 0.0, 1.0f));


    // Scale
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        glm::vec3(pCurMesh->uniformScale,
            pCurMesh->uniformScale,
            pCurMesh->uniformScale));


    // Calculate the final model/world matrix
    matModel *= matTranslate;     // matModel = matModel * matTranslate;
    matModel *= matRotateX;
    matModel *= matRotateY;
    matModel *= matRotateZ;
    matModel *= matScale;

    //           matRoationOnly = matModel * matRotateX * matRotateY * matRotateZ;


               //mat4x4_mul(mvp, p, m);
               //mvp = p * v * m;
   //            glm::mat4 matMVP = matProjection * matView * matModel;

               //const GLint mvp_location = glGetUniformLocation(program, "MVP");
               //glUniformMatrix4fv(mvp_location, 
               //    1,
               //    GL_FALSE,
               //    (const GLfloat*)&matMVP);

    const GLint mvp_location = glGetUniformLocation(program, "matModel");
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&matModel);

    //const GLint mvp_location = glGetUniformLocation(program, "matView");
    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&matMVP);

    //const GLint mvp_location = glGetUniformLocation(program, "matProjection");
    //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&matMVP);

    // uniform bool bUseObjectColour;
    GLint bUseObjectColour = glGetUniformLocation(program, "bUseObjectColour");

    if (pCurMesh->bOverrideObjectColour)
    {
        // bool doesn't really exist, it's a float...
        glUniform1f(bUseObjectColour, (GLfloat)GL_TRUE);    // or 1.0f
    }
    else
    {
        glUniform1f(bUseObjectColour, (GLfloat)GL_FALSE);   // or 0.0f
    }

    // Set the object colour
    // uniform vec4 objectColour;
    GLint objectColour_UL = glGetUniformLocation(program, "objectColour");
    glUniform4f(objectColour_UL,
        pCurMesh->objectColourRGBA.r,
        pCurMesh->objectColourRGBA.g,
        pCurMesh->objectColourRGBA.b,
        1.0f);


    // solid or wireframe, etc.
//        glPointSize(10.0f);
    if (pCurMesh->bIsWireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //        glDrawArrays(GL_TRIANGLES, 0, 3);
//            glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_TO_DRAW);

    sModelDrawInfo meshToDrawInfo;
    if (vaoManager->FindDrawInfoByModelName(pCurMesh->modelFileName, meshToDrawInfo))
    {
        // Found the model
        glBindVertexArray(meshToDrawInfo.VAO_ID); 		// enable VAO(and everything else)
        //https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDrawElements.xhtml
        glDrawElements(GL_TRIANGLES,
            meshToDrawInfo.numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0);

        glBindVertexArray(0); 			//disable VAO(and everything else)
    }


    return;
}



Camera* Scene::AddCamera(glm::vec3 position, glm::vec3 rotation, glm::vec2 res)
{
    Camera* newCamera = new Camera(position, rotation, res);
    newCamera->scene = this;
    cameras.push_back(newCamera);
    return newCamera;
}



void Scene::DrawSphere(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program)
{
    // Created the debug sphere, yet?
    if (!pSphere)           // Same as if ( pDebugSphere == NULL )
    {
        pSphere = new sMesh();
        pSphere->modelFileName = "assets/models/Sphere_radius_xyz_n_uv.ply";
        pSphere->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
        pSphere->bIsWireframe = false;
        pSphere->objectColourRGBA = RGBA;
        pSphere->uniqueFriendlyName = "Sphere";
        pSphere->uniformScale = scale;
        pSphere->bDoNotLight = true;
    }
    // At this point the debug sphere is created

    pSphere->bIsVisible = true;
    pSphere->positionXYZ = position;
    pSphere->bOverrideObjectColour = true;
    pSphere->objectColourRGBA = RGBA;
    pSphere->uniformScale = scale;

    DrawMesh(pSphere, program);

    pSphere->bIsVisible = false;

    return;
}



void Scene::DrawRay(glm::vec3 pos, glm::vec3 posEnd, GLuint program, glm::vec4 color, bool isWire, float scale)
{
    float distance = glm::distance(pos, posEnd);
    glm::vec3 direction = glm::normalize(posEnd - pos);
    glm::vec3 movingPoint = pos;
    while (glm::distance(pos, movingPoint) < distance)
    {
        // Move the next ball 0.1 times the normalized camera direction
        movingPoint += (direction * 0.10f);
         DrawSphere(movingPoint, color, scale, program);
    }

}



sModelDrawInfo LoadDrawInfo(cVAOManager* meshManager, sModelDrawInfo drawInfo, GLuint program)
{
    meshManager->LoadModelIntoVAO(drawInfo.meshPath,
        drawInfo, program);

    std::cout <<"Draw Info for '"<< drawInfo.meshPath << "' - Loaded" << std::endl << drawInfo.numberOfVertices << " vertices loaded" << std::endl;

    return drawInfo;
}


sMesh* CreateMeshObjects(std::vector<sMesh*>& meshes, sMesh* mesh)
{

    meshes.push_back(mesh);

    return mesh;

}



void Scene::Prepare(cVAOManager* meshManager, GLuint program, PhysicsManager* physMan, GLFWwindow* newWindow, cBasicFlyCamera* newFlyCamera)
{

    physicsManager = physMan;

    physicsManager->VAOMan = meshManager;

    window = newWindow;

    newFlyCamera->scene = this;
    fCamera = newFlyCamera;

    vaoManager = meshManager;


    for (sModelDrawInfo info : modelInfos)
    {
        LoadDrawInfo(meshManager, info, program);
    }


    for (Object* object : sceneObjects)
    {
       
        object->scene = this;


        if (object->isCollisionStatic)
            physicsManager->AddTriangleMesh(object->mesh->modelFileName, object->startTranform->position, object->startTranform->rotation, object->startTranform->scale.x);

    }

    //called only once
    this->lightManager->loadUniformLocations(program);

    textureManager = new cBasicTextureManager();
    
    SetCurrentCameraToID();
    programs.push_back(program);

  //  animator = new Animator();
   // animator->scene = this;
}
// Comparator function
bool Scene::CompareObjectsBasedOnDistanecToCamera(Object* a, Object* b) {

    float distanceA = glm::distance(a->mesh->positionXYZ, fCamera->getEyeLocation());
    float distanceB = glm::distance(b->mesh->positionXYZ, fCamera->getEyeLocation());

    return distanceA < distanceB;
}

void Scene::SortObjectsForDrawing()
{
     sceneObjectsSorted = sceneObjects;
    std::sort(sceneObjectsSorted.begin(), sceneObjectsSorted.end(), [this](Object* a, Object* b) {
        if(a->mesh->transperency<1 && b->mesh->transperency>=1)
        {
            return false;
        } 
        
        if(a->mesh->transperency>=1 && b->mesh->transperency<1)
        {
            return true;
        }
        float distanceA = glm::distance(a->mesh->positionXYZ, fCamera->getEyeLocation());
        float distanceB = glm::distance(b->mesh->positionXYZ, fCamera->getEyeLocation());

        return distanceA > distanceB;
        });//I tried using function to sort, but it didn't work, so I used "lambda" approach.



    
}

void Scene::UpdateDeltaTime()
{
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> dt = currentTime - previousTime;
    previousTime = currentTime;
    deltaTime = dt.count();
    if (deltaTime > 0.5) deltaTime = 0.5f;
    
}


void Scene::Start()
{
    for (Object* obj : sceneObjects)
    {
        if (obj)
        {
            for (Action* action : obj->actions)
            {
                action->Start();
            }
        }
    }

  /*  if (!luaScript.LoadScript("cObjectMovement.lua")) {
        std::cerr << "Failed to load Lua script." << std::endl;
    }*/


    //animator = new Animator();
    
    
  
    
    // Continue with the rest of your logic

    //lua_register(L, "MoveObject", ObjectManager::BindToLua);
    



    
}




void Scene::Update()
{
  
    if (!isFlyCamera)
    {
     //   MoveCameraToPoint();
     //   fCamera->FreezePlayerRotation();
    }
    else
    {
        fCamera->UnfreezePlayerRotation();
    }
    for (Object* obj : sceneObjects)
    {
        if (obj)
            if (obj->actions.size() >0)
                for (Action* action : obj->actions)
                    if (action && action->isActive) action->Update();

    }

    

  
    UpdateDeltaTime();

}

void Scene::AddActionToObj(Action* action, Object* object)
{
    action->object = object;
    object->actions.push_back(action);
    actions.push_back(action);
}

void Scene::MoveCameraToPoint()
{
   if (cameraPoints.size() == 0) return;

   glm::vec3 newPos = moveTowards(g_pFlyCamera->getEyeLocation(), cameraPoints[currentCameraIndex].position, cameraMoveSpeed * deltaTime);
   glm::vec2 newRot = moveTowards2(g_pFlyCamera->getEyeRotation(), cameraPoints[currentCameraIndex].rotation, cameraRotateSpeed * deltaTime);
   
   
   // Print both the old and new rotation values
   

   std::cout << "Target Rotation: (" << cameraPoints[currentCameraIndex].rotation.x << ", " << cameraPoints[currentCameraIndex].rotation.y << ")" << std::endl;
   std::cout << "New Rotation: (" << newRot.x << ", " << newRot.y << ")" << std::endl;

   g_pFlyCamera->setEyeLocation(newPos);
   g_pFlyCamera->setEyeRotation(newRot.x, newRot.y);
}

void Scene::SetCameraToNextPoint()
{
    if (cameraPoints.size() == 0) return;

    if (currentCameraIndex >= cameraPoints.size() - 1) currentCameraIndex = -1;
    currentCameraIndex++;

    g_pFlyCamera->setEyeLocation(cameraPoints[currentCameraIndex].position);
    g_pFlyCamera->setEyeRotation(cameraPoints[currentCameraIndex].rotation.x, cameraPoints[currentCameraIndex].rotation.y);
}

void Scene::SetCameraToFirstPoint()
{
    if (cameraPoints.size() == 0) return;
    g_pFlyCamera->setEyeLocation(cameraPoints[1].position);
    g_pFlyCamera->setEyeRotation(cameraPoints[1].rotation.x, cameraPoints[1].rotation.y);
    SetCurrentCameraToID();
}

void Scene::SetCameraToPlayer()
{
    if (cameraPoints.size() == 0) return;
    fCamera->getEyeLocation() = sceneObjects[15]->mesh->positionXYZ;
    SetCurrentCameraToID();
}

void Scene::NextCameraPoint()
{
      if (cameraPoints.size() == 0) return;

    if (currentCameraIndex >= cameraPoints.size() - 1) currentCameraIndex = -1;
    currentCameraIndex++;
    SetCurrentCameraToID();
}

void Scene::SetCurrentCameraToID()
{
    currentCameraPoint = cameraPoints[currentCameraIndex];
}

void Scene::SetCurrentCamera(CameraPoint camPoint)
{
    currentCameraPoint = camPoint;
}

void Scene::RemoveObject(Object* obj)
{
    std::vector<Object*>::iterator it = std::find(sceneObjects.begin(), sceneObjects.end(), obj);
    if (it != sceneObjects.end())
    {
        delete* it; // Free the memory if dynamically allocated
        sceneObjects.erase(it); // Remove from vector
    }
}


Object* Scene::GenerateMeshObjectsFromObject(
    const std::string& filePath, glm::vec3 posXYZ, float scale, glm::vec3 rotXYZ,
    bool bOverrideColor, glm::vec4 objectColor, bool bDoLightingExist,
    std::vector<Object*>& sceneObjects
) {
    Object* object = new Object;
    object->mesh = new sMesh();
 
    // Initialize mesh properties
    object->mesh->uniqueFriendlyName = filePath;
    object->mesh->modelFileName = filePath;
    object->mesh->positionXYZ = posXYZ;
    object->mesh->rotationEulerXYZ = rotXYZ;
    object->mesh->bOverrideObjectColour = bOverrideColor;
    object->mesh->objectColourRGBA = objectColor;

    object->startTranform->position = posXYZ;
    object->startTranform->rotation = rotXYZ;
    object->startTranform->scale.x = scale;


    // Set uniform scale based on the passed-in scale parameter
    object->mesh->uniformScale = scale; 

    // Set lighting based on the parameter
    object->mesh->bDoNotLight = !bDoLightingExist;
    object->scene = this;
    // Add the object to the scene
    sceneObjects.push_back(object);

    return object;
}


void Scene::ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
