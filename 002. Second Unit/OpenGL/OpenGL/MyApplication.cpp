#define GLM_ENABLE_EXPERIMENTAL
#include "MyApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "Shader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;


MyApplication::MyApplication()
{
}


MyApplication::~MyApplication()
{
}

bool MyApplication::startup()
{

	if (glfwInit() == false)
		return false;
	window = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);


	// Camera
	aie::Gizmos::create(10000, 10000, 10000, 10000);
	view = glm::lookAt(vec3(20, 20, 20), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	m_positions[0] = glm::vec3(10, 5, 10);
	m_positions[1] = glm::vec3(-10, 0, -10);
	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));

	m_hipFrames[0].position = glm::vec3(0, 5, 0);
	m_hipFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_hipFrames[1].position = glm::vec3(0, 5, 0);
	m_hipFrames[1].rotation = glm::quat(glm::vec3(-1, 0, 0));

	m_kneeFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_kneeFrames[1].position = glm::vec3(0, -2.5, 0);
	m_kneeFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_ankleFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[0].rotation = glm::quat(glm::vec3(-1, 0, 0));
	m_ankleFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));


	
	m_gridTexture.load("../data/numbered_grid.tga");

	aie::Texture texture2;
	unsigned char texelData[4] = { 0, 255, 255, 0 };
	texture2.create(2, 2, aie::Texture::RED, texelData);


	// loading in spear
	if (m_spearMesh.load("../data/soulspear/soulspear.obj", true, true) == false)
	{
		printf("SoulSpear Mesh Error!\n");
		return false;
	}

	m_spearTransform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};


	// load vertex shader from file
	m_shader.loadShader(aie::eShaderStage::VERTEX, "../OpenGL/simple.vert");
	//load fragment shader from file
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "../OpenGL/simple.frag");

	if (m_shader.link() == false)
	{
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}
	// loading in bunny
	if (m_bunnyMesh.load("../data/stanford/Bunny.obj") == false)
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	m_bunnyTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		0,0,0,1
	};


	m_quadTransform =
	{
		10,0,0,0,
		0,10,0,0,
		0,0,10,0,
		0,0,0,1

	};

	m_quadMesh.initialiseQuad();

	Mesh::Vertex vertices[6];
	//vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	//vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	//vertices[2].position = { -0.5, 0, -0.5f, 1 };
	//vertices[3].position = { 0.5f, 0, -0.5, 1 };
	//
	//vertices[4].position = { -0.5f, 1, 0.5f, 1 };
	//vertices[5].position = { 0.5f, 1, 0.5f, 1 };
	//vertices[6].position = { -0.5, 1, -0.5f, 1 };
	//vertices[7].position = { 0.5f, 1, -0.5, 1 };
	//
	//unsigned int indices[36] = { 0, 1, 2, 2, 1, 3,
	//							4, 5, 6, 6, 5, 7,
	//							0, 1, 4, 4, 5, 1,
	//							0, 2, 4, 4, 6, 2,
	//							2, 3, 6, 6, 7, 3,
	//							1, 3, 5, 5, 7, 3};

	//m_quadMesh.initialise(8, vertices, 36, indices);

	
	
	m_light.direction = { 0, 1, 0 };
	m_light.diffuse = { 1, 1, 1 };
	m_light.specular = { 1, 1, 0 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };


	return true;
	
}

bool MyApplication::update()
{

	
	
	glfwSwapBuffers(window);
	glfwPollEvents();

	// query time since application started
	float time = glfwGetTime();

	// rotate light
	m_light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));
	
	return glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

void MyApplication::draw()
{

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	aie::Gizmos::clear();

	aie::Gizmos::addTransform(glm::mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; i++)
	{
		aie::Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		aie::Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	float s = glm::cos(glfwGetTime()) * 0.5f + 0.5f;

	//Hip Bone
	glm::vec3 p = (1.0f - s) * m_hipFrames[0].position + s * m_hipFrames[1].position;
	glm::quat r = glm::slerp(m_hipFrames[0].rotation, m_hipFrames[1].rotation, s);
	m_hipBone = glm::translate(p) * glm::toMat4(r);

	//Knee
	glm::vec3 w = (1.0f - s) * m_kneeFrames[0].position + s * m_kneeFrames[1].position;
	glm::quat t = glm::slerp(m_kneeFrames[0].rotation, m_kneeFrames[1].rotation, s);
	m_kneeBone = glm::translate(w) * glm::toMat4(t);
	m_kneeBone = m_hipBone * m_kneeBone;

	//Ankle
	glm::vec3 e = (1.0f - s) * m_ankleFrames[0].position + s * m_ankleFrames[1].position;
	glm::quat y = glm::slerp(m_ankleFrames[0].rotation, m_ankleFrames[1].rotation, s);
	m_ankleBone = glm::translate(e) * glm::toMat4(y);
	m_ankleBone = m_kneeBone * m_ankleBone;

	aie::Gizmos::addTransform(m_hipBone);
	aie::Gizmos::addAABBFilled(p, glm::vec3(.5f), glm::vec4(1, 0, 0, 1), &m_hipBone);

	glm::vec3 hipPos = glm::vec3(m_hipBone[3].x, m_hipBone[3].y, m_hipBone[3].z);
	glm::vec3 kneePos = glm::vec3(m_kneeBone[3].x, m_kneeBone[3].y, m_kneeBone[3].z);
	glm::vec3 anklePos = glm::vec3(m_ankleBone[3].x, m_ankleBone[3].y, m_ankleBone[3].z);

	glm::vec4 half(0.5f);
	glm::vec4 pink(1, 0, 1, 1);

	aie::Gizmos::addAABBFilled(hipPos, half, pink, &m_hipBone);
	aie::Gizmos::addAABBFilled(kneePos, half, pink, &m_kneeBone);
	aie::Gizmos::addAABBFilled(anklePos, half, pink, &m_ankleBone);


	// drawing Grid
	auto pvm = projection * view * m_quadTransform;
	m_shader.bindUniform("projection", pvm);

	m_shader.bind();

	// bind light
	m_shader.bindUniform("Ia", m_ambientLight);
	m_shader.bindUniform("Id", m_light.diffuse);
	m_shader.bindUniform("Is", m_light.specular);
	m_shader.bindUniform("LightDirection", m_light.direction);

	m_shader.bindUniform("cameraPosition", vec3(glm::inverse(view)[3]));

	m_gridTexture.bind(0);

	m_quadMesh.draw();

	


	aie::Gizmos::draw(projection * view);
	
	// drawing bunny
	auto pvn = projection * view * m_bunnyTransform;
	m_shader.bindUniform("projection", pvn);

	// bind transforms for lighting 
	m_shader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_bunnyTransform)));

	m_bunnyMesh.draw();

	// draw Spear
	auto ppm = projection * view * m_spearTransform;
	m_shader.bindUniform("projection", ppm);

	// bind transforms for lighting 
	m_shader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));

	m_spearMesh.draw();

	

	// bind transform
	auto ppp = projection * view * m_quadTransform;
	m_shader.bindUniform("projection", ppp);

	// bind transform for lighting 
	m_shader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_quadTransform)));

	m_quadMesh.draw();

}

void MyApplication::shutdown()
{
	aie::Gizmos::destroy();
	glfwDestroyWindow(window);
	glfwTerminate();
}
