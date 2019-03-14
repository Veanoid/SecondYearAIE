#include "Rigidbody.h"
#include <math.h>




Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity, float rotation, float mass) : PhysicsObject(shapeID)
{
	m_position = position;
	m_velocity = velocity;
	m_rotation = rotation;
	m_mass = mass;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	applyForce(gravity * m_mass * timeStep);
	m_velocity -= m_velocity * m_linearDrag *timeStep;
	// m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;
	if (glm::length(m_velocity) < MIN_LINEAR_THRESHOLD)
	{
		m_velocity = glm::vec2(0, 0);
	}
	m_position += m_velocity * timeStep;
	// if (glm::abs(m_angularVelocity) < MIN_ROTATION_THRESHOLD)
	// {
	// 	m_angularVeloicity = 0;
	// }
}

void Rigidbody::debug()
{
}

void Rigidbody::applyForce(glm::vec2 force)
{
	if (!isStatic)
	{
		m_velocity += force / m_mass;
	}
}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}
/*
applying restutitution is where after two object hit one another they will seperate if they are overlaping
first it see if there is an actor than it will go through the if statement

*/
void Rigidbody::applyRestitution(glm::vec2 collisionNormal, float penatration, Rigidbody * Actor2)
{
	
	// checking to see if both actor and actor 2 aren't static at all also seeing if there is an actor 2
	if (Actor2 && !Actor2->getStatic() && !this->getStatic())
	{
		glm::vec2 move = collisionNormal * penatration / 2.0f;
		Actor2->SetPosition(move + Actor2->getPosition());
		SetPosition(getPosition() - move);
	}
	// seeing if there is no actor2 or if the actor2 is staic and actor isn't 
	else if(!Actor2 || Actor2->getStatic() && !this->getStatic())
	{
		penatration = abs(penatration);
		this->SetPosition(collisionNormal * penatration + this->getPosition());
	}
	// seeing if there is an actor2 and actor is static and actor 2 isn't 
	else if (Actor2 && this->getStatic() && !Actor2->getStatic())
	{
		//penatration = -abs(penatration);
		Actor2->SetPosition(collisionNormal * penatration + Actor2->getPosition());
	}
	if (this && isnan(this->getPosition().x))
	{
		int temp = 0;
	}
}

void Rigidbody::resolveCollision(Rigidbody * actor2, glm::vec2 collisionNormal)
{
	glm::vec2 normal = glm::normalize(collisionNormal);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = (m_elasticity + actor2->getElasitcity()) / 2.0f;
	float j;

	if (!this->getStatic() && !actor2->getStatic())
	{
		 j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
			glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));
	}
	else if (this->getStatic() && !actor2->getStatic())
	{
		 j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
			glm::dot(normal, normal * ((1 / actor2->getMass())));
	}
	else if (!this->getStatic() && actor2->getStatic())
	{
		 j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
			glm::dot(normal, normal * ((1 / m_mass)));
	}

	glm::vec2 force = normal * j;

	applyForceToActor(actor2, force);
	
}
