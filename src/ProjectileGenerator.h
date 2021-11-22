#pragma once

#include <vector>
#include "Projectile.h"
class ProjectileGenerator
{
public:
	Projectile currentProjectile;
	std::vector<Projectile> Projectiles;

	ProjectileGenerator();

	ProjectileGenerator(Projectile& projectile);

	void Update(float screenWidth, float screenHeight, float dt);
	void Draw(Sprite_Renderer& renderer, float dt);

	void SetProjectile(Projectile& projectile);

	void Launch(float nProjectiles, glm::vec2 position, glm::vec2 direction);

	int firstUnusedProjectile();
};
