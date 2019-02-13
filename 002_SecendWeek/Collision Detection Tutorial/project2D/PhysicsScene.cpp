#include "PhysicsScene.h"
#include <list>
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include <iostream>
#include "Sphere.h"
#include "Plane.h"

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
		// check for collisions (ideally you'd want to have some sort of scene managment in place)
		for (auto pActor : m_actors)
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
	dirty.clear();
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
	PhysicsScene::plane2Plane, PhysicsScene::plane2Sphere,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere,
};

void PhysicsScene::checkForCollision()
{
}

bool PhysicsScene::plane2Plane(PhysicsObject * a, PhysicsObject * b)
{
	return false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject * a, PhysicsObject * b)
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
		//this is where the collision detection happens
		//you need code which sets the velocity of the two sphere to zero
		//if they are overlapping
		//get distance between 2 spheres
		//if distance is less than the combind radius of
		//both sphere, then a collision occurred so set the 
		// velocity of both sphere to 0 (we'll add collision resolution later)
	
		float distant = glm::length(sphere1->getPosition() - sphere2->getPosition());
		if (distant < sphere1->getRadius() + sphere2->getRadius())
		{
			sphere1->setVelocity(glm::vec2(0,0));
			sphere2->setVelocity(glm::vec2(0,0));
		}


	}
	return false;
}


