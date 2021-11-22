#pragma once
#include "GameObject.h"
class Platform : public GameObject
{
public:
	Platform();
	Platform(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	void HandleCollision(GameObject& object, float direction) override;
};

