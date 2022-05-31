#include "GameObject.h"
#include "../Graphics.h"
#include "../Engine.h"
#include "../Resources.h"
#include "Components/ComponentSprite.h"

GameObject::GameObject()
{
	//TexResID = Engine::Get().rs->LoadTexture("test.png");

	//myid = GameObject::nextid++;

	SetGlobalPosition({ 0,0,0 });
	SetGlobalRotation(0);
	SetGlobalScale({50,50});

}

GameObject::~GameObject()
{
	for (Component* c : components)
	{
		delete c;
	}
	components.clear();
}

void GameObject::SetGlobalPosition(glm::vec3 newpos)
{
	//move my local and global matrix
	//glm::vec3 diff = newpos - global_transform.pos;
	//
	//local_transform.pos -= diff;
	//local_transform.CalculateMatrix();

	global_transform.pos = newpos;
	global_transform.CalculateMatrix();

	////change my children transforms
	//for (GameObject* go : children)
	//{
	//	go->SetGlobalPosition(go->global_transform.pos - diff);
	//}
}

void GameObject::SetGlobalRotation(float newrot)
{
	global_transform.rotation = newrot;
	global_transform.CalculateMatrix();
}

void GameObject::SetGlobalScale(glm::vec2 scl)
{
	global_transform.scale = scl;
	global_transform.CalculateMatrix();
}

void GameObject::SaveToScene(rapidjson::Writer<rapidjson::StringBuffer>* writer)
{
	writer->Key("name");
	writer->String(name.c_str());

	writer->Key("ID");
	writer->Uint(myid);

	writer->Key("transform");
	writer->StartObject();


		writer->Key("position");
		writer->StartObject();

			writer->Key("x");
			writer->Double(global_transform.pos.x);

			writer->Key("y");
			writer->Double(global_transform.pos.y);

			writer->Key("z");
			writer->Double(global_transform.pos.z);

		writer->EndObject();


		writer->Key("rotation");
		writer->StartObject();

			writer->Key("val");
			writer->Double(global_transform.rotation);

		writer->EndObject();


		writer->Key("scale");
		writer->StartObject();

			writer->Key("x");
			writer->Double(global_transform.scale.x);

			writer->Key("y");
			writer->Double(global_transform.scale.y);

		writer->EndObject();

	writer->EndObject();


	writer->Key("components");
	writer->StartArray();

	for (Component* c : components)
	{
		writer->StartObject();
		writer->Key("type");
		writer->Uint(c->type);
		
		writer->Key("data");
		writer->StartObject();
			c->SaveToFile(writer);
		writer->EndObject();

		writer->EndObject();
	}

	writer->EndArray();


}

void GameObject::LoadFromFile(const rapidjson::Value& object)
{

	name = object["name"].GetString();
	myid = object["ID"].GetUint();
	const rapidjson::Value& transform = object["transform"];

	const rapidjson::Value& pos = transform["position"];
	float x = pos["x"].GetDouble();
	float y = pos["y"].GetDouble();
	float z = pos["z"].GetDouble();

	const rapidjson::Value& rot = transform["rotation"];
	float r = rot["val"].GetDouble();

	const rapidjson::Value& scl = transform["scale"];
	float sx = scl["x"].GetDouble();
	float sy = scl["y"].GetDouble();
	SetGlobalPosition({ x, y, z });
	SetGlobalRotation(r);
	SetGlobalScale({ sx,sy });

	const rapidjson::Value& comproot = object["components"];

	for (int i = 0; i < comproot.Size(); i++)
	{
		const rapidjson::Value& comp = comproot[i];

		CompType t = (CompType)comp["type"].GetUint();

		Component* c = AddComponent(t);
		if (c != nullptr)
			c->LoadFromFile(comp["data"]);
	}

}

void GameObject::Draw()
{
	//Engine::Get().gr->DrawSprite(global_transform.matrix, target_in_rect,TexResID);

	for (Component*c : components)
	{
		if (c->type == CT_SPRITE)
		{
			ComponentSprite* sc = (ComponentSprite*)c;
			sc->Draw();
		}
	}

}

Component* GameObject::AddComponent(CompType type)
{
	Component* c = nullptr;

	switch (type)
	{
	case CT_SPRITE:
		c = new ComponentSprite();
		break;
	case CT_SOUND_SOURCE:
		break;
	case CT_SCRIPT:
		break;
	default:
		break;
	}

	if (c != nullptr)
	{
		c->parent = this;
		components.push_back(c);
	}

	return c;
}

Component* GameObject::getComponent(CompType type)
{
	for (Component* com : components)
	{
		if (com->type == type)
		{
			return com;
		}
	}

	return nullptr;
}
