#include "SoundTutoriralApp.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main() {
	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// allocation
	auto app = new SoundTutoriralApp();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}