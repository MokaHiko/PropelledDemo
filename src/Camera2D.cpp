#include "Camera2D.h"

Camera2D::Camera2D(float screenWidth, float screenHeight, GameObject* objectToFollow)
	: ScreenWidth(screenWidth), ScreenHeight(screenHeight), Zoom(1.0), ObjectToFollow(objectToFollow)
{
		
}

glm::mat4 Camera2D::GetViewMatrix()
{
	float offsetX = -(ObjectToFollow->Position.x - ScreenWidth /2);
	float offsetY = -ObjectToFollow->Position.y + ScreenHeight /2;
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(offsetX, offsetY, 0));
	view = glm::scale(view, glm::vec3(1, 1, 1));
	return view;
}

