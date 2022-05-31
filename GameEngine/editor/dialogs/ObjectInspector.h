#ifndef OBJECT_INSPECTOR__H
#define OBJECT_INSPECTOR__H

#include "../Dialog.h"

#include <vector>
#include "../UIclasses/UIComboBox.h"

class Component;

LRESULT CALLBACK ObjectInspectorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);


class ComponentViewer;
class TransformViewer;

class GameObject;
class Transform;

class ObjectInspector : public Dialog
{
public:

	int margin_x = 20;

	ObjectInspector(LPCWSTR dialog_name, int min_size = 250, WindowPosition pos = LEFT, HWND editor_handler = NULL);
	GameObject* to_inspect = nullptr;

	void SetGameObject(int id);

	void AddTransformUI(Transform*);
	int AddComponentUI(Component*,int current_height);
	void ComponentChanged(HWND handler);

	std::vector<ComponentViewer*> components;

	void HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void OnResize(int newx, int newy);


	HWND AddComponentButton;
	UIComboBox ComponentTypeList;



};




#endif // !SCENE_VIEWER__H

