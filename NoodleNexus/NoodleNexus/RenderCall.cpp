#include "GLCommon.h"
#include "sharedThings.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
#include "sMesh.h"
#include "cVAOManager/cVAOManager.h"
#include <iostream>
#include "cBasicTextureManager.h"


//RenderCall

sMesh* pDebugSphere = NULL;
sMesh* pDebugCube = NULL;


// Texture set up
void SetUpTextures(sMesh* pCurMesh, GLuint program, cBasicTextureManager* textureManager)
{
    GLuint MissingTexture_ID = textureManager->getTextureIDFromName("UV_Test_750x750.bmp");


    {
        GLuint textureID_00 = textureManager->getTextureIDFromName(pCurMesh->textures[0]);
        if (textureID_00 == 0)
        {
            textureID_00 = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, textureID_00);

        //glBindTextureUnit(0, textureID_00);
          // Set the texture wrapping mode for S and T coordinates

        GLenum selectedWrapMode;
        switch (pCurMesh->textureFillType[0])
        {
        case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        default:
            selectedWrapMode = GL_REPEAT;
            break;
        }


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping


        GLint texture00_UL = glGetUniformLocation(program, "texture00");
        glUniform1i(texture00_UL, 0);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
    }

    {
        GLuint textureID_01 = textureManager->getTextureIDFromName(pCurMesh->textures[1]);
        if (textureID_01 == 0)
        {
            textureID_01 = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, textureID_01);



        GLenum selectedWrapMode;
        switch (pCurMesh->textureFillType[1])
        {
        case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        default:
            selectedWrapMode = GL_REPEAT;
            break;
        }


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping

        GLint texture01_UL = glGetUniformLocation(program, "texture01");
        glUniform1i(texture01_UL, 1);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
    }

    {
        GLuint textureID_02 = textureManager->getTextureIDFromName(pCurMesh->textures[2]);
        if (textureID_02 == 0)
        {
            textureID_02 = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, textureID_02);


        GLenum selectedWrapMode;
        switch (pCurMesh->textureFillType[2])
        {
        case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        default:
            selectedWrapMode = GL_REPEAT;
            break;
        }


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping


        GLint texture02_UL = glGetUniformLocation(program, "texture02");
        glUniform1i(texture02_UL, 2);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
    }

    {
        GLuint textureID_03 = textureManager->getTextureIDFromName(pCurMesh->textures[3]);
        if (textureID_03 == 0)
        {
            textureID_03 = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 3);
        glBindTexture(GL_TEXTURE_2D, textureID_03);


        GLenum selectedWrapMode;
        switch (pCurMesh->textureFillType[3])
        {
        case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        default:
            selectedWrapMode = GL_REPEAT;
            break;
        }


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping


        GLint texture03_UL = glGetUniformLocation(program, "texture03");
        glUniform1i(texture03_UL, 3);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
    }



    {
        GLuint textureID_AO = textureManager->getTextureIDFromName(pCurMesh->AOtexture);
        if (textureID_AO == 0)
        {
            textureID_AO = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, textureID_AO);

    

        GLenum selectedWrapMode;
        selectedWrapMode = GL_REPEAT;
        //switch (pCurMesh->textureFillType[2])
        //{
        //case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        //case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        //case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        //default:
        //    selectedWrapMode = GL_REPEAT;
        //    break;
        //}


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping


        GLint textureAO_UL = glGetUniformLocation(program, "textureAO");
        glUniform1i(textureAO_UL, 4);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        GLint bUseAO = glGetUniformLocation(program, "useAO");
        if (pCurMesh->AOtexture!="")
        {
            //glUniform1f(bDoNotLight_UL, 1.0f);  // True
            glUniform1f(bUseAO, (GLfloat)GL_TRUE);  // True
        }
        else
        {
            //                glUniform1f(bDoNotLight_UL, 0.0f);  // False
            glUniform1f(bUseAO, (GLfloat)GL_FALSE);  // False
        }
    }


    {
        GLuint textureID_ST = textureManager->getTextureIDFromName(pCurMesh->STTexture);
        if (textureID_ST == 0)
        {
            textureID_ST = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, textureID_ST);

        GLint metalicUniform = glGetUniformLocation(program, "metallic");
        glUniform1f(metalicUniform, pCurMesh->metal);

        GLint smoothUniform = glGetUniformLocation(program, "smoothness");
        glUniform1f(smoothUniform, pCurMesh->smoothness);


        GLenum selectedWrapMode;
        selectedWrapMode = GL_REPEAT;
        //switch (pCurMesh->textureFillType[2])
        //{
        //case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        //case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        //case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        //default:
        //    selectedWrapMode = GL_REPEAT;
        //    break;
        //}


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping


        GLint textureST_UL = glGetUniformLocation(program, "textureST");
        glUniform1i(textureST_UL, 5);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        GLint bUseST = glGetUniformLocation(program, "useST");
        if (pCurMesh->STTexture != "")
        {
            //glUniform1f(bDoNotLight_UL, 1.0f);  // True
            glUniform1f(bUseST, (GLfloat)GL_TRUE);  // True
        }
        else
        {
            //                glUniform1f(bDoNotLight_UL, 0.0f);  // False
            glUniform1f(bUseST, (GLfloat)GL_FALSE);  // False
        }
    }


    {
        GLuint textureID_NM = textureManager->getTextureIDFromName(pCurMesh->NMTexture);
        if (textureID_NM == 0)
        {
            textureID_NM = MissingTexture_ID;

        }
        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_2D, textureID_NM);


        GLenum selectedWrapMode;
        selectedWrapMode = GL_REPEAT;
        //switch (pCurMesh->textureFillType[2])
        //{
        //case 1:selectedWrapMode = GL_CLAMP_TO_EDGE; break;
        //case 2:selectedWrapMode = GL_CLAMP_TO_BORDER; break;
        //case 3:selectedWrapMode = GL_MIRRORED_REPEAT; break;
        //default:
        //    selectedWrapMode = GL_REPEAT;
        //    break;
        //}


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, selectedWrapMode); // Horizontal wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, selectedWrapMode); // Vertical wrapping


        GLint textureNM_UL = glGetUniformLocation(program, "textureNM");
        glUniform1i(textureNM_UL, 6);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
        GLint bUseNM = glGetUniformLocation(program, "useNM");
        if (pCurMesh->NMTexture != "")
        {
            //glUniform1f(bDoNotLight_UL, 1.0f);  // True
            glUniform1f(bUseNM, (GLfloat)GL_TRUE);  // True
        }
        else
        {
            //                glUniform1f(bDoNotLight_UL, 0.0f);  // False
            glUniform1f(bUseNM, (GLfloat)GL_FALSE);  // False
        }
    }

    //TODO refator this shit mesh of a repeating code to load textures. I'm sure it can be waaaaaay smaller.


    // Now the ratios
    // uniform vec4 texRatio_0_to_3;
    GLint texRatio_0_to_3_UL = glGetUniformLocation(program, "texRatio_0_to_3");
    glUniform4f(texRatio_0_to_3_UL,
        pCurMesh->blendRatio[0],
        pCurMesh->blendRatio[1],
        pCurMesh->blendRatio[2],
        pCurMesh->blendRatio[3]);

    return;
}



