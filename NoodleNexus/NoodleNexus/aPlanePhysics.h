#pragma once

#include "Action.h";
#include "sObject.h"

#include <iostream>
#include "Scene.hpp"
#include "cPhysics.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/intersect.hpp> // Optional for intersection utilities
#include "aPlaneMovement.hpp"

void DrawDebugCube(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program, Scene* scene);


struct PhysicsData
{
    glm::vec3 position;
    glm::vec3 acceleration = glm::vec3(0,0,0);
    glm::vec3 velocity = glm::vec3(0,0,0);
    float mass = 1000.0f;
    bool isStatic = false;
};

class aPlanePhysics : public Action
{
public:
 
    std::vector<cPhysics::sTriangle> meshCollider;
    std::vector<cPhysics::sTriangle> meshColliderWorld;
    aPlaneMovement* planeMovement;
    cPhysics* physMan = nullptr;
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;
    glm::vec3 prevpos;
    glm::vec3 deltaPos;
    PhysicsData physData;
    float AABBSIZE = 200.0f;


    void Start() override
    {
        physData.position = object->mesh->positionXYZ;



        //LOOP THROUGH THIS MODEL TRIANGKES
        std::vector<cVAOManager::sTriangle> triInfo;
        object->scene->vaoManager->getTriangleMeshInfo(object->mesh->modelFileName, triInfo);



        //LOAD MESH COLLIDER
        for (std::vector<cVAOManager::sTriangle>::iterator itVAOTri = triInfo.begin();
            itVAOTri != triInfo.end(); itVAOTri++)
        {
            cVAOManager::sTriangle curVAOTri = *itVAOTri;

            // This triangle is in MODEL space, not world space
            cPhysics::sTriangle curTri;
            curTri.vertices[0] = curVAOTri.vertices[0];
            curTri.vertices[1] = curVAOTri.vertices[1];
            curTri.vertices[2] = curVAOTri.vertices[2];
            curTri.normal = curVAOTri.normal;
            curTri.calculateSideLengths();

            // transform this into world space...
            // (just like we do to draw it)


            // Translation (movement, position, placement...)
            glm::mat4 matTranslate
                = glm::translate(glm::mat4(1.0f), object->mesh->positionXYZ);

            // Rotation...
            // Caculate 3 Euler acix matrices...
            glm::mat4 matRotateX =
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(object->mesh->rotationEulerXYZ.x), // Angle in radians
                    glm::vec3(1.0f, 0.0, 0.0f));

            glm::mat4 matRotateY =
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(object->mesh->rotationEulerXYZ.y), // Angle in radians
                    glm::vec3(0.0f, 1.0, 0.0f));

