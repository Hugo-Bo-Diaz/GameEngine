#include "ComponentSprite.h"

#include "../GameObject.h"

#include "../../Engine.h"
#include "../../Graphics.h"
#include "../../Resources.h"

ComponentSprite::ComponentSprite()
{
	type = CT_SPRITE;
}

void ComponentSprite::SetImage(int ResourceId)
{
	TexID = ResourceId;

	Texture* t = (Texture*)Engine::Get().rs->GetResource(TexID);

	if (t != nullptr)
	{
		max_image_size_x = t->size_x;
		max_image_size_y = t->size_y;

		UV.CalculateCoords(max_image_size_x, max_image_size_y, true);
	}
	else
	{
		max_image_size_x = 0;
		max_image_size_y = 0;
	}
}

void ComponentSprite::SetShader(int ResourceId)
{
	ShdId = ResourceId;
}

void ComponentSprite::Draw()
{

	Engine::Get().gr->DrawSprite(parent->global_transform.matrix,UV.coords,TexID,ShdId);
}

void ComponentSprite::SaveToFile(rapidjson::Writer<rapidjson::StringBuffer>* writer)
{
	writer->Key("UV");
	writer->StartObject();

		
		writer->Key("x");
		writer->Uint(UV.x);

		writer->Key("y");
		writer->Uint(UV.y);

		writer->Key("w");
		writer->Uint(UV.w);

		writer->Key("h");
		writer->Uint(UV.h);


	writer->EndObject();

	writer->Key("TexID");
	writer->Int(TexID);

	writer->Key("ShdID");
	writer->Int(ShdId);
	//save UV
	//save tex and shader
	//save transform(not yet)
}

void ComponentSprite::LoadFromFile(const rapidjson::Value& val)
{
	const rapidjson::Value& UV_obj = val["UV"];

	UV.x = UV_obj["x"].GetUint();
	UV.y = UV_obj["y"].GetUint();
	UV.w = UV_obj["w"].GetUint();
	UV.h = UV_obj["h"].GetUint();

	SetImage(val["TexID"].GetInt());

	SetShader(val["ShdID"].GetInt());

}

void TexCoords::CalculateCoords(int max_w, int max_h, bool setdefault) {

	
	if(setdefault)
	{
		coords[0] = 0;		coords[1] = 0;
		coords[2] = 1;		coords[3] = 1;
	}
	else
	{
		coords[0] = x / max_w;			coords[1] = y / max_h;//down coord = y + h
		coords[2] = (x + w) / max_w;	coords[3] = (y + h) / max_h;//up coord = y
	}


};
