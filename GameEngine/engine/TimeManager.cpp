#include "TimeManager.h"
#include "Engine.h"
#include "Graphics.h"


TimeManager::TimeManager()
{
}

bool TimeManager::Initialize(FPS fps)
{
	SetFPS(fps);
	return true;
}

void TimeManager::Update()
{
	float total_frame_time_ms;
	float time_left;
	switch (frames_per_second)
	{
	case FPS30:
		total_frame_time_ms = 1000 / 30;
		break;
	case FPS60:
		total_frame_time_ms = 1000 / 60;
		break;
	case FPS90:
		total_frame_time_ms = 1000 / 90;
		break;
	default:
		break;
	}

	time_left = total_frame_time_ms - FrameTimer.GetTimeMS();

	Sleep(time_left);

}

bool TimeManager::ShutDown()
{
	return false;
}

void TimeManager::StartFrameTime()
{
	FrameTimer.Reset();
	DeltaTimer.Reset();
}

void TimeManager::EndFrameTime()
{
	//FrameTimer.Reset();
}

void TimeManager::SetFPS(FPS fps)
{
	frames_per_second = fps;

	if (frames_per_second == VSYNC)
		Engine::Get().gr->SetVsync(true);
	else
		Engine::Get().gr->SetVsync(false);
}




void Timer::Start()
{
	started_at = std::chrono::steady_clock::now();
	paused = false;
	time_paused = 0;
}

float Timer::GetTimeMS()
{
	std::chrono::time_point<std::chrono::steady_clock> end;

	if (paused)
		end = paused_at;
	else
		end = std::chrono::steady_clock::now();

	std::chrono::duration_cast<std::chrono::milliseconds>(end - started_at).count() - time_paused;
	return 0.0f;
}

float Timer::GetTimeS()
{
	return 0.0f;
}

void Timer::Reset()
{
	paused = false;
	time_paused = 0;
	started_at = std::chrono::steady_clock::now();
}

void Timer::Pause()
{
	if (!paused)
	{
		paused = true;
		paused_at = std::chrono::steady_clock::now();
	}
}

void Timer::Resume()
{
	if (paused)
	{
		paused = false;
		time_paused += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - paused_at).count();
	}
}
