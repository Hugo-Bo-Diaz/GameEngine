#ifndef SCENE_VIEWER__H
#define SCENE_VIEWER__H

#include "../Dialog.h"

#include "../UIclasses/TreeViewer.h"

#include <vector>


LRESULT CALLBACK SceneViewerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);

struct ObjInHier {
	HTREEITEM parentitem;
	HTREEITEM treeitem;
	int gameobjectid;
};

class GameObject;

class SceneViewer : public Dialog
{
private:
	ObjInHier* FindObjInHierarchy(int id);
	ObjInHier* FindObjInHierarchy(HTREEITEM item);
	void DeleteFromHierarchy(int id);
public:

	int margin_x = 20;
	TreeViewer hierarchy;

	SceneViewer(LPCWSTR dialog_name, int min_size = 250, WindowPosition pos = LEFT, HWND editor_handler = NULL);


	void HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void OnResize(int newx, int newy);

	void UpdateGameObjectHierarchy(int id);
	std::vector<ObjInHier*> objects;

	void SendAddObjMsgEng();
	void SendDeleteObjMsg();

	int currentgameobjectidselected = -1;

	//FUNCTIONS THAT RECIEVE STRINGS FROM TEXTBOXES
	static void SSWAddGameObject(const char*, SceneViewer*);
};




#endif // !SCENE_VIEWER__H

