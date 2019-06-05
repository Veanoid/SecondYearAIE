#pragma once

#include "Application.h"
#include <Renderer2D.h>
#include <Font.h>
#include <glm/mat4x4.hpp>
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>
#include <string>
#include <Texture.h>


class SoundTutoriralApp : public aie::Application {
public:

	SoundTutoriralApp();
	virtual ~SoundTutoriralApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D* m_2dRenderer;
	aie::Font* m_font;
	// camera transforms
	FMOD::System *m_pfmodSystem;
	FMOD::Sound* pSound = nullptr;
	FMOD::Channel* pChannel;
	FMOD_RESULT result;
	FMOD::ChannelGroup* pChannelGroup;

	FMOD_VECTOR position = { 0,0,0 };
	FMOD_VECTOR velocity = { 0,0,0 };
	FMOD_VECTOR forward = { 1,0,0 };
	FMOD_VECTOR up = { 0,1,0 };

	FMOD_VECTOR channel_position;
	FMOD_VECTOR Channel_velocity;
	bool *isplaying = false;
	bool *isPaused = false;
	std::vector<std::string> playList;

	int currentSong;

	//aie::Texture PlayButton = aie::Texture("./playButton.png");
};

