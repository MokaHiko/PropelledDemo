#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "Shader.h"

struct SubSprite
{
	glm::vec2 SheetDimensions;
	glm::vec2 SpriteDimensions;
	glm::vec2 Position;

	SubSprite(glm::vec2 sheetDim, glm::vec2 spriteDim, glm::vec2 spritePosition)
		:SheetDimensions(sheetDim), SpriteDimensions(spriteDim), Position(spritePosition){}
};

struct Animation
{
	int currentFrame;
	std::vector<SubSprite> spriteMapFrames;
	Animation()
		:currentFrame(0) {}
};

class Sprite_Renderer
{
public:
	Sprite_Renderer(Shader& shader, float screenWidth, float screenHeight);
	~Sprite_Renderer();

	void DrawSprite(Texture2D& texture,
					glm::vec2 position,
					glm::vec2 size = glm::vec2(10.0f, 10.0f),
					float rotate = 0.0f,
					glm::vec3 color = glm::vec3(1.0f));

	void DrawFromSpriteSheet(Texture2D& texture, 
							 SubSprite& subsprite, glm::vec2 position,
							 glm::vec2 size, float rotate, 
							 glm::vec3 color);

	std::vector<glm::vec2> LightPositions;
	glm::mat4 ViewMatrix;

	void SetLightPositions(std::vector<glm::vec2>& newLightPositions);

	private:
	Shader shader; 

	unsigned int VBO;
	unsigned int quadVAO;

	float screenWidth, screenHeight;

	void initRenderData();
private:
	float staticVertices[6 * 4] = {
	// pos		// tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
	};
};
