#include "SceneViewer.h"
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

#include "../../Globals.h"

#include "../EditorWindow.h"
#include "ObjectInspector.h"

#include "../../engine/Engine.h"
#include "../../engine/Scene.h"
#include "../../engine/GameObject/GameObject.h"

#include "../../../MessageManager.h"

#include "../ExtraWindows/ShortStringWindow.h"


SceneViewer::SceneViewer(LPCWSTR dialog_name, int width, WindowPosition _pos, HWND editor_handler)
{
	type = SCENE_VIEWER;
	pos = _pos;
	min_size = width;
	Handler = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 100, 100, editor_handler, NULL, NULL, NULL);
	SetWindowSubclass(Handler, SceneViewerProc, 0, 0);

	//add title
	CreateWindowW(L"static", dialog_name, WS_VISIBLE | WS_CHILD, 20, 50, width, 20, Handler, NULL,NULL, NULL);

	hierarchy.Initialize(margin_x, 70, width-margin_x*2, 300, Handler);

	HWND addbutt = CreateWindowW(L"button",L"Add Object", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER ,20,10,100,30,Handler,(HMENU)BUT_ADD_OBJECT,NULL,NULL);
	HWND rembutt = CreateWindowW(L"button", L"Remove Object", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 130, 10, 100, 30, Handler, (HMENU)BUT_REM_OBJECT, NULL, NULL);


	handlers.push_back(addbutt);
	handlers.push_back(rembutt);
	handlers.push_back(hierarchy.TreeView);
	handlers.push_back(Handler);
}

void SceneViewer::HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	switch (wpar)
	{
	case BUT_ADD_OBJECT:
		SendAddObjMsgEng();
		break;
	case BUT_REM_OBJECT:
		SendDeleteObjMsg();
		break;
	default:
		break;
	}

}

void SceneViewer::HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	
	switch (((LPNMHDR)lpar)->code)
	{
	case TVN_SELCHANGEDW:
	case TVN_SELCHANGEDA:
	case TCN_SELCHANGE:
	
		NMTREEVIEW* pnmtv = (LPNMTREEVIEW)lpar;
		currentgameobjectidselected = FindObjInHierarchy(pnmtv->itemNew.hItem)->gameobjectid;
		EditorWindow::Get().GetObjectInspector()->SetGameObject(currentgameobjectidselected);
	
		break;
	}

}

void SceneViewer::OnResize(int newx, int newy)
{

	MoveWindow(hierarchy.TreeView, margin_x, 70, width - margin_x*2, newy - 90,TRUE);

}

void SceneViewer::UpdateGameObjectHierarchy(int id)
{
	//get the gameobject
	const GameObject* obj = Engine::Get().sc->GetGameObject(id);
	ObjInHier* InHier = FindObjInHierarchy(id);

	//object is missing! attempt to delete from my array
	if (obj == nullptr)
	{
		//it was not even in this lol, DO NOTHING
		if (InHier == nullptr) 
		{
			return;
		}
		else//delete from the tree view
		{
			if (EditorWindow::Get().GetObjectInspector() != nullptr)
				EditorWindow::Get().GetObjectInspector()->SetGameObject(-1);

			DeleteFromHierarchy(id);
		}
			
	}
	else//add or update the data on the hierarchy
	{
		if (InHier == nullptr)//add to hierarchy
		{
			ObjInHier* oih = new ObjInHier();
			oih->gameobjectid = id;
			oih->treeitem = hierarchy.AddItem(obj->name.c_str());//WHEN CHILDREN ARE IMPLEMENTED ADD THEM AS PARENT
			
			objects.push_back(oih);
		}
		else//update on hierarchy
		{
			hierarchy.UpdateItem(InHier->treeitem, obj->name.c_str());
		}

	}



}

void SceneViewer::SendAddObjMsgEng()
{
	ShortStringWindow* ssw = new ShortStringWindow();
	ssw->Initialize(L"New Object", Handler, SSWAddGameObject);
}

void SceneViewer::SSWAddGameObject(const char* res, SceneViewer* sv)
{
	addobjmsg* m = new addobjmsg();

	m->dest = ENGINE;
	m->name = res;

	MessageManager::Get().AddMessage(m);
}



void SceneViewer::SendDeleteObjMsg()
{
	if (currentgameobjectidselected == -1)
		return;

	deleteobjmsg* m = new deleteobjmsg();

	m->dest = ENGINE;
	m->to_delete = currentgameobjectidselected;

	MessageManager::Get().AddMessage(m);
}


ObjInHier* SceneViewer::FindObjInHierarchy(int id)
{
	if (objects.empty())
		return nullptr;

	for (ObjInHier* oih : objects)
	{
		if (oih->gameobjectid == id)
			return oih;
	}

	return nullptr;
}

ObjInHier* SceneViewer::FindObjInHierarchy(HTREEITEM item)
{
	if (objects.empty())
		return nullptr;

	for (ObjInHier* oih : objects)
	{
		if (oih->treeitem == item)
			return oih;
	}

	return nullptr;
}

void SceneViewer::DeleteFromHierarchy(int id)
{
	for (auto it = objects.begin(); it != objects.end(); ) {
		if ((*it)->gameobjectid == id) 
		{
			hierarchy.DeleteItem((*it)->treeitem);
			delete* it;
			it = objects.erase(it);
		}
		else 
		{
			++it;
		}
	}
}


LRESULT CALLBACK SceneViewerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR)
{
	switch (msg) {
	case WM_COMMAND:
		EditorWindow::Get().GetSceneViewer()->HandleCommands(hwnd, wParam, lParam);
		break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, SceneViewerProc, 0);
		break;
	case WM_NOTIFY:
		EditorWindow::Get().GetSceneViewer()->HandleNotify(hwnd, wParam, lParam);
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}