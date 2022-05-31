#ifndef GAMEOBJECT__H
#define GAMEOBJECT__H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <rpc.h>

#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/document.h"

enum CompType {
	CT_SPRITE,
	CT_SOUND_SOURCE,
	CT_SCRIPT,
	CT_MAX_TYPES,
};

class GameObject;

class Component {
public:
	CompType type;
	GameObject* parent;

	virtual void SaveToFile(rapidjson::Writer<rapidjson::StringBuffer>*) = 0;
	virtual void LoadFromFile(const rapidjson::Value& val) = 0;
};

struct Transform {
	glm::vec3 pos = {0,0,0};
	float rotation = 0;
	glm::vec2 scale = {1,1};

	glm::mat4 matrix = glm::mat4(1.0f);

	void CalculateMatrix(Transform* parent = nullptr) {
		
		//reset matrix
		matrix = glm::mat4(1.0f);

		//add components
		matrix = glm::translate(matrix, pos);
		matrix = glm::rotate(matrix, rotation, { 0,0,1 });
		matrix = glm::scale(matrix, glm::vec3(scale,1.0f));
		
		/*if (parent != nullptr)
		{
			matrix = matrix* parent->matrix;
		}*/
	};
};

class GameObject 
{
public:
	GameObject();
	~GameObject();
	//TRANSFORM
	//Transform local_transform;
	Transform global_transform;

	void SetGlobalPosition(glm::vec3);
	void SetGlobalRotation(float);
	void SetGlobalScale(glm::vec2);


	void SaveToScene(rapidjson::Writer<rapidjson::StringBuffer>*);
	void LoadFromFile(const rapidjson::Value& val);

	//IDENTIFIERS
	//unsigned int UUID;
	int myid;
	//static int nextid;
	std::string name;

	//deleteLater (SPRITE COMPONENT)
	void Draw();

	std::vector<Component*> components;

	//GameObject* parent;
	//std::vector<GameObject*>children;

	Component* AddComponent(CompType);
	Component* getComponent(CompType);

};

#endif // !ENGINE__H
