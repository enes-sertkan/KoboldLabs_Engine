#include "sObject.h"
#include "Scene.hpp"



void Object::Destroy()
{
    // 1. Delete all actions
    for (Action* action : actions)
    {
        action->OnDestroy();
        delete action;
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
    }

  
 //   delete this;
    // 5. If this object is dynamically allocated, 
    // you would call 'delete this' here - but only
    // if you're certain about ownership!
}