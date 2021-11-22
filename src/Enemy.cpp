#include "Enemy.h"
#include <iostream>
#include "PhysicsEngine2D.h"

Enemy::Enemy()
	:GameObject(), health(100), damage(20)
{
	Tag = "enemy";
}

Enemy::Enemy(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
	:GameObject(pos, size, sprite, glm::vec3(1.0f)), health(100), damage(20)
{
	Tag = "enemy";
}

void Enemy::TakeDamage(float damage, glm::vec2 direction)
{
	health -= damage;
	if (health < 0)
	{
		IsDestroyed = true;

	}
}

void Enemy::HandleCollision(GameObject& object, float direction)
{
	if (object.Tag == "player")
	{
		TakeDamage(12, glm::vec2(0.0f,0.0f));
	}
	else if (object.Tag == "projectile")
	{
		TakeDamage(12, glm::vec2(0.0f,0.0f));
	}
	else if (object.Tag == "platform")
	{

	}
}

void Enemy::DrawHealthBar(Sprite_Renderer& renderer)
{
	renderer.DrawSprite(ResourceManager::GetTexture("healthbar"), glm::vec2(Position.x + Size.x * 0.1f, Position.y - Size.y * 0.2), glm::vec2(Size.x * 0.75 * 0.01 * health, Size.y * 0.4));
}

void Enemy::Animate(std::string animationName, Sprite_Renderer& renderer, float dt)
{
	//for spritesheetAnimations
	runningTime += dt; 
	float fps = 12;

	Animation animation = ResourceManager::Animations[animationName];
	SubSprite currentFrame = animation.spriteMapFrames[animation.currentFrame];

	DrawFromSpriteSheet(renderer, currentFrame);
	DrawHealthBar(renderer);

	if (animation.currentFrame + 1 >= (int)ResourceManager::Animations[animationName].spriteMapFrames.size()) //Render Loop
	{
		ResourceManager::Animations[animationName] .currentFrame = 0;
	}
	else if(runningTime* fps  >= 1.0f)
	{
		ResourceManager::Animations[animationName].currentFrame += 1;
		runningTime = 0;
	}
}
