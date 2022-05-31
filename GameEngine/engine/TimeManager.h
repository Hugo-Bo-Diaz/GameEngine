#ifndef TIME_MANAGER__H
#define TIME_MANAGER__H

#include <chrono>

enum FPS {
	VSYNC = 0,
	FPS30 = 30,
	FPS60 = 60,
	FPS90 = 90,
};


class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> started_at;
	double time_paused;
	std::chrono::time_point<std::chrono::steady_clock> paused_at;

public:
	void Start();

	float GetTimeMS();
	float GetTimeS();
	void Reset();

	void Pause();
	void Resume();
	
	bool paused = false;
};


class TimeManager
{
public:

	TimeManager();
	bool Initialize(FPS);
	void Update();
	bool ShutDown();

	void StartFrameTime();

	void EndFrameTime();

	Timer FrameTimer;

	Timer DeltaTimer;

	void SetFPS(FPS);
	FPS frames_per_second;
	float GetDeltaTime() { return deltatime; };
private:
	float deltatime;

};
#endif