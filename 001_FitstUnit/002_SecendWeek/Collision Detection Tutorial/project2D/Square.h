#pragma once
#include "Rigidbody.h"
#include <vector>
#include <Gizmos.h>

class Square : public Rigidbody
{
public:

	Square(const glm::vec2 min, const glm::vec2 max, glm::vec2 position, glm::vec2 velocity, float mass, glm::vec4 colour);
	~Square();

	virtual void makeGizmo();

	glm::vec2 center() const;

	glm::vec2 extents() const;

	std::vector<glm::vec2> corners() const;
	glm::vec2 getMin() { return m_min; }
	glm::vec2 getMax() { return m_max; }


protected:

	glm::vec2 m_min, m_max;
	glm::vec4 m_colour; 
};

