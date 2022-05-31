#include "../Globals.h"

#include "Engine.h"

#include <glm/glm.hpp>

#include <fstream>
#include <sstream>

#include "Graphics.h"
#include "Scene.h"
#include "Resources.h"
#include "Input.h"
#include "TimeManager.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <iostream>

#if EDIT_MODE 1
#include "../editor/EditorWindow.h"
#include "../MessageManager.h"

#endif // EDIT_MODE 1


Engine* eng = nullptr;

Engine& Engine::Get()
{
	if (eng == nullptr)
		eng = new Engine();

	return *eng;
}



void Engine::InitializeSubSystems()
{
	gr = new Graphics();

	rs = new Resources();
	sc = new Scene();

	in = new Input();
	tm = new TimeManager();
}

bool Engine::InitializeWin32(HWND parwindow)
{

	if (!glfwInit())
	{
		return false;
	}

	bool result = true;

	if(result)
		result = gr->InitWin32(parwindow);

#if EDIT_MODE 1
	EditorWindow::Get().SetEngineView(gr->Whandler);
	#endif // EDIT_MODE 1

	if(result)
		result = in->Initialize(gr->window);

	if (result)
		result = tm->Initialize(FPS60);


	return result;
}


void Engine::Loop()
{
	float dt = tm->GetDeltaTime();

	tm->StartFrameTime();

	//my logic
	sc->Update(dt);
	gr->Render();
	//editor
	RecieveEditorWindowMessages();

	tm->EndFrameTime();

	//wait for the reminder of the frame
	tm->Update();

}

void Engine::ShutDown()
{
	sc->ClearScene();
	rs->FreeResources();
	gr->ShutDown();
	in->ShutDown();
	delete gr;
}

void Engine::SetEngineState(EngineState st)
{
	EStateMX.lock();
	state = st;
	EStateMX.unlock();
}

void Engine::RecieveEditorWindowMessages()
{
	std::list<msg*> message = MessageManager::Get().GetMessages(ENGINE);

	for (msg* m : message)
	{
		switch (m->code)
		{
		case RESIZE_ENGINE_WINDOW:

			gr->MoveEngineWindow(((resizemsg*)m)->x, ((resizemsg*)m)->y, ((resizemsg*)m)->w, ((resizemsg*)m)->h);
			break;

		case SAVE_SCENE:
			SaveScene(((scenemsg*)m)->path.c_str());
			break;

		case LOAD_SCENE:
			LoadScene(((scenemsg*)m)->path.c_str());
			break;

		case CREATE_GAME_OBJECT:
			sc->AddGameObject(((addobjmsg*)m)->name.c_str());
			break;

		case DELETE_GAME_OBJECT:
			sc->RemoveGameObject(((deleteobjmsg*)m)->to_delete);
			break;

		case UPDATE_RESOURCES:
			rs->LoadResource(((updateresourcesmsg*)m)->fullpath.c_str());
			break;

		case PLAY_ENGINE:
			PlayGame();
			break;

		case STOP_ENGINE:
			StopGame();
			break;

		case PAUSE_ENGINE:
			PauseGame();
			break;

		default:
			break;
		}
	}
	MessageManager::DeleteMessagesFromList(&message);

}




void Engine::SaveScene(const char* filename)
{

	rapidjson::Document document;

	//avoiding verbosity
	using rapidjson::StringBuffer;
	using rapidjson::Writer;

	StringBuffer buff;
	Writer<StringBuffer> writer(buff);

	writer.StartObject();


	writer.Key("Resources");
	writer.StartObject();
	rs->SaveToScene(&writer);
	writer.EndObject();

	writer.Key("Objects");
	writer.StartObject();
	sc->SaveToScene(&writer);
	writer.EndObject();


	writer.EndObject();

	std::ofstream out(filename);
	out << buff.GetString();
	out.close();


	//sc->SaveToScene();
}

void Engine::LoadScene(const char* filename)
{

	rapidjson::Document document;

	std::string jsonfile;
	std::ifstream jsonifstream;
	std::stringstream jsonststream;

	jsonifstream.open(filename);
	jsonststream << jsonifstream.rdbuf();
	jsonifstream.close();

	jsonfile = jsonststream.str();

	document.Parse(jsonfile.c_str());

	rs->LoadFromScene(document["Resources"]);
	sc->LoadFromScene(document["Objects"]);

	//rs->LoadFromScene();
	//sc->LoadFromScene();
}

void Engine::PlayGame()
{
	//save scene
	plState = PL_PLAY;

	//avoiding verbosity
	using rapidjson::StringBuffer;
	using rapidjson::Writer;

	StringBuffer buff;
	Writer<StringBuffer> writer(buff);

	writer.StartObject();
	sc->SaveToScene(&writer);
	writer.EndObject();

	std::stringstream engine_stream;
	engine_stream << buff.GetString();

	engine_state = engine_stream.str();
}

void Engine::PauseGame()
{
	//set dt to 0
	if (plState == PL_PLAY)
	{
		//pause game
		plState = PL_PAUSE;
	}
	else
	{
		//resume game
		plState = PL_PLAY;
	}

	msg* m = new msg();
	m->code = PAUSED_BUTTON;
	m->dest = EDITOR;
	MessageManager::Get().AddMessage(m);

}

void Engine::StopGame()
{
	//load scene
	//set dt to 0
	plState = PL_STOP;

	rapidjson::Document document;
	document.Parse(engine_state.c_str());

	sc->LoadFromScene(document);


	msg* m1 = new msg();
	m1->code = PLAYED_BUTTON;
	m1->dest = EDITOR;
	MessageManager::Get().AddMessage(m1);

	msg* m = new msg();
	m->code = STOPPED_BUTTON;
	m->dest = EDITOR;
	MessageManager::Get().AddMessage(m);
}

