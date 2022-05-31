#ifndef SHORT_STRING_WINDOW__H
#define SHORT_STRING_WINDOW__H

#include <Windows.h>
#include <string>
#include <functional>


#include "../EditorWindow.h"

LRESULT CALLBACK ShortStringProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);


class ShortStringWindow {
private:
	void CreateSSWindow(LPCWSTR title, HWND parent);
public:
	HWND main_handler;
	HWND edit_handler;

	std::string str;

	void(*callbackEW) (const char*, EditorWindow*) = 0;
	void(*callbackSV) (const char*, SceneViewer*) = 0;

	void Initialize(LPCWSTR title, HWND parent, void (*ptr) (const char*,EditorWindow*));//std::function<void EditorWindow::* (const char*)>);
	void Initialize(LPCWSTR title, HWND parent, void (*ptr) (const char*, SceneViewer*));
};




#endif // !SHORT_STRING_WINDOW__H


//subclass?
//wait editor thread here??
//sendmessage??