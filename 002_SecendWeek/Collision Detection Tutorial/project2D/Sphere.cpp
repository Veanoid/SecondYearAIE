#include "Sphere.h"
#include <Gizmos.h>





Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float mass, float radius, glm::vec4 colour) :
	Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere()
{
}

void Sphere::makeGizmo()
{
	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
}

bool Sphere::checkCollision(PhysicsObject * pOther)
{
	Sphere* sOther = dynamic_cast <Sphere*> (pOther);
	float distant = glm::length(m_position - sOther->getPosition());
	if (distant < m_radius + sOther->getRadius())
	{
		return true;
	}
	return false;
}
