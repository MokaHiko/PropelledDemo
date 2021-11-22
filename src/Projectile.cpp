#include "Projectile.h"

Projectile::Projectile()
	:GameObject()
{
	Tag = "projectile";
	Mass = 1;
	isGravity = false;
}

Projectile::Projectile(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
	:GameObject(pos, size, sprite, glm::vec3(1.0f))
{
	Tag = "projectile";
	Mass = 1;
	isGravity = false;
}

void Projectile::HandleCollision(GameObject& object, float direction)
{
	IsDestroyed = true;
}

void Projectile::Launch(glm::vec2 direction)
{
	Velocity = direction * 10.0f;
}

