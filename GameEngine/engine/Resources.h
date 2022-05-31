#ifndef RESOURCES__H
#define RESOURCES__H

#ifdef _WIN32
#include<Windows.h>
#endif

#include <vector>
#include <string>
#include <list>
#include "stb/stb_image.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"


enum ResType {
	RT_NOT_ASSIGNED,

	RT_TEXTURE,
	RT_SHADER,
	
	RT_SOUND,
	
	RT_SCRIPT,


	RT_NONE
};


struct Resource
{
	unsigned int ID;
	ResType type;
	std::string path;
	std::string name;

	virtual void FreeRes() {};
};

struct Texture : Resource {

	unsigned int bufferID;
	unsigned int size_x;
	unsigned int size_y;
	unsigned int stride;
	unsigned int nchannels;


	void FreeRes() override;

};

struct Shader : Resource {

	unsigned int bufferID;

	void FreeRes() override;

};

struct Script : Resource {
	std::string script;
	std::list<std::string> functions;
};

//container for resources
class Resources 
{
public:

	Resources();

	int rescount = 0;
	int LoadTexture(const char* path);
	int LoadShader(const char* shader);
	int LoadResource(const char* path);


	int GetTexture(int ID);
	int GetShader(int ID);
	int GetScript(int ID);
	const char* GetSound(int ID);

	std::vector<int> GetAllResourcesWithType(ResType);

	int BaseShader;
	
	bool FreeResources();
	void SaveToScene(rapidjson::Writer<rapidjson::StringBuffer>*);
	void LoadFromScene(rapidjson::Value&);
	Resource* GetResource(int ID);

private:
	Resource* GetResource(const char* res);
	std::vector<Resource*> resource_list;

	const char* GetFileNameFromPath(const char*);

	
};

#endif // !ENGINE__H
