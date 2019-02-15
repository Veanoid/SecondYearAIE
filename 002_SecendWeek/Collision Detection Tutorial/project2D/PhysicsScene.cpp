#include "PhysicsScene.h"
#include <list>
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"
#include "Square.h"

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0,0))
{

}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}

void PhysicsScene::addActor(PhysicsObject * actor)
{
	m_actors.push_back(actor);
}

bool PhysicsScene::removeActor(PhysicsObject * actor)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i] == actor)
		{
			m_actors.erase(m_actors.begin() + i);
			return true;
		}
	}
	return false;
}

void PhysicsScene::update(float dt)
{
	static std::list<PhysicsObject*> dirty;
	// update physics at a fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}
		accumulatedTime -= m_timeStep;
		checkForCollision();
		// check for collisions (ideally you'd want to have some sort of scene managment in place)
	/*	for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
				{
					continue;
				}
				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() && std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
				{
					continue;
				}

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				if (pRigid->checkCollision(pOther) == true)
				{
					pRigid->applyForceToActor(
						dynamic_cast<Rigidbody*>(pOther),
						pRigid->getVelocity() * pRigid->getMass());
					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
	dirty.clear();*/

	}
	
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

typedef bool(*fn)(PhysicsObject* a, PhysicsObject* b);

static fn collisionFuntionArray[] =
{
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere, PhysicsScene::plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box
};

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

	//need to check for collision against all objects except this one.
	for (int outer = 0; outer < actorCount; outer++)
	{
		for (int inner = outer + 1; + inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			//uising function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFuntionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				//did a collision occur?
				collisionFunctionPtr(object1, object2);

			}
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject * a, PhysicsObject * b)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject * a, PhysicsObject * b)
{
	return sphere2Plane(b, a);
}

bool PhysicsScene::plane2Box(PhysicsObject *, PhysicsObject *)
{
	return false;
}

bool PhysicsScene::sphere2Plane(PhysicsObject * a, PhysicsObject * b)
{
	Sphere *sphere = dynamic_cast<Sphere*>(a);
	Plane *plane = dynamic_cast<Plane*>(b);

	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(
			sphere->getPosition(), plane->getNormal()) - plane->getDistance();

		//if we are behind then we flip normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}
		float intersection = sphere->getRadius() - sphereToPlane;
		if (intersection > 0)
		{
			//set sphere velocity to zero here
			plane->resolveCollision(sphere);
			return true;
		}
	}

	return false;
}

bool PhysicsScene::sphere2Sphere(PhysicsObject * a, PhysicsObject * b)
{
	//try to cast object to sphere and sphere
	Sphere *sphere1 = dynamic_cast<Sphere*>(a);
	Sphere *sphere2 = dynamic_cast<Sphere*>(b);
	//if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float distant = glm::length(sphere1->getPosition() - sphere2->getPosition());
		if (distant < sphere1->getRadius() + sphere2->getRadius())
		{
			sphere1->resolveCollision(sphere2);
			//sphere2->setVelocity(glm::vec2(0,0));
			
		}
	}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject *a, PhysicsObject *b)
{
	Sphere *sphere1 = dynamic_cast<Sphere*>(a);
	Square *square1 = dynamic_cast<Square*>(b);
	glm::vec2 point = glm::clamp(sphere1->getPosition(), square1->getMin(), square1->getMax());
	glm::vec2 distant = point - sphere1->getPosition();
	if (glm::length(distant) * glm::length(distant) <= sphere1->getRadius()* sphere1->getRadius());
	{
		sphere1->resolveCollision(square1);
	}


	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject *a, PhysicsObject *b)
{
	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject *a, PhysicsObject *b)
{
	return sphere2Box(b, a);
}

bool PhysicsScene::box2Box(PhysicsObject *a, PhysicsObject *b)
{
	Square *square1 = dynamic_cast<Square*>(a);
	Square *square2 = dynamic_cast<Square*>(b);

	bool noOverlap = square1->getMax().x < square2->getMin().x || square2->getMax().x < square1->getMin().x || square1->getMax().y < square2->getMin().y || square2->getMax().y < square1->getMin().y;
	if (!noOverlap)
	{
		square1->resolveCollision(square2);
	}
	return !noOverlap;
}


