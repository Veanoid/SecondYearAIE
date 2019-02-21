#include "Rigidbody.h"




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
	m_velocity += force / m_mass;
}

void Rigidbody::applyForceToActor(Rigidbody * actor2, glm::vec2 force)
{
	actor2->applyForce(force);
	applyForce(-force);
}

void Rigidbody::applyRestitution(glm::vec2 collisionNormal, float penatration, Rigidbody * Actor2)
{
	if (Actor2)
	{
		glm::vec2 move = collisionNormal * penatration / 2.0f;
		Actor2->SetPosition(move + Actor2->getPosition());
		SetPosition(getPosition() - move);
	}
	else
	{
		this->SetPosition(collisionNormal * penatration + this->getPosition());
	}
}

void Rigidbody::resolveCollision(Rigidbody * actor2)
{
	glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);
	glm::vec2 relativeVelocity = actor2->getVelocity() - m_velocity;

	float elasticity = (m_elasticity + actor2->getElasitcity()) / 2.0f;


	float j = glm::dot(-(1 + elasticity) * (relativeVelocity), normal) /
		glm::dot(normal, normal * ((1 / m_mass) + (1 / actor2->getMass())));

	glm::vec2 force = normal * j;

	applyForceToActor(actor2, force);
}