void DrawCameraViewToTexture(Camera* camera, int framebufferID)
{



}





void DrawMesh(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Scene* scene)
{
    

    // Is it visible? 
    if (!pCurMesh->bIsVisible)
    {
        // Continue in loops jumps to the end of this loop
        // (for, while, do)
        return;
    }

    GLint bUseStencilTexture_UL = glGetUniformLocation(program, "bUseStencilTexture");


    if (pCurMesh->bIsStencilTexture)
    {



        glUniform1f(bUseStencilTexture_UL, (GLfloat)GL_TRUE);

        glActiveTexture(GL_TEXTURE0 + pCurMesh->stencilTextureID);

        GLuint stencilTextureID = textureManager->getTextureIDFromName(pCurMesh->stencilTexture);
        glBindTexture(GL_TEXTURE_2D, stencilTextureID);

        GLint stencilTexture_UL = glGetUniformLocation(program, "stencilTexture");
        glUniform1i(stencilTexture_UL, pCurMesh->stencilTextureID);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
    }
    else
    {
        glUniform1f(bUseStencilTexture_UL, (GLfloat)GL_FALSE);
    }

    float currentTime = glfwGetTime(); // For example, using GLFW
    GLint timeUniform = glGetUniformLocation(program, "time");
    glUniform1f(timeUniform, currentTime);





        pCurMesh->time += 0.1;
        GLint time_UL = glGetUniformLocation(program, "time");
        glUniform1f(time_UL, pCurMesh->time);

        GLint speedX_UL = glGetUniformLocation(program, "speedX");
        glUniform1f(speedX_UL, pCurMesh->textureSpeed.x);

        GLint speedY_UL = glGetUniformLocation(program, "speedY");
        glUniform1f(speedY_UL, pCurMesh->textureSpeed.y);

  

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


    //TODO: Add if
    // Set up the textures for THIS mesh
    // uniform bool bUseTextureAsColour;	// If true, then sample the texture
    GLint bUseTextureAsColour_UL = glGetUniformLocation(program, "bUseTextureAsColour");
    glUniform1f(bUseTextureAsColour_UL, (GLfloat)GL_TRUE);




    SetUpTextures(pCurMesh, program, textureManager);

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

    GLint cameraLocation = glGetUniformLocation(program, "cameraLocation");
    glUniform3f(cameraLocation,
        scene->fCamera->getEyeLocation().x+10.f,
        scene->fCamera->getEyeLocation().y,
        scene->fCamera->getEyeLocation().z);


    //transperency
   // if (pCurMesh->transperency < 1)
   // {
       // glDisable(GL_DEPTH_TEST);
        glUniform1f(glGetUniformLocation(program, "wholeObjectTransparencyAlpha"), pCurMesh->transperency);


        if (pCurMesh->uniqueFriendlyName == "trees" || pCurMesh->uniqueFriendlyName == "Clouds")
        glUniform1f(glGetUniformLocation(program, "suckPower"), 25.f);
        else
        glUniform1f(glGetUniformLocation(program, "suckPower"), 0.f);
    //}



        if (pCurMesh->uniqueFriendlyName == "trees" || pCurMesh->uniqueFriendlyName == "Clouds")
            glUniform1f(glGetUniformLocation(program, "shakePower"), 0.005f);
        else
            glUniform1f(glGetUniformLocation(program, "shakePower"), 0.f);


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

   // glEnable(GL_DEPTH_TEST);
	return;
}

