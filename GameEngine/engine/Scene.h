#ifndef SCENE__H
#define SCENE__H

class GameObject;

#include <vector>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

//container and executer for objects
class Scene
{
public:

	Scene();

	bool Update(float dt);

	int objectcount = 0;
	std::vector<GameObject*> objects;
	GameObject* GetGameObject(int ID);
	
	int AddGameObject(const char* name);
	void RemoveGameObject(int ID);
	std::list<int> to_delete;

	void ClearScene();

	void SaveToScene(rapidjson::Writer<rapidjson::StringBuffer>*);
	void LoadFromScene(rapidjson::Value&);

	bool ShutDown();
};

#endif // !ENGINE__H
