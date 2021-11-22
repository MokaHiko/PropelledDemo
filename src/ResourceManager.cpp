#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <tinyxml/tinyxml2.h>

#include <stb_image/stb_image.h>



// instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader>	 ResourceManager::Shaders;
std::map<std::string, Animation> ResourceManager::Animations;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

Animation ResourceManager::GetAnimationFromSpriteXML(const char* file, glm::vec2 sheetDim, std::string animationName, glm::vec2 range, float interpolationFrames)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(file);
    int x, y, width, height;

    tinyxml2::XMLElement* subSpritesXML = doc.FirstChildElement("TextureAtlas"); 

    Animation animation;
    unsigned int subSpriteIdx= 0;
    for (tinyxml2::XMLElement* subSpriteXML= subSpritesXML->FirstChildElement();  subSpriteXML != NULL; subSpriteXML = subSpriteXML->NextSiblingElement())
    {
        if (subSpriteIdx >= range.x &&  subSpriteIdx <= range.y)
        {
			subSpriteXML->QueryAttribute("x", &x);
			subSpriteXML->QueryAttribute("y", &y);
			subSpriteXML->QueryAttribute("width", &width);
			subSpriteXML->QueryAttribute("height", &height);

            for (int i = -1 ; i < interpolationFrames; i++)
            {
			    animation.spriteMapFrames.push_back(SubSprite(sheetDim, glm::vec2(width, height), glm::vec2(x, y)));
            }
        }
        subSpriteIdx++;
    }
    Animations[animationName] = animation;

    return Animations[animationName];
}
Animation ResourceManager::GetAnimationFromSpriteSheet(const char* file, glm::vec2 sheetDim, std::string animationName,glm::vec2 spriteDim, glm::vec2 range, float interpolationFrames)
{
    int width = sheetDim.x / spriteDim.x; 
    int height = sheetDim.y / spriteDim.y;

    Animation animation;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (i + j >= range.x && i + j <= range.y)
            {
				animation.spriteMapFrames.push_back(SubSprite(sheetDim, 
													glm::vec2(spriteDim.x, spriteDim.y), 
													glm::vec2(i * spriteDim.x, j * spriteDim.y)));
            }
        }
    }
    Animations[animationName] = animation;
    return Animations[animationName];
}



void ResourceManager::Clear()
{
	//properly delete all shaders
	for (auto iter : Shaders)
	{
		glDeleteProgram(iter.second.ID);
	}

	//properly delete all textures
	for (auto iter : Textures)
	{
		glDeleteTextures(1, &iter.second.ID);
	}
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	Texture2D texture;
	//create Texture object
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	//load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    
	//now generate texture
	texture.Generate(width, height, data);

	stbi_image_free(data);
	return texture;
}
