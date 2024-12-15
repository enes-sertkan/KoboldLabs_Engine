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

void DrawDebugCube(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program, Scene* scene);



// Helper function to compute the normal of a triangle
glm::vec3 ComputeTriangleNormal(const cPhysics::sTriangle& tri) {
    glm::vec3 edge1 = tri.vertices[1] - tri.vertices[0];
    glm::vec3 edge2 = tri.vertices[2] - tri.vertices[0];
    return glm::normalize(glm::cross(edge1, edge2));
}

// Helper function to check if all points of a triangle are on the same side of a plane
bool PointsOnSameSide(const cPhysics::sTriangle& tri, const glm::vec3& normal, float d) {
    bool firstSign = (glm::dot(normal, tri.vertices[0]) + d) > 0;
    for (int i = 1; i < 3; ++i) {
        if ((glm::dot(normal, tri.vertices[i]) + d > 0) != firstSign) {
            return false; // Points are on different sides
        }
    }
    return true;
}

// 1. Check if triangles overlap in the plane
bool TrianglesDoNotOverlapInPlane(const cPhysics::sTriangle& T1, const cPhysics::sTriangle& T2) {
    // Compute plane for T1
    glm::vec3 normal1 = ComputeTriangleNormal(T1);
    float d1 = -glm::dot(normal1, T1.vertices[0]);

    // Check if T2 is entirely on one side of T1's plane
    if (PointsOnSameSide(T2, normal1, d1)) return true;

    // Compute plane for T2
    glm::vec3 normal2 = ComputeTriangleNormal(T2);
    float d2 = -glm::dot(normal2, T2.vertices[0]);

    // Check if T1 is entirely on one side of T2's plane
    if (PointsOnSameSide(T1, normal2, d2)) return true;

    return false; // Planes overlap
}

// 2. Check if edges intersect
bool EdgeIntersect(const glm::vec3& A1, const glm::vec3& B1, const glm::vec3& A2, const glm::vec3& B2) {
    glm::vec3 dir1 = B1 - A1;
    glm::vec3 dir2 = B2 - A2;
    glm::vec3 r = A1 - A2;

    float a = glm::dot(dir1, dir1);
    float e = glm::dot(dir2, dir2);
    float f = glm::dot(dir2, r);

    float c = glm::dot(dir1, r);
    float b = glm::dot(dir1, dir2);

    float denom = a * e - b * b; // Determinant

    if (glm::abs(denom) < 1e-6f) {
        return false; // Parallel edges
    }

    float s = (b * f - c * e) / denom;
    float t = (a * f - b * c) / denom;

    // Check if the parameters are within bounds
    return (s >= 0.0f && s <= 1.0f && t >= 0.0f && t <= 1.0f);
}

bool EdgesIntersect(const cPhysics::sTriangle& T1, const cPhysics::sTriangle& T2) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (EdgeIntersect(
                T1.vertices[i], T1.vertices[(i + 1) % 3],
                T2.vertices[j], T2.vertices[(j + 1) % 3])) {
                return true;
            }
        }
    }
    return false;
}

// 3. Check if a vertex is inside a triangle using barycentric coordinates
bool VertexInsideTriangle(const glm::vec3& P, const cPhysics::sTriangle& tri) {
    glm::vec3 v0 = tri.vertices[1] - tri.vertices[0];
    glm::vec3 v1 = tri.vertices[2] - tri.vertices[0];
    glm::vec3 v2 = P - tri.vertices[0];

    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);

    float denom = d00 * d11 - d01 * d01;
    if (glm::abs(denom) < 1e-6f) return false; // Degenerate triangle

    float u = (d11 * d20 - d01 * d21) / denom;
    float v = (d00 * d21 - d01 * d20) / denom;

    // Check if the point is inside the triangle
    return (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f);
}

bool VertexInsideTriangle(const cPhysics::sTriangle& T1, const cPhysics::sTriangle& T2) {
    for (int i = 0; i < 3; ++i) {
        if (VertexInsideTriangle(T1.vertices[i], T2)) return true;
    }
    return false;
}

