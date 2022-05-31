#include "../Globals.h"

#include "Resources.h"
#include <glad/glad.h>

#include "ResLoaders/TexLoader.h"
#include "ResLoaders/ShaderLoader.h"

#include "Engine.h"
#include "Graphics.h"

#include "../MessageManager.h"

Resources::Resources()
{

	//BaseShader = GetShader(LoadResource(RT_SHADER, "default.glsl"));

}

int Resources::GetTexture(int ID)
{
	Resource* res = GetResource(ID);

	if (res == nullptr)
		return -1;

	if (res->type == ResType::RT_TEXTURE)
		return ((Texture*)res)->bufferID;
	else
		return -1;
}

int Resources::GetShader(int ID)
{
	Resource* res = GetResource(ID);

	if (res == nullptr)
		return -1;

	if (res->type == ResType::RT_SHADER)
		return ((Shader*)res)->bufferID;
	else
		return -1;
}

int Resources::GetScript(int ID)
{
	return 0;
}

const char* Resources::GetSound(int ID)
{
	return nullptr;
}

std::vector<int> Resources::GetAllResourcesWithType(ResType t)
{
	std::vector<int> to_fill;
	for (Resource* r : resource_list)
	{
		if (r->type == t)
			to_fill.push_back(r->ID);
	}

	return to_fill;
}

bool Resources::FreeResources()
{
	for (Resource* rs : resource_list)
	{
		rs->FreeRes();
		delete rs;
	}

	resource_list.clear();

	return true;
}

void Resources::SaveToScene(rapidjson::Writer<rapidjson::StringBuffer>* writer)
{
	//ALL ENCOMP

	writer->Key("rescount");
	writer->Uint(rescount);

	writer->Key("list");
	writer->StartArray();
	for (Resource* res : resource_list)
	{
		writer->StartObject();

		writer->Key("ID");
		writer->Uint(res->ID);

		writer->Key("path");
		writer->String(res->path.c_str());

		writer->Key("name");
		writer->String(res->name.c_str());

		writer->Key("type");
		writer->Uint(res->type);

		writer->EndObject();
	}
	writer->EndArray();

	

}

void Resources::LoadFromScene(rapidjson::Value& val)
{
	FreeResources();

	rescount = 0;

	rapidjson::Value& reslist = val["list"];

	Engine::Get().gr->LoadBase();

	for (rapidjson::Value::ConstValueIterator itr = reslist.Begin(); itr != reslist.End(); ++itr)
	{
		const rapidjson::Value& resource = *itr;

		std::string path = resource["path"].GetString();
		int type = resource["type"].GetUint();

		std::string temp1;
		std::string temp2;
		int temp;

		switch (type)
		{
		case RT_TEXTURE:
			temp = LoadTexture(path.c_str());
			//GetResource(temp)->name = resource["name"].GetString();
			//GetResource(temp)->ID = resource["ID"].GetUint();
			break;
		case RT_SOUND:
			break;
		case RT_SCRIPT:
			break;
		case RT_SHADER:
			//temp = LoadShader(path.c_str());
			//GetResource(temp)->name = resource["name"].GetString();
			//GetResource(temp)->ID = resource["ID"].GetUint();
			break;
		default:
			break;
		}
		//MessageBox(NULL, std::to_string(id).c_str(), std::to_string(type).c_str(), MB_OK);
	}

	//Engine::Get().gr->base_shader = ((Shader*)GetResource(Engine::Get().gr->base_shader_name.c_str()))->ID;
	//Engine::Get().gr->base_texture = ((Texture*)GetResource(Engine::Get().gr->base_texture_name.c_str()))->ID;


	//rescount = val["rescount"].GetUint();

}

Resource* Resources::GetResource(int ID)
{
	if (resource_list.empty())
		return nullptr;

	for (Resource* rs : resource_list)
	{
		if (rs->ID == ID)
		{
			return rs;
		}
	}

	return nullptr;
}

Resource* Resources::GetResource(const char* res)
{
	if (resource_list.empty())
		return nullptr;

	for (Resource* rs : resource_list)
	{
		if (rs->path == res)
		{
			return rs;
		}
	}

	return nullptr;
}

const char* Resources::GetFileNameFromPath(const char* path)
{
	std::string p = path;
	return p.substr(p.find_last_of("/\\") + 1).c_str();
}

int Resources::LoadTexture(const char* path)
{
	//check if loaded
	Resource* attempt = GetResource(path);
	if (attempt != nullptr)
		return attempt->ID;

	//loaded if its not
	Texture* tex = TextureLoader::LoadTexture(path);
	if (tex != nullptr)
	{
		tex->name = GetFileNameFromPath(path);
		tex->type = RT_TEXTURE;
		//texture was loaded successfully
		resource_list.push_back(tex);
		tex->ID = rescount++;
		return tex->ID;
	}
	else
	{
		return -1;
	}
}

int Resources::LoadShader(const char* shader)
{
	//std::string t = vertex; t += "*"; t += fragment;

	Resource* attempt = GetResource(shader);
	if (attempt != nullptr)
		return attempt->ID;

	Shader* sha = ShaderLoader::LoadShader(shader);
	if (sha != nullptr)
	{
		sha->name = GetFileNameFromPath(shader);
		sha->type = RT_SHADER;
		sha->ID = rescount++;
		sha->path = shader;
		resource_list.push_back(sha);
		return sha->ID;
	}
	else
	{
		return -1;
	}
}

int Resources::LoadResource(const char* path)
{
	std::string pstr = path;

	//default return value
	int ret = -1;

	if (pstr.find_last_of(".") != std::string::npos) {
		std::string ext = pstr.substr(pstr.find_last_of(".") + 1);

		if (ext == "png")
		{
			ret = LoadTexture(path);
		}
		else if (ext == "glsl")
		{
			ret = LoadShader(path);
		}
		else if (ext == "other")
		{
			//LoadOther
		}

	}

	if (ret != -1)
	{
		updateresourceviewermsg* m1 = new updateresourceviewermsg();

		m1->dest = EDITOR;
		m1->id = ret;

		MessageManager::Get().AddMessage(m1);
	}
	
	return ret;
}

void Texture::FreeRes()
{
	glDeleteTextures(1, &bufferID);
}

void Shader::FreeRes()
{
	glDeleteProgram(bufferID);
}
