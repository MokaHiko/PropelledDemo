#include "Platform.h"

Platform::Platform()
	:GameObject()
{
	Tag = "platform";
	isRigidBody = false;
	isGravity = false;
	Mass = 0;
}

Platform::Platform(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
	:GameObject(pos, size, sprite, glm::vec3(1.0f))
{
	Tag = "platform";
	isRigidBody = false;
	isGravity = false;
	Mass = 0;
}

void Platform::HandleCollision(GameObject& object, float direction)
{
}

