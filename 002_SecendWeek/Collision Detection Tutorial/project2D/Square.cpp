#include "Square.h"




Square::Square(const glm::vec2 min, const glm::vec2 max, glm::vec2 position, glm::vec2 velocity, float mass, glm::vec4 colour) : Rigidbody(BOX, position, velocity, 0, mass )
{
	m_min = min;
	m_max = max;
	m_colour = colour;
	
}


Square::~Square()
{
}

void Square::makeGizmo()
{
	if (m_bIsFilled)
		aie::Gizmos::add2DAABBFilled(m_position, extents(), m_colour);
	else
		aie::Gizmos::add2DAABB(m_position, extents(), m_colour);
}

glm::vec2 Square::center() const
{
	return (m_min + m_max) * 0.5f;
}

glm::vec2 Square::extents() const
{
	return { abs(m_max.x - m_min.x) * 0.5, abs(m_max.y - m_min.y) * 0.5f };
}

std::vector<glm::vec2> Square::corners() const
{
	std::vector<glm::vec2> corners(4);
	corners[0] = m_min;
	corners[1] = { m_min.x, m_max.y };
	corners[2] = m_max;
	corners[3] = { m_max.x, m_min.y };
	return corners;

}