            glm::mat4 matRotateZ =
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(object->mesh->rotationEulerXYZ.z), // Angle in radians
                    glm::vec3(0.0f, 0.0, 1.0f));


            // Scale

            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(object->mesh->uniformScale, object->mesh->uniformScale, object->mesh->uniformScale));

            // Calculate the final model/world matrix
            glm::mat4 matModel = glm::mat4(1.0f);
            matModel *= matTranslate;     // matModel = matModel * matTranslate;
            matModel *= matRotateX;
            matModel *= matRotateY;
            matModel *= matRotateZ;
            matModel *= matScale;



            

            meshCollider.push_back(curTri);

        }

        prevpos = object->mesh->positionXYZ;

    }


    void UpdateMeshCollider()
    {


        meshColliderWorld.clear();

        for (cPhysics::sTriangle curTri : meshCollider)
        {


            // Translation (movement, position, placement...)
            glm::mat4 matTranslate
                = glm::translate(glm::mat4(1.0f), object->mesh->positionXYZ);

            // Rotation...
            // Caculate 3 Euler acix matrices...
            glm::mat4 matRotateX =
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(object->mesh->rotationEulerXYZ.x), // Angle in radians
                    glm::vec3(1.0f, 0.0, 0.0f));

            glm::mat4 matRotateY =
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(object->mesh->rotationEulerXYZ.y), // Angle in radians
                    glm::vec3(0.0f, 1.0, 0.0f));

            glm::mat4 matRotateZ =
                glm::rotate(glm::mat4(1.0f),
                    glm::radians(object->mesh->rotationEulerXYZ.z), // Angle in radians
                    glm::vec3(0.0f, 0.0, 1.0f));


            // Scale

            glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                glm::vec3(object->mesh->uniformScale, object->mesh->uniformScale, object->mesh->uniformScale));

            // Calculate the final model/world matrix
            glm::mat4 matModel = glm::mat4(1.0f);
            matModel *= matTranslate;     // matModel = matModel * matTranslate;
            matModel *= matRotateX;
            matModel *= matRotateY;
            matModel *= matRotateZ;
            matModel *= matScale;


            cPhysics::sTriangle triangle;
            //Make every vertex to world position. 
            for (unsigned int vertIndex = 0; vertIndex < 3; vertIndex++)
            {
                glm::vec4 transformedVertex = matModel * glm::vec4(curTri.vertices[vertIndex], 1.0f);

                if (transformedVertex.w != 0.0f) {
                    transformedVertex /= transformedVertex.w; // Normalize if w is valid

                    triangle.vertices[vertIndex] = glm::vec3(transformedVertex);
                }
                else {
                    std::cout << "Wow, w is 0, so this vertex is 0,0,0" << std::endl;
                    // Handle the degenerate case (e.g., log an error, skip, or assign a fallback value)
                }

            }
            meshColliderWorld.push_back(triangle);
           
          
            

        }
    }

    void Update() override
    {
        if (!physMan) return;

          UpdateMeshCollider();
        physData.position = object->mesh->positionXYZ;

        // The size of the AABBs that we sliced up the Galactical model in the broad phase

        // Using the same XYZ location in space we used for the triangle vertices,
        //  we are going to pass the location of the viper to get an ID
        //  of an AABB/Cube that WOULD BE at that location (if there was one...)
        unsigned long long hypotheticalAABB_ID
            = physMan->calcBP_GridIndex(
                physData.position.x,
                physData.position.y,
                physData.position.z, AABBSIZE);

        // Where would that hypothetical AABB be in space
        glm::vec3 minXYZofHypotheticalCube = physMan->calcBP_MinXYZ_FromID(hypotheticalAABB_ID, AABBSIZE);

        //// Draw a cube at that location
        //sMesh* pDebugAABB = pFindMeshByFriendlyName("AABB_MinXYZ_At_Origin");
        //pDebugAABB->positionXYZ = minXYZofHypotheticalCube;
        //pDebugAABB->bIsVisible = true;
        //pDebugAABB->uniformScale = 1'000.0f;

        // Is this an AABB that's already part of the broad phase? 
        // i.e. is it already in the map?
        std::map< unsigned long long, cPhysics::cBroad_Cube* >::iterator
            it_pCube = physMan->map_BP_CubeGrid.find(hypotheticalAABB_ID);
        //
        if (it_pCube == physMan->map_BP_CubeGrid.end())
        {
            // NO, there is no cube there
            //pDebugAABB->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
            numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;
        }

        float closestDistance = 100000000000;
        // NOT equal to the end
        if (it_pCube != physMan->map_BP_CubeGrid.end())
        {
            // YES, there is an AABB (full of triangles) there!
         //   pDebugAABB->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
            // 
            // 


            cPhysics::cBroad_Cube* pTheAABB_Cube = it_pCube->second;
            planeMovement->pTheAABB_Cube = pTheAABB_Cube;
            float size = pTheAABB_Cube->getMaxXYZ().x - pTheAABB_Cube->getMinXYZ().x;
            glm::vec3 center = glm::vec3(pTheAABB_Cube->getMinXYZ().x + size / 2, pTheAABB_Cube->getMinXYZ().y + size / 2, pTheAABB_Cube->getMinXYZ().z + size / 2);
            DrawDebugCube(center, glm::vec4(1, 1, 1, 1), size, object->scene->programs[0], object->scene);
            std::cout << pTheAABB_Cube->vec_pTriangles.size() << std::endl;

        
            int ammount = pTheAABB_Cube->vec_pTriangles.size() / 1000;
            int i = 0;


            for (cPhysics::sTriangle tri : meshColliderWorld)
            {
               
             // DrawDebugCube(tri.vertices[0], glm::vec4(1, 1, 1, 1), 1, object->scene->programs[0], object->scene);

                std::vector<cPhysics::sCollision_RayTriangleInMesh> vec_RayTriangle_Collisions;

                if (i < 2)
                {
                    if (physMan->rayCastCustom(tri.vertices[i], tri.vertices[i+1], pTheAABB_Cube->vec_pTriangles, vec_RayTriangle_Collisions, false))
                    {
                        std::cout << "PLANE COLLISION1" << std::endl;
                        planeMovement->HitSmt(tri.vertices[0]);
                    }

                }else
                    if (physMan->rayCastCustom(tri.vertices[i], tri.vertices[i-1], pTheAABB_Cube->vec_pTriangles, vec_RayTriangle_Collisions, false))
                    {
                        std::cout << "PLANE COLLISION1" << std::endl;
                        planeMovement->HitSmt(tri.vertices[0]);
                    }

                i++;
                if (i > 2) i = 0;

            }     

            std::cout << meshColliderWorld[0].vertices->x << std::endl;

            std::cout << closestDistance << std::endl;

            // Pass THIS smaller list of triangles to the narrow phase
            numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = pTheAABB_Cube->vec_pTriangles.size();


            std::cout << "[BROAD] There are " << numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing << "triangles in current box." << std::endl;


            //Very stupid and simple way to check collisions. TODO: Change    
             /*   float distance = glm::distance(tri.vertices[0], physData.position);
                if (distance < closestDistance) closestDistance = distance;
                if (distance<5)
                {
                    object->mesh->positionXYZ = object->startTranform->position;
                }
            */

        }

    
    
       

      

        //  DrawMesh(pDebugAABB, program);
       //    pDebugAABB->bIsVisible = false;


      



    }
};