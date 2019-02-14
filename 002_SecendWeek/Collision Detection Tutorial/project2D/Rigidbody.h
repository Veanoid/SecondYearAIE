#pragma once
#include "PhysicsObject.h"
#define MIN_LINEAR_THRESHOLD 0.01
#define MIN_ROTATION_THRESHOLD 0.1

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position,
		glm::vec2 velocity, float rotation, float mass);
	~Rigidbody();

	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();
	void applyForce(glm::vec2 force);
	void applyForceToActor(Rigidbody* actor2, glm::vec2 force);

	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void resolveCollision(Rigidbody* actor2);

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation;

	float m_linearDrag;
	float m_angularDrag;
};

