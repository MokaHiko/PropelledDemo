#include "ProjectileGenerator.h"

ProjectileGenerator::ProjectileGenerator() {}

ProjectileGenerator::ProjectileGenerator(Projectile& projectile)
	:currentProjectile(projectile) { }

void ProjectileGenerator::Update(float screenWidth, float screenHeight, float dt)
{
	bool allDestroyed = true;
	for (unsigned int i = 0; i < Projectiles.size(); i++)
	{
		bool outOfBoundsX = Projectiles[i].Position.x + Projectiles[i].ColliderOffset.x + Projectiles[i].ColliderSize.x >= screenWidth ||
							Projectiles[i].Position.x <= 0;
		bool outOfBoundsY = Projectiles[i].Position.y + Projectiles[i].ColliderOffset.y + Projectiles[i].ColliderSize.y >= screenHeight || false;

		if (outOfBoundsX || outOfBoundsY)
		{
			Projectiles[i].IsDestroyed = true;
		}
		else
		{
			Projectiles[i].ProcessRigidbody(screenWidth, screenHeight, dt);
		}
	
		if (!Projectiles[i].IsDestroyed)
		{
			allDestroyed = false;
		}
	}

	if (allDestroyed)
	{
		Projectiles.clear();
	}
}

void ProjectileGenerator::Draw(Sprite_Renderer& renderer, float dt)
{
	for (unsigned int i = 0; i < Projectiles.size(); i++)
	{
		if (!Projectiles[i].IsDestroyed)
		{
			Projectiles[i].Draw(renderer);
		}
		else
		{
			Projectiles[i].Animate("bulletDestroyed", renderer, dt);
		}
	}
}

void ProjectileGenerator::SetProjectile(Projectile& projectile)
{
	currentProjectile = projectile;
}

void ProjectileGenerator::Launch(float nProjectiles, glm::vec2 position, glm::vec2 direction)
{
	Projectile newProjectile = currentProjectile;

	newProjectile.Position = position;
	newProjectile.Launch(direction);
	int projectileIdx = firstUnusedProjectile();
	if (projectileIdx < 0)
	{
		Projectiles.push_back(newProjectile);
	}
	else
	{
		Projectiles[projectileIdx] = newProjectile;
	}
}

int ProjectileGenerator::firstUnusedProjectile()
{
	// linear search
	for (int i = 0; i < Projectiles.size(); i++)
	{
		if (Projectiles[i].IsDestroyed)
		{
			return i;
		}
	}
	return -1;
}

