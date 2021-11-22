#pragma once
#include <vector>

#include "GameObject.h"
#include "Enemy.h"

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

struct Collision
{
	bool isCollision;
	Direction direction;
};

class PhysicsEngine2D
{
public:
	float Gravity;
	float Drag;

	PhysicsEngine2D(float gravity = 9.8, float drag = 0);

	//Physics Functions
	void HandleCollision(GameObject& object1, GameObject& object2);
	void AddForce(GameObject& object, glm::vec2 force);
private:
	Collision CheckCollision(GameObject& one, GameObject& two);
	Direction VectorDirection(glm::vec2 target);
};

