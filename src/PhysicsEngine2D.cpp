#include "PhysicsEngine2D.h"
#include <iostream>

PhysicsEngine2D::PhysicsEngine2D(float gravity, float drag)
	:Gravity(gravity), Drag(drag) { }

void PhysicsEngine2D::HandleCollision(GameObject& object1, GameObject& object2)
{
	if (!object1.IsDestroyed && !object2.IsDestroyed)
	{
		Collision collision = CheckCollision(object1, object2);
		if (collision.isCollision)
		{
			glm::vec2 obj1Momentum = glm::vec2(object1.Velocity) * object1.Mass;
			if (collision.direction == LEFT)
			{
				object1.Velocity.x = 0;
				object1.Position.x = object2.Position.x - object1.ColliderOffset.x - object1.ColliderSize.x;
				object2.AddForceByImpulse(glm::vec2(obj1Momentum.x, obj1Momentum.x));
			}
			if (collision.direction == RIGHT)
			{
				object1.Velocity.x = 0;
				object1.Position.x = object2.Position.x - object1.ColliderOffset.x + object2.ColliderSize.x ;
				object2.AddForceByImpulse(glm::vec2(obj1Momentum.x, std::abs(obj1Momentum.x)));
			}
			if (collision.direction == UP)
			{
				object1.Velocity.y = 0;
				object1.Position.y =  object2.Position.y + object2.ColliderSize.y;
			}
			if (collision.direction == DOWN)
			{
 				object1.Velocity.y = 0;
				object1.Position.y = object2.Position.y - object1.ColliderOffset.y - object1.ColliderSize.y;
			}
			object1.HandleCollision(object2, collision.direction); // hits
			object2.HandleCollision(object1, collision.direction); // gets hit
		}
	}
}


void PhysicsEngine2D::AddForce(GameObject& object, glm::vec2 force){}

Direction PhysicsEngine2D::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[]
	{
		glm::vec2( 0.0,  1.0),	// up
		glm::vec2( 1.0,  0.0),	// right
		glm::vec2( 0.0, -1.0),	// down
		glm::vec2(-1.0,  0.0)	// left
	};

	float best_so_far = 0;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > best_so_far)
		{
			best_so_far = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

//Physics Functions
Collision PhysicsEngine2D::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
	glm::vec2 oneColliderPos = one.GetColliderDimensions();
	glm::vec2 twoColliderPos = two.GetColliderDimensions();

	// collision x-axis?
	bool collisionX = oneColliderPos.x + one.ColliderSize.x >= twoColliderPos.x &&
		twoColliderPos.x + two.ColliderSize.x >= oneColliderPos.x;
	// collision y-axis?
	bool collisionY = oneColliderPos.y + one.ColliderSize.y >= twoColliderPos.y &&
		twoColliderPos.y + two.ColliderSize.y >= oneColliderPos.y;
	// check Direction
	glm::vec2 diff = glm::vec2(oneColliderPos.x + one.ColliderSize.x / 2, oneColliderPos.y + one.ColliderSize.y / 2) - 
					 glm::vec2(twoColliderPos.x + two.ColliderSize.x / 2, twoColliderPos.y + one.ColliderSize.y / 2);
	
	Direction direction = VectorDirection(diff);
	return { collisionX && collisionY , direction};
}


