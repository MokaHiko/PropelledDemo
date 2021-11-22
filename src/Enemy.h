#pragma once
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();
	Enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite);

	float health;
	float damage;
	
	void HandleCollision(GameObject& object, float direction = 0.0f) override;

	// ui and animations 
	void DrawHealthBar(Sprite_Renderer& renderer);
	void Animate(std::string animationName, Sprite_Renderer& renderer, float dt) override;

private:
	void TakeDamage(float damage, glm::vec2 direction);

	//std::vector<drop pool>;
};

