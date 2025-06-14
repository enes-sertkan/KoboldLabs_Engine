#ifndef _cBasicTextureManager_HG_
#define _cBasicTextureManager_HG_

#include "../GLCommon.h"

#include <string>
#include <map>
#include "CTextureFromBMP.h"

class cBasicTextureManager
{
public:
	bool Create2DTextureFromBMPFile(std::string textureFileName, bool bGenerateMIPMap = true);
	// We are also going to add this, too
	bool Create2DTextureFromPNGFile(std::string textureFileName, bool bGenerateMIPMap = true);

	// Picks a random texture from the textures loaded
	std::string PickRandomTexture(void);

	bool CreateCubeTextureFromBMPFiles(std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		bool bIsSeamless, std::string& errorString);


	// returns 0 on error
	GLuint getTextureIDFromName(std::string textureFileName);
	void ConnectTextureIDToName(GLuint textureID ,std::string textureName);

	void SetBasePath(std::string basepath);

private:
	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString(std::string nextErrorText);
	void m_appendErrorStringLine(std::string nextErrorTextLine);

	std::map< std::string, CTextureFromBMP* > m_map_TexNameToTexture;


};

#endif
