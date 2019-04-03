#pragma once
#include "Application.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "Gizmos.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "tiny_obj_loader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

class MyApplication : public Application
{
public:
	MyApplication();
	virtual ~MyApplication();

	virtual bool startup();
	virtual bool update();
	virtual void draw();
	virtual void shutdown();

	struct KeyFrame
	{
		glm::vec3 position;
		glm::quat rotation;
	};



protected:
	GLFWwindow*				window;
	mat4					view;
	mat4					projection;

	glm::vec3				m_positions[2];
	glm::quat				m_rotations[2];

	KeyFrame				m_hipFrames[2];
	KeyFrame				m_kneeFrames[2];
	KeyFrame				m_ankleFrames[2];

	glm::mat4				m_hipBone;
	glm::mat4				m_kneeBone;
	glm::mat4				m_ankleBone;

	aie::Texture			m_gridTexture;


	aie::ShaderProgram		m_shader;
	Mesh					m_quadMesh;
	glm::mat4				m_quadTransform;

	aie::OBJMesh			m_bunnyMesh;
	glm::mat4				m_bunnyTransform;

	aie::OBJMesh			m_spearMesh;
	glm::mat4				m_spearTransform;

	struct Light
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
	Light					m_light;
	glm::vec3				m_ambientLight;

};

