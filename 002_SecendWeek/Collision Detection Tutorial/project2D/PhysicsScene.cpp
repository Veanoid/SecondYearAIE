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

bool PhysicsScene::plane2Box(PhysicsObject * a, PhysicsObject * b)
{
	return box2Plane(b, a);
}

bool PhysicsScene::sphere2Plane(PhysicsObject * a, PhysicsObject * b)
{
	Sphere *sphere = dynamic_cast<Sphere*>(a);
	Plane *plane = dynamic_cast<Plane*>(b);

	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();

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
			sphere->applyRestitution(collisionNormal, intersection);
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
		glm::vec2 collisionNormal = sphere1->getPosition() - sphere2->getPosition();
		float distant = glm::length(collisionNormal);
		if (distant < sphere1->getRadius() + sphere2->getRadius())
		{
			collisionNormal = glm::normalize(collisionNormal);
			sphere1->applyRestitution(collisionNormal, distant - (sphere1->getRadius() + sphere2->getRadius()), sphere2);
			sphere1->resolveCollision(sphere2, collisionNormal);
			//sphere2->setVelocity(glm::vec2(0,0));
			
		}
	}
	return false;
}

bool PhysicsScene::sphere2Box(PhysicsObject *a, PhysicsObject *b)
{
	Sphere *sphere1 = dynamic_cast<Sphere*>(a);
	Square *square1 = dynamic_cast<Square*>(b);
	glm::vec2 point = glm::clamp(sphere1->getPosition(), square1->getPosition() + square1->getMin(), square1->getPosition() + square1->getMax());
	glm::vec2 distant = sphere1->getPosition() - point;
	if (glm::length(distant) <= sphere1->getRadius())
	{
		glm::vec2 collisionNormal = glm::normalize(distant);
		sphere1->applyRestitution(collisionNormal, glm::length(distant) - sphere1->getRadius(), square1);
		sphere1->resolveCollision(square1, collisionNormal);
		
		square1->InvertFilled();
	}


	return false;
}

bool PhysicsScene::box2Plane(PhysicsObject *a, PhysicsObject *b)
{

	Square *square = dynamic_cast<Square*>(a);
	Plane *plane = dynamic_cast<Plane*>(b);
	if (square == nullptr || plane == nullptr)
		return false;

	glm::vec2 collisionNormal = plane->getNormal();
	std::vector<glm::vec2> corners = square->corners();
	float CornersToLine[4];
	for (int i = 0; i < corners.size(); i++)
	{
		//if we are successful then test for collision
		float SquaretoPlan = glm::dot(square->getPosition() + corners[i], plane->getNormal()) - plane->getDistance();

		CornersToLine[i] = SquaretoPlan;
	}

	bool BRCube = CornersToLine[0] < 0 && CornersToLine[2] > 0;
	bool BLCube = CornersToLine[1] > 0 && CornersToLine[3] < 0;

	if (BRCube || BLCube)
	{
		float tration;
		if (BRCube)
		{
			tration = glm::min(abs(CornersToLine[0]), abs(CornersToLine[2]));
		}
		else
		{
			tration = glm::min(abs(CornersToLine[1]), abs(CornersToLine[3]));
		}

		float behind = glm::dot(square->getPosition(), plane->getNormal()) - plane->getDistance();
		if (behind < 0)
		{
			collisionNormal = -collisionNormal;
		}

		square->applyRestitution(collisionNormal, tration);
		plane->resolveCollision(square);
		square->InvertFilled();
		return true;
		
	}

	return false;
}

bool PhysicsScene::box2Sphere(PhysicsObject *a, PhysicsObject *b)
{
	return sphere2Box(b, a);
}

bool PhysicsScene::box2Box(PhysicsObject *a, PhysicsObject *b)
{

	float pen;
	glm::vec2 ncoll;

	Square *square1 = dynamic_cast<Square*>(a);
	Square *square2 = dynamic_cast<Square*>(b);

	glm::vec2 pos1 = square1->getPosition();
	glm::vec2 pos2 = square2->getPosition();

	glm::vec2 faces[4]
	{
		glm::vec2(-1,0),	//left
		glm::vec2(1,0),		// right
		glm::vec2(0,-1),	// bottom
		glm::vec2(0,1)		// top
	};
	float distances[4]
	{
		(square2->getMax().x + pos2.x) - (square1->getMin().x + pos1.x),
		(square1->getMax().x + pos1.x) - (square2->getMin().x + pos2.x) ,
		(square2->getMax().y + pos2.y) - (square1->getMin().y + pos1.y),
		(square1->getMax().y + pos1.y) - (square2->getMin().y + pos2.y)

	};


	for (int i = 0; i < 4; i++)
	{
		if (distances[i] < 0.0f)
			return false;

		if (i == 0 || (distances[i] < pen))
		{
			ncoll = faces[i];
			pen = distances[i];
		}
	}
	square1->applyRestitution(ncoll, pen, square2);
	square1->resolveCollision(square2, ncoll);
	square1->InvertFilled();
	square2->InvertFilled();
	return true;
}


