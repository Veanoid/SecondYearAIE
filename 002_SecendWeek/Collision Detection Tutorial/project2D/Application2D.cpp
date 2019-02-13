#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <glm\ext.hpp>
#include "PhysicsScene.h"
#include <Gizmos.h>
#include "Sphere.h"


Application2D::Application2D() {

}

Application2D::~Application2D() {

}

bool Application2D::startup() {
	
	//increase the 2d line count to maximize the number of object we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	m_2dRenderer = new aie::Renderer2D();

	m_texture = new aie::Texture("./textures/numbered_grid.tga");
	m_shipTexture = new aie::Texture("./textures/ship.png");

	m_font = new aie::Font("./font/consolas.ttf", 32);
	
	m_cameraX = 0;
	m_cameraY = 0;
	m_timer = 0;
	Rocket_mass = 60.0f;
	m_fule = 10.0f;
	tankEmpty = false;
	
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, 0));
	m_physicsScene->setTimeStep(0.01f);

	Sphere* ball1 = new Sphere(glm::vec2(-40, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(ball1);
	Sphere* ball2 = new Sphere(glm::vec2(40, 0), glm::vec2(0, 0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
	//m_physicsScene->addActor(ball2);

	ball1->applyForceToActor(ball2, glm::vec2(2, 0));

	ball1->applyForce(glm::vec2(300, 0));
	ball2->applyForce(glm::vec2(-150, 0));

	Rocket = new Sphere(glm::vec2(0, -10), glm::vec2(0,0), Rocket_mass, 4, glm::vec4(1,1,1,1));
	//m_physicsScene->addActor(Rocket);


	setupConinuousDemo(glm::vec2(-40, 0), 45, 30, -9.8f);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -10));
	m_physicsScene->setTimeStep(0.5f);


	 
	float radius = 1.0f;
	float speed = 30;
	glm::vec2 startPos(-40, 0);
	float inclination = 45 * 0.0174533;


	float x = cosf(inclination) * speed;
	float y = sinf(inclination) * speed;

	glm::vec2 velocity(x, y);

	m_physicsScene->addActor(new Sphere(
		startPos, velocity, 1, radius, glm::vec4(1, 0, 0, 1)));


	return true;
}

void Application2D::shutdown() {
	
	delete m_font;
	delete m_texture;
	delete m_shipTexture;
	delete m_2dRenderer;
}

void Application2D::update(float deltaTime)
{
	Rocket->setColour(glm::vec4((int)rand % 100 / 100.0f,(int) rand % 100 / 100.0f, (int)rand % 100 / 100.0f, 1));
	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	//aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// use arrow keys to move camera
		if(input->isKeyDown(aie::INPUT_KEY_SPACE) && !tankEmpty)
		{
			/*Rocket_mass -= 0.1f;
			Rocket->setMass(Rocket_mass);
			m_fule -= 0.1f;

			if (m_fule <= 5.0f)
			{
				if (m_fule > 5.0f)
				{
					tankEmpty = false;
				}
				else
				{
					tankEmpty = true;
				}

			}
			if (m_timer >= 2)
			{
				Sphere* Gas = new Sphere(glm::vec2(Rocket->getPosition() - glm::vec2(0, 5)), glm::vec2(0, 0), 10.0f, 0.5f, glm::vec4(1, 1, 1, 1));
				m_physicsScene->addActor(Gas);
				Rocket->applyForceToActor(Gas, glm::vec2(0, -5));
				m_timer = 0;
			}
				*/


		}

	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_cameraY += 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_cameraY -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		m_cameraX -= 500.0f * deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		m_cameraX += 500.0f * deltaTime;

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// set the camera position before we begin rendering
	m_2dRenderer->setCameraPos(m_cameraX, m_cameraY);

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// done drawing sprites
	m_2dRenderer->end();
}

void Application2D::setupConinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity)
{
	float t = 0;
	float tStep = 0.5f;
	float radius = 1.0f;
	int segments = 12;
	glm::vec4 colour = glm::vec4(1, 1, 0, 1);
	inclination *= 0.0174533;

	while (t <= 5)
	{
		float x;
		float y;

		x = startPos.x + cosf(inclination) * speed * t;
		y = startPos.y + sinf(inclination) * speed *t + 0.5f * gravity*t*t;

		aie::Gizmos::add2DCircle(glm::vec2(x, y), radius, segments, colour);
		t += tStep;
	}
}
