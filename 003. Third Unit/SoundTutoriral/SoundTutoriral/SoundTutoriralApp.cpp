#include "SoundTutoriralApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#define SPECTRUMSIZE 4096


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

SoundTutoriralApp::SoundTutoriralApp() {
}

SoundTutoriralApp::~SoundTutoriralApp() {

}

bool SoundTutoriralApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	isplaying = new bool(false);
	isPaused = new bool(false);
	currentSong = 0;


	m_2dRenderer = new aie::Renderer2D;
	
	m_font = new aie::Font("consolas.ttf", 20);
	// Other initialisation code here..
	// Initialize FMOD
	
	m_pfmodSystem = nullptr;

	// Create the system object.
	result = FMOD::System_Create(&m_pfmodSystem);

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %\n", result, FMOD_ErrorString(result));
		return false;
	}

	// Initialize FMOD with 512 channels
	result = m_pfmodSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	playList.push_back("Pretending.mp3");
	playList.push_back("Crouch Drive to the Blanc Castle.mp3");
	playList.push_back("Unlike Pluto - Everything Black (feat. Mike Taylor) [Official Lyric Video].mp3");
	playList.push_back("This Town.mp3");
	playList.push_back("Warrior by Jaxson Gamble.mp3");

	result = m_pfmodSystem->createChannelGroup("MyChannelGroup", &pChannelGroup);

	return true;
}

void SoundTutoriralApp::shutdown() {
	delete m_2dRenderer;
	delete m_font;
	pSound->release();

	m_pfmodSystem->close();
	m_pfmodSystem->release();
}

static int test = 0;
static int testCount = 0;

void SoundTutoriralApp::update(float deltaTime)
{

	m_pfmodSystem->update();
	pChannel->isPlaying(isplaying);
	test++;
	if (test >= 10)
	{
		test = 0;
		testCount++;
		printf("%i\n", testCount);
		FMOD::DSP* fftDSP;
		result = m_pfmodSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &fftDSP);
		if (pChannelGroup != nullptr)
		{
			result = pChannelGroup->addDSP(0 /* = Head of the DSP chain. */, fftDSP);

			// Now that the dsp is attached, we can access it

			void* data;
			unsigned int length;
			result = fftDSP->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, &data, &length, 0, 0);

			float soundSum = 0;
			for (int i = 0; i < length - 1; i++)
			{
				soundSum += ((float*)data)[i];
				printf("Sound Sum @%i = %f\n", i, soundSum);
			}
			printf("%f\n", circle);
			circle = soundSum / length;
			pChannelGroup->removeDSP(fftDSP);
		}

		//system("cls");
	}
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();
	if (*isplaying == false)
	{
		
		pSound->release();
		result = m_pfmodSystem->createSound(playList[currentSong].c_str(), FMOD_DEFAULT, 0, &pSound);
		result = m_pfmodSystem->playSound(pSound, pChannelGroup, false, &pChannel);
		currentSong++;
		if (currentSong >= playList.size())
		{
			currentSong = 0;
		}

	}

	if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
	{	
		
		pChannel->getPaused(isPaused);
		if(*isPaused == false && *isplaying == true)
		{
			pChannel->setPaused(true);
		}
		else
		{
			pChannel->setPaused(false);
		}
	}
	if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT))
	{
		if (*isplaying == true)
		{
			pChannel->stop();
		}
	}
	if (input->wasKeyPressed(aie::INPUT_KEY_LEFT))
	{
		currentSong -= 2;
		if (currentSong < 0)
		{
			currentSong += playList.size();
		}
		pChannel->stop();
	}

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void SoundTutoriralApp::draw() {

	// wipe the screen to the background colour
	clearScreen();


	m_2dRenderer->setCameraPos(0, 0);
	m_2dRenderer->begin();

	m_2dRenderer->setRenderColour(1,1,1,1);
	m_2dRenderer->drawText(m_font, std::to_string(currentSong).c_str(), 100, 100);
	m_2dRenderer->drawCircle(640, 360, 0.5f + circle);
	//m_2dRenderer->drawSprite(PlayButton, 100.0f, 100.0f, 10, 10, 0, 0, 0, 0);

	m_2dRenderer->end();
}

