#include "../Globals.h"
#include "Engine.h"
#include "EngineThread.h"


int EngineMainWin32(HWND window)
{
	Engine::Get().state = START;
	Engine::Get().InitializeSubSystems();
	bool success;
	success = Engine::Get().InitializeWin32(window);
	if (success)
	{
		Engine::Get().state = LOOP;
	}
	else
	{
		Engine::Get().state = EXIT;
	}

	while (Engine::Get().state != EXIT)
	{
		switch (Engine::Get().state)
		{
		case LOOP:
			Engine::Get().Loop();
			break;
		case STOP:
			Engine::Get().state = EXIT;
			break;
		default:
			break;
		}

	}

	Engine::Get().ShutDown();

	return 0;
}
