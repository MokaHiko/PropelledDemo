#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
	:shader(shader), texture(texture), amount(amount), ViewMatrix(glm::mat4(1.0f))
{
	init();
}

void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset, bool generate)
{
	// add new particles
	if (generate)
	{
		for (unsigned int i = 0; i < newParticles; i++)
		{
			int unusedParticle= firstUnusedParticle();
			RespawnParticle(particles[unusedParticle], object, offset);
		}
	}

	// update all particles
	for (unsigned int i = 0; i < amount; i++)
	{
		Particle& p = particles[i];
		p.Life -= dt;
		if (p.Life > 0.0f)
		{
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.5f;
		}
	}
}

void ParticleGenerator::Draw()
{
	// use additive blending to give a glow
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	shader.Use();
	
	for (Particle& particle : particles)
	{
		shader.SetVector2f("offset", particle.Position);
		shader.SetVector4f("color", particle.Color);
		shader.SetMatrix4("view", ViewMatrix);

		texture.Bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	// reset the binding mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	unsigned int VBO;
	float particle_quad[] = {
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < amount; i++)
	{
		particles.push_back(Particle());
	}

	//set particleSize
	shader.Use();
	shader.SetFloat("scale", 20);
}

unsigned int lastUsedParticle = 0;
unsigned ParticleGenerator::firstUnusedParticle()
{
	// search from last used particle
	for (unsigned int i = lastUsedParticle; i < amount; i++)
	{
		if (particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	// linear search
	for (unsigned int i = 0; i < lastUsedParticle; i++)
	{
		if (particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	lastUsedParticle = 0;
	return 0; 
}

void ParticleGenerator::RespawnParticle(Particle& particle, GameObject& object, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}

void ParticleGenerator::SetTexture(std::string textureName)
{
	texture = ResourceManager::GetTexture(textureName);
}
