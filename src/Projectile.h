#pragma once
#include "GameObject.h"
class Projectile : public GameObject 
{
public:
	Projectile();
	Projectile(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	void HandleCollision(GameObject& object, float direction) override;
	void Launch(glm::vec2 direction);
};

