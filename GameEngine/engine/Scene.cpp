#include "../Globals.h"

#include "../MessageManager.h"

#include "Scene.h"
#include "GameObject/GameObject.h"

#include <algorithm>

Scene::Scene()
{

	//GameObject* obj1 = new GameObject();
	//obj1->SetGlobalPosition({ 0,0,1 });
	////obj1->SetGlobalRotation(3.14 / 4);
	//obj1->SetGlobalScale({200,200});
	//obj1->name = "firstobj";
	//obj1->myid = 102;

	//obj1->AddComponent(CT_SPRITE);

	//GameObject* obj2 = new GameObject();
	//obj2->SetGlobalPosition({300,300,1 });
	////obj2->SetGlobalRotation(3.14/4);
	//obj2->SetGlobalScale({200,200});
	//obj2->name = "secondobj";
	//obj2->myid = 104;


	//objects.push_back(obj1);
	//objects.push_back(obj2);

	//updatesceneviewermsg* m1 = new updatesceneviewermsg();

	//m1->dest = EDITOR;
	//m1->objectid = obj1->myid;

	//MessageManager::Get().AddMessage(m1);

	//updatesceneviewermsg* m2 = new updatesceneviewermsg();

	//m2->dest = EDITOR;
	//m2->objectid = obj2->myid;

	//MessageManager::Get().AddMessage(m2);


	scenemsg* load = new scenemsg();

	load->dest = ENGINE;
	load->path = "i";
	load->code = LOAD_SCENE;
	MessageManager::Get().AddMessage(load);



}

bool Scene::Update(float dt)
{
	for (GameObject* g : objects)
	{
		g->Draw();
	}

	for (int del : to_delete)
	{
		GameObject* g = GetGameObject(del);
		if (g != nullptr)
		{
			objects.erase(std::remove(objects.begin(),objects.end(),g),objects.end());
			delete g;
		}
	}
	to_delete.clear();
	return true;
}

GameObject* Scene::GetGameObject(int ID)
{
	for (GameObject* g : objects)
	{
		if (g->myid == ID)
			return g;
	}

	return nullptr;
}

int Scene::AddGameObject(const char* name)
{
	GameObject* obj = new GameObject();
	obj->name = name;
	obj->myid = objectcount++;

	objects.push_back(obj);


	updatesceneviewermsg* m1 = new updatesceneviewermsg();

	m1->dest = EDITOR;
	m1->objectid = obj->myid;

	MessageManager::Get().AddMessage(m1);

	return obj->myid;

}

void Scene::RemoveGameObject(int ID)
{
	updatesceneviewermsg* m1 = new updatesceneviewermsg();

	m1->dest = EDITOR;
	m1->objectid = ID;

	MessageManager::Get().AddMessage(m1);

	to_delete.push_back(ID);
}

void Scene::ClearScene()
{
	for (GameObject* go : objects)
	{
		if (go != nullptr)
		{

			updatesceneviewermsg* m1 = new updatesceneviewermsg();

			m1->dest = EDITOR;
			m1->objectid = go->myid;

			MessageManager::Get().AddMessage(m1);


			delete go;
		}
	}
	objects.clear();

}

void Scene::SaveToScene(rapidjson::Writer<rapidjson::StringBuffer>* writer)
{
	writer->Key("objectcount");
	writer->Uint(objectcount);

	writer->Key("list");
	writer->StartArray();
	for (GameObject* go : objects)
	{
		writer->StartObject();
		go->SaveToScene(writer);
		writer->EndObject();
	}


	writer->EndArray();
}

void Scene::LoadFromScene(rapidjson::Value& val)
{
	ClearScene();

	objectcount = val["objectcount"].GetUint();

	rapidjson::Value& scnlist = val["list"];

	for (rapidjson::Value::ConstValueIterator itr = scnlist.Begin(); itr != scnlist.End(); ++itr)
	{
		const rapidjson::Value& object = *itr;

		GameObject* newobj = new GameObject();

		newobj->LoadFromFile(object);

		objects.push_back(newobj);

		//LOAD THE COMPONENTS



		//send updates
		updatesceneviewermsg* m1 = new updatesceneviewermsg();

		m1->dest = EDITOR;
		m1->objectid = newobj->myid;

		MessageManager::Get().AddMessage(m1);

	}
}

bool Scene::ShutDown()
{
	ClearScene();
	return true;
}
