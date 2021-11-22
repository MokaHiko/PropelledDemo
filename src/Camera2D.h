#pragma once
#include "glm/glm.hpp"
#include "GameObject.h"
class Camera2D
{
public:
	float ScreenHeight, ScreenWidth;
	Camera2D(float screenWidth, float screenHeight, GameObject* objectToFollow);
	glm::mat4 GetViewMatrix();
private:
	float Zoom;
	GameObject* ObjectToFollow;
};

