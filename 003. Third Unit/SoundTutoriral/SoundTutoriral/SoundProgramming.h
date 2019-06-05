#pragma once
#include <fmod.h>
#include <fmod.hpp>
class SoundProgramming
{
public:
	SoundProgramming();
	~SoundProgramming();

	bool startup();

private:

	FMOD::System *m_pfmodSystem;
}

