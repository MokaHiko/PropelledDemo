#pragma once
#include <glm/glm.hpp>
#include <map>

#include "Sprite_Renderer.h"
#include "ResourceManager.h"

class GameObject
{
public:
	std::string Tag;
	glm::vec2	Position, Size;
	glm::vec3   Color;
	float		Rotation;
	bool		IsDestroyed;
	
	float		Mass;
	float		Gravity; 
	bool		isGravity;
	bool	    isRigidBody;
	glm::vec2   Velocity, Acceleration, ColliderOffset, ColliderSize;

	Texture2D	Sprite;
	std::string	currentAnimation;
	float	    runningTime;

	GameObject();
	GameObject(glm::vec2 position, 
			   glm::vec2 size,
			   Texture2D sprite,
			   glm::vec3 color = glm::vec3(1.0f),
			   glm::vec2 velocity = glm::vec2(0.0f),
			   float mass = 1,
			   float gravity = 9.8f);

	void Draw(Sprite_Renderer& renderer);

	void DrawFromSpriteSheet(Sprite_Renderer& renderer, SubSprite& subSprite);

	void AddAnimation(std::string animationName, Animation animation);

	virtual void Animate(std::string animationName, Sprite_Renderer& renderer, float dt);

	void ProcessRigidbody(float screenHeight, float screenWidth , float dt);

	void SetCurrentAnimation(std::string animationName);

	void AddForceByImpulse(glm::vec2 impulse);

	void SetColliderDimensions(glm::vec2 colliderOffset, glm::vec2 colliderSize);

	glm::vec2 GetColliderDimensions();

	virtual void HandleCollision(GameObject& object, float direction = 0.0f) { }

	void DrawCollider(Sprite_Renderer& renderer);
private:
	//Physics Functions
	void ProcessGravity(float floor);
};

