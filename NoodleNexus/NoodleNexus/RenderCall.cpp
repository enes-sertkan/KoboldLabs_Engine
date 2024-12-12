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

void DrawSkyBox(sMesh* pCurMesh, GLuint program, cVAOManager* vaoManager, cBasicTextureManager* textureManager, Scene* scene)
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

    DrawMesh(pCurMesh, program, vaoManager, textureManager, scene);

    //SkySphere->mesh->bIsVisible = true;

    glUniform1f(bIsSkyBoxObject_UL, (GLfloat)GL_FALSE);
    glEnable(GL_CULL_FACE);
    //// Enable depth test and write to depth buffer (normal rendering)
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    //glDepthFunc(GL_LESS);
    // **************************************************************


}