// Main function to check for intersection
bool TriangleIntersect(const cPhysics::sTriangle& T1, const cPhysics::sTriangle& T2) {
    // 1. Check plane overlap
    if (TrianglesDoNotOverlapInPlane(T1, T2)) return false;

    // 2. Check edge-edge intersections
    if (EdgesIntersect(T1, T2)) return true;

    // 3. Check if a vertex is inside the other triangle
    if (VertexInsideTriangle(T1, T2) || VertexInsideTriangle(T2, T1)) return true;

    return false;
}



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
    cPhysics* physMan = nullptr;
    unsigned int numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = 0;
    glm::vec3 prevpos;
    glm::vec3 deltaPos;
    PhysicsData physData;
    float AABBSIZE = 250.0f;


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



            //Make every vertex to world position. 
            for (unsigned int vertIndex = 0; vertIndex < 3; vertIndex++)
            {
                glm::vec4 transformedVertex = matModel * glm::vec4(curTri.vertices[vertIndex], 1.0f);

                if (transformedVertex.w != 0.0f) {
                    transformedVertex /= transformedVertex.w; // Normalize if w is valid
                    curTri.vertices[vertIndex] = glm::vec3(transformedVertex); // Convert back to vec3
                }
                else {
                    std::cout << "Wow, w is 0, so this vertex is 0,0,0" << std::endl;
                    // Handle the degenerate case (e.g., log an error, skip, or assign a fallback value)
                    curTri.vertices[vertIndex] = glm::vec3(0.0f, 0.0f, 0.0f); // Example fallback
                }

            }
            

            meshCollider.push_back(curTri);

        }

        prevpos = object->mesh->positionXYZ;

    }


    void UpdateMeshCollider()
    {
        deltaPos = object->mesh->positionXYZ - prevpos;
        prevpos = object->mesh->positionXYZ;
        std::cout << "DeltaPos: " << deltaPos.x << " " << deltaPos.y << " " << deltaPos.z << std::endl;

        for (cPhysics::sTriangle curTri : meshCollider)
        {
            for (glm::vec3 curVert : curTri.vertices)
            {
                curVert += deltaPos;
             
            }
        }
    }


    void Update() override
    {
        if (!physMan) return;

        
        physData.position = object->mesh->positionXYZ;
        
                // The size of the AABBs that we sliced up the Galactical model in the broad phase
           
                // Using the same XYZ location in space we used for the triangle vertices,
                //  we are going to pass the location of the viper to get an ID
                //  of an AABB/Cube that WOULD BE at that location (if there was one...)
                unsigned long long hypotheticalAABB_ID
                    =   physMan->calcBP_GridIndex(
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
                // NOT equal to the end
                if (it_pCube != physMan->map_BP_CubeGrid.end())
                {
                    // YES, there is an AABB (full of triangles) there!
                 //   pDebugAABB->objectColourRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
                    // 
                    // 


                    cPhysics::cBroad_Cube* pTheAABB_Cube = it_pCube->second;
                    float size = pTheAABB_Cube->getMaxXYZ().x - pTheAABB_Cube->getMinXYZ().x;
                    glm::vec3 center = glm::vec3(pTheAABB_Cube->getMinXYZ().x + size / 2, pTheAABB_Cube->getMinXYZ().y + size / 2, pTheAABB_Cube->getMinXYZ().z + size / 2);
                    DrawDebugCube(center, glm::vec4(1, 1, 1, 1), size, object->scene->programs[0], object->scene);
                    std::cout << pTheAABB_Cube->vec_pTriangles.size() << std::endl;

                    float closestDistance = 100000000000;
                    int ammount = pTheAABB_Cube->vec_pTriangles.size() / 1000;
                    int i = 0;
                    

                    
                    for(cPhysics::sTriangle tri: pTheAABB_Cube->vec_pTriangles)
                    {


                        for (cPhysics::sTriangle curTri : meshCollider)
                        {
                              if( TriangleIntersect(curTri, tri)) 
                         {
                           //  DrawDebugCube(tri.vertices[0], glm::vec4(1, 1, 1, 1), 0.1, object->scene->programs[0], object->scene);

                             std::cout << "PLANE COLLISION" << std::endl;
                         }
                        }

                   
                    

                        


                        i++;
                        if (i > ammount)
                        {
                            i = 0;
                            DrawDebugCube(tri.vertices[0], glm::vec4(1, 1, 1, 1), 0.1, object->scene->programs[0], object->scene);
                        }


                       
                    //Very stupid and simple way to check collisions. TODO: Change    
                     /*   float distance = glm::distance(tri.vertices[0], physData.position);
                        if (distance < closestDistance) closestDistance = distance;
                        if (distance<5)
                        {
                            object->mesh->positionXYZ = object->startTranform->position;
                        }
                    */
                    
                    }
                    std::cout << closestDistance << std::endl;

                    // Pass THIS smaller list of triangles to the narrow phase
                    numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing = pTheAABB_Cube->vec_pTriangles.size();
                }


                std::cout << "[BROAD] There are " << numberOfNarrowPhaseTrianglesInAABB_BroadPhaseThing << "triangles in current box." << std::endl;

            
             //  DrawMesh(pDebugAABB, program);
            //    pDebugAABB->bIsVisible = false;

            
                UpdateMeshCollider();

    }


};