#pragma once

#include "GameObject.h"
#include "PhysicsEngine2D.h"
#include "ParticleGenerator.h"
#include "Projectile.h"
#include "ProjectileGenerator.h"
#include "Camera2D.h"

class Pilot : public GameObject
{
public:
	float JumpForce;
	float MovementSpeed;

	float Fuel;
	bool  isGrounded;
	bool  Propelled;

	glm::vec2 MousePos;
	ParticleGenerator* PlayerParticles;
	ProjectileGenerator* PlayerProjectiles;
	
	Pilot();
	Pilot(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float movementSpeed, float jumpForce);

	void PlayerController(bool* Keys, float screenWidth, float screenHeight, float dt, glm::vec2 mousePos, bool* mouseButtons);

	void HandleCollision(GameObject& object, float direction) override;

	void ShootProjectile(float screenWidth, float screenHeight);

	void SetProjectile(Projectile& projectile);
};


