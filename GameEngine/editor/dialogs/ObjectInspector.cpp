#include "ObjectInspector.h"
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

#include "../../Globals.h"

#include "../EditorWindow.h"

#include "../../engine/Engine.h"
#include "../../engine/Scene.h"
#include "../../engine/GameObject/GameObject.h"


#include "ComponentViewers/ComponentViewer.h"
#include "ComponentViewers/TransformViewer.h"
#include "ComponentViewers/SpriteViewer.h"

ObjectInspector::ObjectInspector(LPCWSTR dialog_name, int width, WindowPosition _pos, HWND editor_handler)
{
	type = OBJECT_INSPECTOR;
	pos = _pos;
	min_size = width;
	Handler = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 100, 100, editor_handler, NULL, NULL, NULL);
	SetWindowSubclass(Handler, ObjectInspectorProc, 0, (DWORD_PTR)this);

	//add title
	CreateWindowW(L"static", dialog_name, WS_VISIBLE | WS_CHILD, 10, 10, 150, 20, Handler, NULL,NULL, NULL);

}

void ObjectInspector::SetGameObject(int id)
{
	to_inspect = Engine::Get().sc->GetGameObject(id);

	ComponentTypeList.ClearItems();
	DestroyWindow(ComponentTypeList.ComboBox);
	DestroyWindow(AddComponentButton);

	for (ComponentViewer* hwnd : components)
	{
		ShowWindow(hwnd->handler,SW_HIDE);
		EndDialog(hwnd->handler, 0);
		DestroyWindow(hwnd->handler);
		delete hwnd;
	}
	components.clear();

	if (to_inspect == nullptr)
		return;

	AddTransformUI(&to_inspect->global_transform);

	int cheight = 180;
	for (Component* c : to_inspect->components)
	{
		cheight = AddComponentUI(c,cheight);
	}

	ComponentTypeList.Initialize(10,cheight+10,(width/2)-20,30,Handler);

	RedrawWindow(Handler,NULL,NULL, RDW_INTERNALPAINT);

	AddComponentButton = CreateWindowW(L"button", L"Add", WS_VISIBLE | WS_CHILD, 10 + (width / 2), cheight + 10, (width / 2) - 20, 30, Handler, (HMENU)BUT_ADD_COMPONENT, NULL, NULL);

	for (size_t i = 0; i < CT_MAX_TYPES; i++)
	{
		switch (i)
		{	
		case CT_SPRITE:
			ComponentTypeList.AddItem("SPRITE");
			break;
		case CT_SCRIPT:
			ComponentTypeList.AddItem("SCRIPT");
			break;
		case CT_SOUND_SOURCE:
			ComponentTypeList.AddItem("SOUND_SOURCE");
			break;
		defalt:
			break;
		}
	}
}

void ObjectInspector::AddTransformUI(Transform* tr)
{

	TransformViewer* tv = new TransformViewer(30, width, 150, &to_inspect->global_transform,Handler);
	components.push_back(tv);
}

void ObjectInspector::ComponentChanged(HWND handler)
{
	for (ComponentViewer* c : components)
	{
		if (c->handler == handler)
		{
			c->ApplyComponent();
		}
	}

}

int ObjectInspector::AddComponentUI(Component* comp, int current_height)
{
	//HWND Cwind = CreateWindowW();
	//components.pushback(Cwind);
	int height_added = 100;
	ComponentViewer* n = nullptr;
	switch (comp->type)
	{
	case CT_SPRITE:
		height_added = 200;
		n = new SpriteViewer(current_height,width,height_added,(ComponentSprite*)comp,Handler);
		break;
	default:
		break;
	}


	//if (n != nullptr)
	components.push_back(n);

	return height_added + current_height;
}

void ObjectInspector::HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	switch (wpar)
	{
	case BUT_ADD_COMPONENT:
		//add component to object that is being observed
		to_inspect->AddComponent((CompType)ComponentTypeList.GetSelectedItem());
		SetGameObject(to_inspect->myid);
		break;
	case BUT_REM_OBJECT:
		MessageBox(NULL, "pleaseasd", "helpjkasdbdhjkas", MB_OK);
		break;
	default:
		break;
	}

}

void ObjectInspector::HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
}

void ObjectInspector::OnResize(int newx, int newy)
{


}


LRESULT CALLBACK ObjectInspectorProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR pointer)
{
	switch (msg) {
	case WM_COMMAND:
		EditorWindow::Get().GetObjectInspector()->HandleCommands(hwnd, wParam, lParam);
		break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, ObjectInspectorProc, 0);
		break;
	//case WM_PAINT:
	//	MessageBox(NULL, "lol", "lmao", MB_OK);
	//	break;
	case WM_NOTIFY:
		EditorWindow::Get().GetObjectInspector()->HandleNotify(hwnd, wParam, lParam);
		break;
	}
	//return DefSubclassProc

	return DefSubclassProc(hwnd, msg, wParam, lParam);
}