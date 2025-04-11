#include "sObject.h"
#include "Scene.hpp"
#include "SceneEditor.h"


void Object::Destroy()
{
    // 1. Delete all actions
    for (Action* action : actions)
    {
        action->OnDestroy();
        RemoveAction(action);
       // delete action;
    }
    actions.clear();

    // 2. Clean up owned resources
    delete startTranform;
    startTranform = nullptr;

 

    // 3. Destroy child objects if we own them
    // (Only if sceneObjects contains owned pointers)
    // for (Object* child : sceneObjects) {
    //     child->Destroy();
    // }
    // sceneObjects.clear();

    // 4. Remove from scene
    for (Object* child : m_children)
        child->Destroy();


    if (scene)
    {
        scene->RemoveObject(this);

        if (scene->sceneEditor)
            if (scene->sceneEditor->selectedObject == this)
                scene->sceneEditor->selectedObject = nullptr;
    }

  
 //   delete this;
    // 5. If this object is dynamically allocated, 
    // you would call 'delete this' here - but only
    // if you're certain about ownership!
}


Object* Object::Clone()
{
    // Step 1: Use the Scene function to initialize the clone's basic structure
    Object* clone = scene->GenerateMeshObjectsFromObject(
        this->mesh->modelFileName,
        this->mesh->positionXYZ,
        this->mesh->uniformScale,
        this->mesh->rotationEulerXYZ,
        this->mesh->bOverrideObjectColour,
        this->mesh->objectColourRGBA,
        !this->mesh->bDoNotLight,
        scene->sceneObjects
    );

    // Step 2: Copy metadata and Object-specific settings
    clone->name = this->name;
    clone->fakeParent = this->fakeParent;
    clone->isTemporary = this->isTemporary;
    clone->isCollisionStatic = this->isCollisionStatic;
    clone->tags = this->tags;
    clone->isActive = this->isActive;
    clone->actions = this->actions; // Optional: Deep copy if needed

    // Step 3: Copy Transform
    *(clone->startTranform) = *(this->startTranform);

    // Step 4: Deep copy mesh fields
    sMesh* src = this->mesh;
    sMesh* dst = clone->mesh;

    dst->modelFileName = src->modelFileName;
    dst->uniqueFriendlyName = src->uniqueFriendlyName;
    dst->positionXYZ = src->positionXYZ;
    dst->rotationEulerXYZ = src->rotationEulerXYZ;
    dst->uniformScale = src->uniformScale;

    dst->objectColourRGBA = src->objectColourRGBA;
    dst->bOverrideObjectColour = src->bOverrideObjectColour;

    dst->bIsWireframe = src->bIsWireframe;
    dst->bIsVisible = src->bIsVisible;
    dst->bDoNotLight = src->bDoNotLight;
    dst->bIsStencilTexture = src->bIsStencilTexture;

    dst->textureSpeed = src->textureSpeed;
    dst->time = src->time;

    dst->stencilTextureID = src->stencilTextureID;
    dst->stencilTexture = src->stencilTexture;

    dst->transperency = src->transperency;

    for (unsigned int i = 0; i < sMesh::MAX_NUM_TEXTURES; ++i)
    {
        dst->textures[i] = src->textures[i];
        dst->blendRatio[i] = src->blendRatio[i];
        dst->textureFillType[i] = src->textureFillType[i];
    }

    dst->AOtexture = src->AOtexture;
    dst->STTexture = src->STTexture;
    dst->NMTexture = src->NMTexture;

    dst->metal = src->metal;
    dst->smoothness = src->smoothness;
    dst->useMetalTexture = src->useMetalTexture;
    dst->useSmoothTexture = src->useSmoothTexture;

    dst->shellTexturing = src->shellTexturing;
    dst->stData = src->stData;

    for (int i = 0; i < 20; ++i)
    {
        dst->stColliders[i] = src->stColliders[i];
    }

    dst->zoomPower = src->zoomPower;
    dst->chromaticPower = src->chromaticPower;
    dst->drawBothFaces = src->drawBothFaces;

    dst->isParticleEmitter = src->isParticleEmitter;
    *(dst->pParticles) = *(src->pParticles); // Copy particle data

    dst->particleSSBO = src->particleSSBO;
    dst->particleUBO = src->particleUBO;

    for (int i = 0; i < 10; ++i)
    {
        dst->waves[i] = src->waves[i];
    }

    // Step 5: Parent handling
    if (m_parent)
    {
        m_parent->AddChildRaw(clone);
    }

    return clone;
}
