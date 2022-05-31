#ifndef COMP_SPRITE__H
#define COMP_SPRITE__H
#include "../GameObject.h"

struct TexCoords {

	TexCoords() {
		CalculateCoords(1, 1, true);
	};

	glm::vec4 coords;

	//image rectangle
	float x = 0, y = 0, w = 0, h = 0;

	int current_image_res;

	void CalculateCoords(int max_w, int max_h, bool def = false); 
};


class ComponentSprite : public Component
{
public:

	ComponentSprite();

	void SetImage(int ResourceId);
	void SetShader(int ResourceId);

	float max_image_size_x;
	float max_image_size_y;

	int TexID = -1;
	int ShdId = -1;

	void Draw();

	TexCoords UV;
	glm::mat4 transform = glm::mat4(1.0f);

	void SaveToFile(rapidjson::Writer<rapidjson::StringBuffer>*);
	void LoadFromFile(const rapidjson::Value& val);

};

#endif