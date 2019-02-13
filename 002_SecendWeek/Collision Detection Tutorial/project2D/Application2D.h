#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include <glm\ext.hpp>
class PhysicsScene;
class Sphere;

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();
	void setupConinuousDemo(glm::vec2 startPos, float inclination, float speed, float gravity);

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Texture*		m_texture;
	aie::Texture*		m_shipTexture;
	aie::Font*			m_font;

	float m_cameraX, m_cameraY;
	float m_timer;

	PhysicsScene* m_physicsScene;

	Sphere* Rocket;
	

	float m_fule;
	float Rocket_mass;

	bool tankEmpty;
};