#ifndef EDITOR_WINDOW__H
#define EDITOR_WINDOW__H

#ifndef UNICODE
#define UNICODE
#endif 

//engine action types, different commands
#include <Windows.h>
#include "../TaskRunner.h"
#include <vector>
LRESULT CALLBACK EditorWindowProcedure(HWND, UINT, WPARAM, LPARAM);

class Dialog;
class SceneViewer;
class ObjectInspector;
class FileExplorer;
class TopBar;

enum DialogType {
	DTYPE_NONE = 0,
	SCENE_VIEWER,
	OBJECT_INSPECTOR,
	FILE_EXPLORER,
	TOP_BAR,
};


class EditorWindow{
private:

	WNDCLASSW defaultClass = {0};
	EditorWindow() {};

	Dialog* GetDialog(DialogType type);

public:
	HWND Handler;

	HWND EngineView;
	void SetEngineView(HWND);
	
	void Initialize(HINSTANCE);

	bool Loop();
	void ShutDown();
	
	SceneViewer* GetSceneViewer();
	ObjectInspector* GetObjectInspector();
	FileExplorer* GetFileExplorer();
	TopBar* GetTopBar();
	void AddMenu(HWND addto);

	std::vector<Dialog*> dialogs;


	void OnResize(int newx, int newy);
	int width;
	int height;
	int GetMinWidth();
	int GetMinHeight();
	
	void PlaceLayout();

	int dialog_size = 250;

	static EditorWindow& Get();
	
	bool RecieveEngineMessages();
	void SendQuitMessage();

	
	//0 = new, 1 = save, 2 = load
	void OpenFileNameDialog(int action);

	//FUNCTIONS THAT RECIEVE STRINGS FROM TEXTBOXES
	static void SSWNew(const char*, EditorWindow*);
	static void SSWSave(const char*, EditorWindow*);
	static void SSWLoad(const char*, EditorWindow*);

};

#endif