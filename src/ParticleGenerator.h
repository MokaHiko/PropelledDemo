#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

struct Particle
{
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle()
		:Position(0.0), Velocity(0.0), Color(1.0f), Life(0.0f){}

};
class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0, 0.0f), bool generate = true);
	
	void Draw();
	void SetTexture(std::string textureName);
	glm::mat4 ViewMatrix;
private:
	//state
	std::vector<Particle> particles;
	unsigned int amount;

	//render state
	Shader shader;
	Texture2D texture;
	unsigned int VAO;

	void init();
	unsigned int firstUnusedParticle();

	void RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

