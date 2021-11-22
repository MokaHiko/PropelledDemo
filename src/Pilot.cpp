#include "Pilot.h"
#include <GLFW/glfw3.h>
#include <iostream>

float runningTime = 0;
Pilot::Pilot()
	:GameObject(), MovementSpeed(100.0f), JumpForce(100.0f), Propelled(false), Fuel(100), isGrounded(true)
{
	Tag = "player";
}

Pilot::Pilot(glm::vec2 pos, glm::vec2 size, Texture2D sprite, float movementSpeed, float jumpForce)
	:GameObject(pos, size, sprite, glm::vec3(1.0f)), MovementSpeed(movementSpeed), JumpForce(jumpForce), Propelled(false), Fuel(100) , isGrounded(true)
{
	Tag = "player";
}

void Pilot::PlayerController(bool* Keys, float screenWidth, float screenHeight, float dt, glm::vec2 mousePos, bool* mouseButtons)
{
	float playerVelocity =  MovementSpeed * dt;
		if (Keys[GLFW_KEY_LEFT_SHIFT] && Fuel > 0)
		{
			Propelled = true;
			Fuel = 0.2;
			isGrounded = false;
		}
		else
		{
			Propelled = false;
		}

		if (Keys[GLFW_KEY_SPACE] && Keys[GLFW_KEY_LEFT_SHIFT])
		{
			currentAnimation = "jump";
			if (Propelled)
			{
				PlayerParticles->SetTexture("particle");
				Velocity.y =  JumpForce * dt * 3;
			}
		}
		else if (Keys[GLFW_KEY_SPACE])
		{
				if (isGrounded)
				{
					Velocity.y =  JumpForce * dt * 15;
					PlayerParticles->SetTexture("blueSquareParticle");
					isGrounded = false;
				}
		}

		if (Keys[GLFW_KEY_A])
		{
			if (Propelled)
			{
				currentAnimation = "propelled";
				if (Keys[GLFW_KEY_SPACE])
				{
					AddForceByImpulse(glm::vec2(-playerVelocity * 0.1f, 0));
				}
				else
				{
					AddForceByImpulse(glm::vec2(-playerVelocity * 0.4f, 0));
				}
			}
			else
			{
				currentAnimation = "walk";
				Position.x -= playerVelocity;
			}
		}
		else if (Keys[GLFW_KEY_D])
		{
			if (Propelled)
			{
				currentAnimation = "propelled";
				if (Keys[GLFW_KEY_SPACE])
				{
					AddForceByImpulse(glm::vec2(playerVelocity * 0.1f, 0));
				}
				else
				{
					AddForceByImpulse(glm::vec2(playerVelocity * 0.4f, 0));
				}
			}
			else
			{
				currentAnimation = "walk";
				Position.x += playerVelocity;
			}
		}
		else
		{
			currentAnimation = "idle";
		}

		runningTime += dt;
		if (mouseButtons[GLFW_MOUSE_BUTTON_LEFT])
		{
			if (runningTime >= 0.1)
			{
				runningTime = 0;
				ShootProjectile(screenWidth, screenHeight);
			}
		}	
		// check on floor
		if (Position.y + ColliderSize.y == screenHeight)
		{
			isGrounded = true;
		}

		// projectiles
		MousePos = mousePos;
}

void Pilot::HandleCollision(GameObject& object, float direction)
{
	if (object.Tag == "enemy" && direction == DOWN)
	{
		float direction = Position.x > object.Position.x ? -1 : 1;
		AddForceByImpulse(glm::vec2(0, JumpForce * 0.2));
	}
	else if (object.Tag == "platform")
	{
		isGrounded = true;
	}
}

void Pilot::ShootProjectile(float screenWidth, float screenHeight)
{
	glm::vec2 aim = glm::normalize(glm::vec2(MousePos.x - screenWidth/2 , - (MousePos.y - screenHeight / 2)));
	PlayerProjectiles->Launch(1, glm::vec2(Position.x + Size.x/2, Position.y), aim);
}

void Pilot::SetProjectile(Projectile& projectile)
{
	PlayerProjectiles->SetProjectile(projectile);
}




