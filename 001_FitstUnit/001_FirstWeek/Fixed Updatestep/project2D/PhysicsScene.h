#pragma once
#include <glm\ext.hpp>
#include <vector>
#include "PhysicsObject.h"

class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	void debugScene();

	void addActor(PhysicsObject* actor);
	bool removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	protected:
		glm::vec2 m_gravity;
		float m_timeStep;
		std::vector<PhysicsObject* > m_actors;


};

