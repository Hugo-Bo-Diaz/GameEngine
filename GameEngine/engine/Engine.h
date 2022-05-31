#ifndef ENGINE__H
#define ENGINE__H

enum EngineState
{
	START,
	LOOP,
	STOP,
	EXIT
};

enum PlayState
{
	PL_PLAY,
	PL_STOP,
	PL_PAUSE
};

#ifdef _WIN32
#include<Windows.h>
#endif

#include <mutex>
#include <condition_variable>

class Graphics;
class Resources;
class Scene;
class Input;
class TimeManager;

class Engine 
{
private:
	Engine() {};

public:
	#ifdef _WIN32
	bool InitializeWin32(HWND window);
	#endif // _WIN32

	void InitializeSubSystems();

	void Loop();

	void ShutDown();
	std::mutex EStateMX;
	EngineState state = START;

	PlayState plState = PL_STOP;
	std::string engine_state = "";

	void SetEngineState(EngineState st);

	Graphics* gr;
	Resources* rs;
	Scene* sc;
	Input* in;
	TimeManager* tm;

	void SaveScene(const char*);
	void LoadScene(const char*);

	void PlayGame();
	void PauseGame();
	void StopGame();

	static Engine& Get();

	void RecieveEditorWindowMessages();

};

#endif // !ENGINE__H
