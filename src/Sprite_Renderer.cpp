#include "Sprite_Renderer.h"
#include <algorithm>

Sprite_Renderer::Sprite_Renderer(Shader& shader, float screenWidth, float screenHeight)
	:shader(shader), screenWidth(screenWidth), screenHeight(screenHeight), ViewMatrix(glm::mat4(1.0f))
{
	this->initRenderData();
}

Sprite_Renderer::~Sprite_Renderer()
{
	glDeleteVertexArrays(1, &quadVAO);
}

void Sprite_Renderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	//prepare transformations	
	shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0)); //re translate after rotation
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0, 0, 1.0));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0)); //rotation occur at 0,0 so we need to center origin
	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.SetMatrix4("model", model);
	shader.SetMatrix4("view", ViewMatrix);
	shader.SetVector3f("spriteColor", color);

	float totalLight = 0;
	for (glm::vec2 light : LightPositions)
	{
		float lightDistance = glm::length(glm::vec2(screenWidth, screenHeight)) / glm::length((light - position));
		totalLight += lightDistance;
	}
	shader.SetFloat("lightDistance", totalLight * 0.05);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(quadVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(staticVertices), staticVertices, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
void Sprite_Renderer::DrawFromSpriteSheet(Texture2D& texture, SubSprite& subsprite, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
{
	//prepare transformations	
	shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0)); //re translate after rotation
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0, 0, 1.0));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0)); //rotation occur at 0,0 so we need to center origin
	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.SetMatrix4("model", model);
	shader.SetMatrix4("view", ViewMatrix);
	shader.SetVector3f("spriteColor", color);

	float totalLight = 0;
	for (glm::vec2 light : LightPositions)
	{
		float lightDistance = glm::length(glm::vec2(screenWidth, screenHeight)) / glm::length((light - position));
		totalLight += lightDistance;
	}
	shader.SetFloat("lightDistance", totalLight * 0.05);

	//sprite dimensions
	float sheetHeight = subsprite.SheetDimensions.y;
	float sheetWidth = subsprite.SheetDimensions.x;
	float width = subsprite.SpriteDimensions.x, height = subsprite.SpriteDimensions.y;
	float x = subsprite.Position.x, y = subsprite.Position.y; 

	float vertices[] = {
		// pos		// tex
		0.0f, 1.0f, (x / sheetWidth), (y + height) / sheetHeight,
		1.0f, 0.0f, (x + width) / sheetWidth, y/ sheetHeight,
		0.0f, 0.0f, (x / sheetWidth), y / sheetHeight,

		0.0f, 1.0f, (x / sheetWidth), (y + height) / sheetHeight,
		1.0f, 1.0f, (x + width) / sheetWidth, (y + height) /sheetHeight,
		1.0f, 0.0f, (x + width) / sheetWidth, y / sheetHeight,
	};

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(quadVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Sprite_Renderer::SetLightPositions(std::vector<glm::vec2>& newLightPositions)
{
	LightPositions = newLightPositions;
}

void Sprite_Renderer::initRenderData()
{
	//configure VAO/VBO
	float vertices[] = {
		// pos		// tex     
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(quadVAO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(0);
}

