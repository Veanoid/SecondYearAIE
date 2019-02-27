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
	void applyRestitution(glm::vec2 collisionNormal, float penatration,Rigidbody* Actor2 = nullptr);

	void SetPosition(glm::vec2 position) { m_position = position; }
	glm::vec2 getPosition() { return m_position; }
	float getRotation() { return m_rotation; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
	void resolveCollision(Rigidbody* actor2, glm::vec2 collisionNormal);
	float getElasitcity() { return m_elasticity; }

	void InvertFilled() { m_bIsFilled = !m_bIsFilled; };

	bool getStatic() { return isStatic; };
	void setStatic(bool gimmestatic) { isStatic = gimmestatic; }

protected:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation;

	bool m_bIsFilled = true;

	float m_linearDrag = 0;
	float m_angularDrag;
	float m_elasticity = 1;

	bool isStatic = false;
};

