#include "SpriteViewer.h"
#include "../../../Globals.h"
#include "../../../engine/GameObject/GameObject.h"
#include "../../../engine/GameObject/Components/ComponentSprite.h"
#include "../../Engine.h"
#include "../../Resources.h"
#include "../../Graphics.h"
#include "../../EditorWindow.h"
#include "../ObjectInspector.h"

#include "../../ExtraWindows/SpriteShowcaseWindow.h"

#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

SpriteViewer::SpriteViewer(int y, int width, int height, ComponentSprite* tr, HWND Handler)
{
	handler = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, y, width, height, Handler, NULL, NULL, NULL);
	SetWindowSubclass(handler, SpriteViewerProc, 0, (DWORD_PTR)this);

	title = CreateWindowW(L"static", L"Sprite", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, handler, NULL, NULL, NULL);

	to_transform = tr;

	cb_texture.Initialize(20, 40, width - 40, 20, handler);
	cb_shader.Initialize(20, 70, width - 40, 20, handler);

	std::vector<int> tex = Engine::Get().rs->GetAllResourcesWithType(RT_TEXTURE);
	for (int i = 0; i<tex.size();++i)
	{
		Resource* r =  Engine::Get().rs->GetResource(tex[i]);
		if (r != nullptr)
		{
			cb_texture.AddItem(r->name.c_str());
			textures_index.push_back(r->ID);
			if (r->ID == tr->TexID)
				cb_texture.SetCurrentItem(i);
		}
	}
	cb_texture.AddItem("default");
	textures_index.push_back(-1);
	if (tr->TexID == -1)
		cb_shader.SetCurrentItem(tex.size());

	tex.clear();


	std::vector<int> shd = Engine::Get().rs->GetAllResourcesWithType(RT_SHADER);
	for (int i = 0; i < shd.size(); ++i)
	{
		Resource* r = Engine::Get().rs->GetResource(shd[i]);
		if (r != nullptr)
		{
			cb_shader.AddItem(r->name.c_str());
			shaders_index.push_back(r->ID);
			if (r->ID == tr->ShdId)
				cb_shader.SetCurrentItem(i);
		}
	}
	cb_shader.AddItem("default");
	shaders_index.push_back(-1);
	if (tr->ShdId == -1)
		cb_shader.SetCurrentItem(shd.size());

	shd.clear();

	std::wstring num_text;
	std::wstring rounded;

	num_text = std::to_wstring(tr->UV.x);
	rounded = num_text.substr(0, num_text.find(L"."));
	CreateWindowW(L"static", L"x", WS_CHILD | WS_CHILD, 20, 100, 20, 17, handler, NULL, NULL, NULL);
	edit_x = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, 40, 100, 50, 17, handler, NULL, NULL, NULL);

	num_text = std::to_wstring(tr->UV.y);
	rounded = num_text.substr(0, num_text.find(L"."));
	CreateWindowW(L"static", L"y", WS_CHILD | WS_CHILD, width-20-70, 100, 20, 17, handler, NULL, NULL, NULL);
	edit_y = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, width-20-50, 100, 50, 17, handler, NULL, NULL, NULL);

	num_text = std::to_wstring(tr->UV.w);
	rounded = num_text.substr(0, num_text.find(L"."));
	CreateWindowW(L"static", L"w", WS_CHILD | WS_CHILD, 20, 130, 20, 17, handler, NULL, NULL, NULL);
	edit_w = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, 40, 130, 50, 17, handler, NULL, NULL, NULL);

	num_text = std::to_wstring(tr->UV.h);
	rounded = num_text.substr(0, num_text.find(L"."));
	CreateWindowW(L"static", L"h", WS_CHILD | WS_CHILD, width-20-70, 130, 20, 17, handler, NULL, NULL, NULL);
	edit_h = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, width-20-50, 130, 50, 17, handler, NULL, NULL, NULL);

	CreateWindowW(L"button", L"Change Coords", WS_VISIBLE | WS_CHILD, 10 , 150, width-20, 30, handler, (HMENU)BUT_OPEN_SPRITE, NULL, NULL);
}

void SpriteViewer::ApplyComponent()
{
	int sel_tex = cb_texture.GetSelectedItem();
	if (sel_tex < 0)
		sel_tex = -1;

	int sel_sha = cb_shader.GetSelectedItem();
	if (sel_sha < 0)
		sel_sha = -1;

	to_transform->SetImage(textures_index[sel_tex]);
	to_transform->SetShader(shaders_index[sel_sha]);

	to_transform->UV.x = GetLabelfloat(edit_x);
	to_transform->UV.y = GetLabelfloat(edit_y);
	to_transform->UV.w = GetLabelfloat(edit_w);
	to_transform->UV.h = GetLabelfloat(edit_h);

	to_transform->UV.CalculateCoords(to_transform->max_image_size_x, to_transform->max_image_size_y, false);
}

void SpriteViewer::ChangeEditor(ComponentSprite* new_transform)
{


}

void SpriteViewer::ChangeLabelText(HWND label, const char* text)
{
}

float SpriteViewer::GetLabelfloat(HWND label)
{
	char buffer[20];//20 figure number
	GetWindowText(label, buffer, 20);
	std::string ret = buffer;

	if (ret.length() == 0)
		return -1;

	return std::stof(ret);
}

void SpriteViewer::CreateSpriteShowcaseWindow()
{

	if (showcase == nullptr)
	{
		showcase = new SpriteShowcaseWindow();

		int sel_tex = cb_texture.GetSelectedItem();
		if (sel_tex < 0)
			sel_tex = -1;

		int resID = textures_index[sel_tex];
		
		Texture*t = (Texture*)Engine::Get().rs->GetResource(resID);

		showcase->Initialize(t->path.c_str(), L"L", handler, to_transform->UV.x, to_transform->UV.y, to_transform->UV.w, to_transform->UV.h);
	}

}



LRESULT SpriteViewer::SpriteViewerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR pointer)
{
	SpriteViewer* s = ((SpriteViewer*)pointer);

	switch (msg)
	{
	case WM_COMMAND:

		switch (wParam)
		{
		case BUT_OPEN_SPRITE:
			s->CreateSpriteShowcaseWindow();
			break;
		case SPR_OKEY:
			SetWindowText(s->edit_x, std::to_string(s->showcase->x).c_str());
			SetWindowText(s->edit_y, std::to_string(s->showcase->y).c_str());
			SetWindowText(s->edit_w, std::to_string(s->showcase->w).c_str());
			SetWindowText(s->edit_h, std::to_string(s->showcase->h).c_str());
			s->ApplyComponent();
			s->showcase = nullptr;
			break;

		case SPR_CANCEL:
			s->showcase = nullptr;
			break;

		case SPR_APPLY:
			SetWindowText(s->edit_x, std::to_string(s->showcase->x).c_str());
			SetWindowText(s->edit_y, std::to_string(s->showcase->y).c_str());
			SetWindowText(s->edit_w, std::to_string(s->showcase->w).c_str());
			SetWindowText(s->edit_h, std::to_string(s->showcase->h).c_str());
			s->ApplyComponent();
			break;

		default:
			break;
		}

		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			EditorWindow::Get().GetObjectInspector()->ComponentChanged(hwnd);
			break;
		case CBN_SELCHANGE:
			EditorWindow::Get().GetObjectInspector()->ComponentChanged(hwnd);
		default:
			break;
		}
		break;
	default:
		break;
	}


	return DefSubclassProc(hwnd, msg, wParam, lParam);
}

void CallBackShowcaseWindow(int, int, int, int)
{
	

}
