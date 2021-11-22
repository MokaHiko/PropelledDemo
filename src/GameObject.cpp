#include "GameObject.h"
#include <cmath>
#include <iostream>

GameObject::GameObject()
	:Position(0.0f,0.0f),isGravity(true), isRigidBody(true),Size(1.0f, 1.0f), ColliderOffset(0.0f,0.0f), ColliderSize(1.0f,1.0f), Velocity(0.0f), Color(1.0f,1.0f,1.0f), Rotation(0.0f), Gravity(9.8), Sprite(), IsDestroyed(false), Acceleration(0), Mass(1), runningTime(0){ }

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, float mass, float gravity)
	:Position(position),isGravity(true), isRigidBody(true), Size(size), ColliderOffset(0.0f,0.0f), ColliderSize(size), Velocity(velocity), Color(color), Rotation(0.0f), Gravity(gravity), Sprite(sprite), IsDestroyed(false), Acceleration(0), Mass(mass), runningTime(0) { }

void GameObject::Draw(Sprite_Renderer& renderer)
{
	renderer.DrawSprite(Sprite, Position, Size, Rotation, Color);
}

void GameObject::DrawFromSpriteSheet(Sprite_Renderer& renderer, SubSprite& subSprite)
{
    renderer.DrawFromSpriteSheet(Sprite, subSprite, Position, Size, Rotation, Color);
}

void GameObject::ProcessGravity(float floor)
{
	const int drag = 0.5;
	if (Position.y + Size.y != floor)
	{
		Velocity.y -= (Gravity * 0.2 * Mass);
	}
}

void GameObject::ProcessRigidbody(float screenWidth, float screenHeight, float dt)
{
	if (isGravity)
	{
		ProcessGravity(screenHeight);
	}
	
	if (isRigidBody)
	{
		// add velocity, decerate to 0
		Velocity.x = Velocity.x + Acceleration.x * dt;
		Velocity.y = Velocity.y + Acceleration.y * dt;
		if (Velocity.x < 0 && Acceleration.x < 0)
		{
			Acceleration.x = 0;
			Velocity.x = 0;
		}
		else if (Velocity.x > 0 && Acceleration.x > 0)
		{
			Acceleration.x = 0;
			Velocity.x = 0;
		}
		// retain object in screen
		if ((Position.x <= screenWidth - Size.x) && (Position.x >= 0.0f))
		{
			Position.x += Velocity.x; 
		}
		if (Position.x > screenWidth - Size.x)
		{
			Position.x = screenWidth - Size.x;
		}
		else if (Position.x < 0)
		{
			Position.x = 0;
		}

		// position is added to the negative of velocity as screen coordinates top left is (0,0)
		Position.y -= Velocity.y;
		if (Position.y + Size.y > screenHeight)
		{
			Velocity.y = 0;
			Acceleration.y = 0;
			Position.y = screenHeight - Size.y;
		}
	}
}

void GameObject::SetCurrentAnimation(std::string animationName)
{
	currentAnimation = animationName;
}

void GameObject::AddForceByImpulse(glm::vec2 impulse)
{
	const int kf = 2;
	const int sf = 2;
	if (std::abs(impulse.x) > sf || true)
	{
 		Velocity = (Velocity + impulse) * Mass;
		if (impulse.x > 0)
		{
			Acceleration.x =  -Gravity * kf * Mass;
		}
		else
		{
			Acceleration.x =  Gravity * kf * Mass;
		}
		Acceleration.y = std::abs(Gravity) * Mass;
	}
}

void GameObject::SetColliderDimensions(glm::vec2 colliderOffset, glm::vec2 colliderSize)
{
	ColliderOffset = colliderOffset;
	ColliderSize = colliderSize;
}

glm::vec2 GameObject::GetColliderDimensions()
{
	return Position + ColliderOffset;
}

void GameObject::DrawCollider(Sprite_Renderer& renderer)
{
	renderer.DrawSprite(ResourceManager::GetTexture("greenOutline"), Position + ColliderOffset , ColliderSize);
}

void GameObject::AddAnimation(std::string animationName, Animation animation)
{
	ResourceManager::Animations[animationName] = animation;
}

void GameObject::Animate(std::string animationName, Sprite_Renderer& renderer, float dt)
{
	runningTime += dt;
	float fps = 12;

	Animation animation = ResourceManager::Animations[animationName];
	SubSprite currentFrame = animation.spriteMapFrames[animation.currentFrame];

	DrawFromSpriteSheet(renderer, currentFrame);

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
