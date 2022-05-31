#ifndef SPRITE_VIEWER__H
#define SPRITE_VIEWER__H

#include "ComponentViewer.h"
#include "../../UIclasses/UIComboBox.h"
#include <vector>

class ComponentSprite;
class SpriteShowcaseWindow;

class SpriteViewer : public ComponentViewer {
public:
	SpriteViewer(int y, int width, int height, ComponentSprite* tr, HWND parent);

	void ApplyComponent();
	void ChangeEditor(ComponentSprite* new_transform);

	void ChangeLabelText(HWND label, const char* text);
	float GetLabelfloat(HWND label);

	void CreateSpriteShowcaseWindow();

	//position

	UIComboBox cb_texture;
	UIComboBox cb_shader;
	
	HWND edit_x;
	HWND edit_y;
	HWND edit_w;
	HWND edit_h;

	ComponentSprite* to_transform;

	std::vector<int> textures_index;
	std::vector<int> shaders_index;

	static LRESULT CALLBACK SpriteViewerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);

	SpriteShowcaseWindow* showcase = nullptr;

};

static void CallBackShowcaseWindow(int, int, int, int);

#endif // !TRANSFORM_VIEWER__H