glm::mat4 CalculateViewMatrixFromRotation(const glm::vec3& cameraRotation, const glm::vec3& cameraPosition)
{
    // Extract pitch, yaw, and roll from the rotation vector
    float pitch = glm::radians(cameraRotation.x);
    float yaw = glm::radians(cameraRotation.y);
    float roll = glm::radians(cameraRotation.z);


    // Calculate the forward vector from pitch and yaw
    glm::vec3 forward;
    forward.x = cos(yaw) * cos(pitch);
    forward.y = sin(pitch);
    forward.z = sin(yaw) * cos(pitch);
    forward = glm::normalize(forward);

    // Calculate the right vector
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Calculate the initial up vector
    glm::vec3 up = glm::normalize(glm::cross(right, forward));

    // Apply roll rotation to the up vector
    if (roll != 0.0f) {
        // Create a rotation matrix around the forward vector by the roll angle
        glm::mat4 rollRotation = glm::rotate(glm::mat4(1.0f), roll, forward);
        // Rotate the up vector using the roll rotation matrix
        up = glm::vec3(rollRotation * glm::vec4(up, 1.0f));
    }

    // Construct the view matrix
    glm::mat4 matView = glm::lookAt(cameraPosition, cameraPosition + forward, up);

    return matView;
}

void DrawShellTexturingWithCamera(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, Camera* camera)
{
    if (!pCurMesh->shellTexturing)
    {
        return;
    }

    // Get location of "bShellTexturing" uniform
    GLint bUseShellTexturing_UL = glGetUniformLocation(program, "bShellTexturing");
    glUniform1f(bUseShellTexturing_UL, (GLfloat)GL_TRUE);

    // Find model info in the VAO manager
    sModelDrawInfo meshToDrawInfo;
    if (!vaoManager->FindDrawInfoByModelName(pCurMesh->modelFileName, meshToDrawInfo))
    {
        return; // Model not found
    }

    // Setup the model transformation (Translation, Rotation, Scale)
    glm::mat4 matModel = glm::mat4(1.0f);

    // Translation matrix (position)
    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f), pCurMesh->positionXYZ);

    // Rotation matrices for X, Y, Z axis
    glm::mat4 matRotateX = glm::rotate(glm::mat4(1.0f), glm::radians(pCurMesh->rotationEulerXYZ.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 matRotateY = glm::rotate(glm::mat4(1.0f), glm::radians(pCurMesh->rotationEulerXYZ.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 matRotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(pCurMesh->rotationEulerXYZ.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Scale matrix
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f), glm::vec3(pCurMesh->uniformScale));

    // Combine all transformations into the model matrix
    matModel *= matTranslate;
    matModel *= matRotateX;
    matModel *= matRotateY;
    matModel *= matRotateZ;
    matModel *= matScale;

    // Pass the model matrix to the shader
    const GLint matModel_UL = glGetUniformLocation(program, "matModel");
    glUniformMatrix4fv(matModel_UL, 1, GL_FALSE, (const GLfloat*)&matModel);

    // Camera transformation: Get View and Projection matrices
    glm::mat4 matProjection = glm::perspective(glm::radians(camera->fov), camera->resolution.x / camera->resolution.y, 0.1f, 1000000.0f);
    glm::mat4 matView = CalculateViewMatrixFromRotation(camera->rotation, camera->position);

    // Pass view and projection matrices to the shader
    const GLint matView_UL = glGetUniformLocation(program, "matView");
    glUniformMatrix4fv(matView_UL, 1, GL_FALSE, (const GLfloat*)&matView);

    const GLint matProjection_UL = glGetUniformLocation(program, "matProjection");
    glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, (const GLfloat*)&matProjection);

    // Set up the camera location (for lighting, etc.)
    GLint cameraLocation_UL = glGetUniformLocation(program, "cameraLocation");
    glUniform3f(cameraLocation_UL, camera->position.x, camera->position.y, camera->position.z);



    GLint shellCount_UL = glGetUniformLocation(program, "shellCount");
    glUniform1i(shellCount_UL, pCurMesh->stData.shellCount);

    GLint verticalTightening_UL = glGetUniformLocation(program, "verticalTightening");
    glUniform1f(verticalTightening_UL, pCurMesh->stData.verticalTightening);

    GLint verticalExponent_UL = glGetUniformLocation(program, "verticalExponent");
    glUniform1f(verticalExponent_UL, pCurMesh->stData.verticalExponent);

    GLint shellLength_UL = glGetUniformLocation(program, "shellLength");
    glUniform1f(shellLength_UL, pCurMesh->stData.shellLength);

    // Set the shell texturing layer and draw the mesh for each layer
    for (int layer = 0; layer < pCurMesh->stData.shellCount; layer++)
    {
        GLint shellLayer_UL = glGetUniformLocation(program, "shellLayer");
        glUniform1i(shellLayer_UL, layer);

        // Bind the VAO and draw the mesh
        glBindVertexArray(meshToDrawInfo.VAO_ID);
        glDrawElements(GL_TRIANGLES, meshToDrawInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
    }

    // Disable VAO
    glBindVertexArray(0);
}


void DrawMeshWithCamera(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Camera* camera)
{
    if (glm::distance(camera->position, pCurMesh->positionXYZ) > camera->drawDistance)
    {
        return;
    }
    
  
  //  if (pCurMesh!=camera->scene->skybox->mesh)
 //   {
        // Check if the object is in front of the camera
        glm::vec3 cameraRotation = camera->rotation;

        float pitch = cameraRotation.x;
        float yaw = cameraRotation.y;
        // Roll is not used for calculating the forward vector

        glm::vec3 forward;
        forward.x = cos(pitch) * cos(yaw);
        forward.y = sin(pitch);
        forward.z = cos(pitch) * sin(yaw);

        forward = glm::normalize(forward);

        // Calculate the vector from the camera to the object
        glm::vec3 toObject = pCurMesh->positionXYZ - camera->position;

        // Calculate the dot product to determine if the object is in front of the camera
        float dotProduct = glm::dot(forward, toObject);


        // Determine your threshold: objects must be within maxAngle from the forward vector
        // For a 30� half-angle (60� total FOV), the cosine is about 0.866.
        float fovThreshold = glm::cos(glm::radians(0.1f));

        // Cull the object if it falls outside the desired cone
        if (dotProduct < fovThreshold) {
            //std::cout << "dot product: " << dotProduct << std::endl;
            //std::cout << "fov Threshold: " << fovThreshold << std::endl;
          //  return; // Object is outside the narrower visible area
    //    }

    }


    if (pCurMesh->drawBothFaces)
    {
        glDisable(GL_CULL_FACE);
    }

    // Is it visible? 
    if (!pCurMesh->bIsVisible)
    {
        // Continue in loops jumps to the end of this loop
        // (for, while, do)
        return;
    }

    GLint bUseStencilTexture_UL = glGetUniformLocation(program, "bUseStencilTexture");


    if (pCurMesh->bIsStencilTexture)
    {



        glUniform1f(bUseStencilTexture_UL, (GLfloat)GL_TRUE);

        glActiveTexture(GL_TEXTURE0 + pCurMesh->stencilTextureID);

        GLuint stencilTextureID = textureManager->getTextureIDFromName(pCurMesh->stencilTexture);
        glBindTexture(GL_TEXTURE_2D, stencilTextureID);

        GLint stencilTexture_UL = glGetUniformLocation(program, "stencilTexture");
        glUniform1i(stencilTexture_UL, pCurMesh->stencilTextureID);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here
    }
    else
    {
        glUniform1f(bUseStencilTexture_UL, (GLfloat)GL_FALSE);
    }

    float currentTime = glfwGetTime(); // For example, using GLFW
    GLint timeUniform = glGetUniformLocation(program, "time");
    glUniform1f(timeUniform, currentTime);





    pCurMesh->time += 0.1;
    GLint time_UL = glGetUniformLocation(program, "time");
    glUniform1f(time_UL, pCurMesh->time);

    GLint speedX_UL = glGetUniformLocation(program, "speedX");
    glUniform1f(speedX_UL, pCurMesh->textureSpeed.x);

    GLint speedY_UL = glGetUniformLocation(program, "speedY");
    glUniform1f(speedY_UL, pCurMesh->textureSpeed.y);

    GLint zoomPower_UL = glGetUniformLocation(program, "zoomPower");
    glUniform1f(zoomPower_UL, pCurMesh->zoomPower);

    GLint chromaticPower_UL = glGetUniformLocation(program, "chromaticPower");
    glUniform1f(chromaticPower_UL, pCurMesh->chromaticPower);

    GLint bNightMode = glGetUniformLocation(program, "bNightMode");
    if (camera->nightMode)
    {
        //glUniform1f(bDoNotLight_UL, 1.0f);  // True
        glUniform1f(bNightMode, (GLfloat)GL_TRUE);  // True
    }
    else
    {
        //                glUniform1f(bDoNotLight_UL, 0.0f);  // False
        glUniform1f(bNightMode, (GLfloat)GL_FALSE);  // False
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


    //TODO: Add if
    // Set up the textures for THIS mesh
    // uniform bool bUseTextureAsColour;	// If true, then sample the texture
    GLint bUseTextureAsColour_UL = glGetUniformLocation(program, "bUseTextureAsColour");
    glUniform1f(bUseTextureAsColour_UL, (GLfloat)GL_TRUE);




    SetUpTextures(pCurMesh, program, textureManager);

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
      
    
    
    GLint bUseShellTextutring = glGetUniformLocation(program, "bShellTexturing");

    if (pCurMesh->shellTexturing)
    {
        // bool doesn't really exist, it's a float...
        glUniform1f(bUseShellTextutring, (GLfloat)GL_TRUE);    // or 1.0f
    }
    else
    {
        glUniform1f(bUseShellTextutring, (GLfloat)GL_FALSE);   // or 0.0f
    }




    // Set the object colour
    // uniform vec4 objectColour;
    GLint objectColour_UL = glGetUniformLocation(program, "objectColour");
    glUniform4f(objectColour_UL,
        pCurMesh->objectColourRGBA.r,
        pCurMesh->objectColourRGBA.g,
        pCurMesh->objectColourRGBA.b,
        1.0f);

    GLint eyeLocation_UL = glGetUniformLocation(program, "eyeLocation");
    glUniform4f(eyeLocation_UL,
        camera->position.x,
        camera->position.y,
        camera->position.z,
        1.0f);


    ///CAMERA

    //        glm::mat4 m, p, v, mvp;
    glm::mat4 matProjection = glm::mat4(1.0f);

    matProjection = glm::perspective(glm::radians(camera->fov),           // FOV
        camera->resolution.x/camera->resolution.y,          // Aspect ratio of screen
        0.1f,           // Near plane
        1000000.0f);       // Far plane

   

    // Construct the view matrix
    glm::mat4 matView = CalculateViewMatrixFromRotation(camera->rotation, camera->position);



    const GLint matView_UL = glGetUniformLocation(program, "matView");
    glUniformMatrix4fv(matView_UL, 1, GL_FALSE, (const GLfloat*)&matView);

    const GLint matProjection_UL = glGetUniformLocation(program, "matProjection");
    glUniformMatrix4fv(matProjection_UL, 1, GL_FALSE, (const GLfloat*)&matProjection);




    GLint cameraLocation = glGetUniformLocation(program, "cameraLocation");
    glUniform3f(cameraLocation,
        camera->position.x,
        camera->position.y,
        camera->position.z);


    //transperency
   // if (pCurMesh->transperency < 1)
   // {
       // glDisable(GL_DEPTH_TEST);
    glUniform1f(glGetUniformLocation(program, "wholeObjectTransparencyAlpha"), pCurMesh->transperency);


    if (pCurMesh->uniqueFriendlyName == "trees" || pCurMesh->uniqueFriendlyName == "Clouds")
        glUniform1f(glGetUniformLocation(program, "suckPower"), 25.f);
    else
        glUniform1f(glGetUniformLocation(program, "suckPower"), 0.f);
    //}



    if (pCurMesh->uniqueFriendlyName == "trees" || pCurMesh->uniqueFriendlyName == "Clouds")
        glUniform1f(glGetUniformLocation(program, "shakePower"), 0.005f);
    else
        glUniform1f(glGetUniformLocation(program, "shakePower"), 0.f);


    for (int i = 0; i < 10; i++) {
        // Pack uv into x and y, active flag into z (as 1.0/0.0), and time into w.
        float activeValue = pCurMesh->waves[i].active ? 1.0f : 0.0f;
        glm::vec4 waveData(pCurMesh->waves[i].uv.x, pCurMesh ->waves[i].uv.y, activeValue, pCurMesh->waves[i].time);

        // Construct the uniform name, e.g., "waves[0].data"
        std::string uniformName = "waves[" + std::to_string(i) + "].data";
        GLint location = glGetUniformLocation(program, uniformName.c_str());
        if (location != -1) {
            glUniform4f(location, waveData.x, waveData.y, waveData.z, waveData.w);
        }
        if (activeValue) pCurMesh->waves[i].time+= 0.02;
        if (pCurMesh->waves[i].time > 4.f)
        {
            pCurMesh->waves[i].active = false;
            //pCurMesh->waves[i].time = 0.f;
            //// Generate a random UV position in the range [0, 1] for both u and v.
            //pCurMesh->waves[i].uv = glm::vec2(
            //    static_cast<float>(rand()) / static_cast<float>(RAND_MAX),
            //    static_cast<float>(rand()) / static_cast<float>(RAND_MAX)
            //);
        }
    }

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

    if (pCurMesh->drawBothFaces)
    {
        glEnable(GL_CULL_FACE);
    }

    // glEnable(GL_DEPTH_TEST);
    return;
}


void DrawSkyBox(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Camera* camera)
{


    // lookings
    // Sky box
//Move the sky sphere with the camera


       // Disable backface culling (so BOTH sides are drawn)
    glDisable(GL_CULL_FACE);
    // Don't perform depth buffer testing
    glDisable(GL_DEPTH_TEST);

    // Don't write to the depth buffer when drawing to colour (back) buffer
    // Not transperancy, just enables or disables
    /*glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);*/// or GL_LESS (default)
    // GL_DEPTH_TEST : do or not do the test against what's already on the depth buffer




    // Tell the shader this is the skybox, so use the cube map
    // uniform samplerCube skyBoxTexture;
    // uniform bool bIsSkyBoxObject;
    GLuint bIsSkyBoxObject_UL = glGetUniformLocation(program, "bIsSkyBoxObject");
    glUniform1f(bIsSkyBoxObject_UL, (GLfloat)GL_TRUE);

    //// Set the cube map texture, just like we do with the 2D
    GLuint cubeSamplerID = textureManager->getTextureIDFromName("Space");
    //        GLuint cubeSamplerID = ::g_pTextures->getTextureIDFromName("SunnyDay");
            // Make sure this is an unused texture unit
    glActiveTexture(GL_TEXTURE0 + 40);
    // *****************************************
    // NOTE: This is a CUBE_MAP, not a 2D
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeSamplerID);
    //glBindTexture(GL_TEXTURE_2D, cubeSamplerID);
            // *****************************************
    GLint skyBoxTextureSampler_UL = glGetUniformLocation(program, "skyBoxTextureSampler");
    glUniform1i(skyBoxTextureSampler_UL, 40);       // <-- Note we use the NUMBER, not the GL_TEXTURE3 here

    DrawMeshWithCamera(pCurMesh, program, vaoManager, textureManager, camera);

    //SkySphere->mesh->bIsVisible = true;

    glUniform1f(bIsSkyBoxObject_UL, (GLfloat)GL_FALSE);
    glEnable(GL_CULL_FACE);
    //// Enable depth test and write to depth buffer (normal rendering)
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    //glDepthFunc(GL_LESS);
    // **************************************************************


}


void DrawCameraView(Camera* camera, int programID)
{
    Scene* scene = camera->scene;
    scene->SortObjectsForDrawing();

    if (camera->scene->skybox != nullptr)
        DrawSkyBox(scene->skybox->mesh, scene->programs[0], scene->vaoManager, scene->textureManager, camera);

    for (Object* object : scene->sceneObjectsSorted)
    {

        sMesh* pCurMesh = object->mesh;

        if (!object->mesh->shellTexturing)
        DrawMeshWithCamera(pCurMesh, scene->programs[0], scene->vaoManager, scene->textureManager, camera);
        else
        DrawShellTexturingWithCamera(pCurMesh, scene->programs[0], scene->vaoManager, camera);
      //  DrawMesh(pCurMesh, scene->programs[programID], scene->vaoManager, scene->textureManager, scene);

    }

}

void DrawCameraViewToFramebufer(Camera* camera, int programID, int framebufferID)
{
    // Store previous framebuffer and viewport
    GLint prevFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &prevFBO);
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // Bind FBO and set viewport
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
    glViewport(0, 0, camera->resolution.x, camera->resolution.y);

    // Clear and render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawCameraView(camera, programID);

    // Restore previous state
    glBindFramebuffer(GL_FRAMEBUFFER, prevFBO);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
  
}

void DrawDebugSphere(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program, Scene* scene)
{
    // Created the debug sphere, yet?
    if (!pDebugSphere)           // Same as if ( pDebugSphere == NULL )
    {
        pDebugSphere = new sMesh();
        pDebugSphere->modelFileName = "assets/models/Sphere_radius_xyz_n_uv.ply";
        pDebugSphere->positionXYZ = glm::vec3(0.0f, 5.0f, 0.0f);
        pDebugSphere->bIsWireframe = true;
        pDebugSphere->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        pDebugSphere->uniqueFriendlyName = "Debug_Sphere";
        pDebugSphere->uniformScale = 10.0f;
        pDebugSphere->bDoNotLight = true;
    }
    // At this point the debug sphere is created

    pDebugSphere->bIsVisible = true;
    pDebugSphere->positionXYZ = position;
    pDebugSphere->bOverrideObjectColour = true;
    pDebugSphere->objectColourRGBA = RGBA;
    pDebugSphere->uniformScale = scale;

    DrawMesh(pDebugSphere, program, scene->vaoManager,scene->textureManager, scene);

    pDebugSphere->bIsVisible = false;

    return;
}

void DrawDebugCube(glm::vec3 position, glm::vec4 RGBA, float scale, GLuint program, Scene* scene)
{
    // Created the debug sphere, yet?
    if (!pDebugCube)           // Same as if ( pDebugSphere == NULL )
    {
        pDebugCube = new sMesh();
        pDebugCube->modelFileName = "assets/models/Cube_xyz_n_uv.ply";
        pDebugCube->positionXYZ = glm::vec3(0.0f, 5.0f, 0.0f);
        pDebugCube->bIsWireframe = true;
        pDebugCube->objectColourRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        pDebugCube->uniqueFriendlyName = "Debug_Cube";
        pDebugCube->uniformScale = 10.0f;
        pDebugCube->bDoNotLight = true;
    }
    // At this point the debug sphere is created

    pDebugCube->bIsVisible = true;
    pDebugCube->positionXYZ = position;
    pDebugCube->bOverrideObjectColour = true;
    pDebugCube->objectColourRGBA = RGBA;
    pDebugCube->uniformScale = scale;

    DrawMesh(pDebugCube, program, scene->vaoManager,scene->textureManager, scene);

    pDebugCube->bIsVisible = false;

    return;
}
