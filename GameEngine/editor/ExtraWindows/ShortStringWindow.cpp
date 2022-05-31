#include "ShortStringWindow.h"
#include "../EditorWindow.h"
#include "../dialogs/SceneViewer.h"
#include "../../../Globals.h"

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

//std::string ret;
//HWND edit_handler;


//void* ShortStringWindow::Initialize(LPCWSTR title, HWND parent, std::function<const char* (EditorWindow)ptr ()>)

void ShortStringWindow::CreateSSWindow(LPCWSTR title, HWND parent)
{
	main_handler = CreateWindowW(L"static", title, WS_VISIBLE | WS_POPUP | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_CAPTION, EditorWindow::Get().width / 2 - 300 / 2, EditorWindow::Get().height , 300, 115, parent, NULL, NULL, NULL);
	SetWindowSubclass(main_handler, ShortStringProc, 0, (DWORD_PTR)this);

	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 255));
	SetClassLongPtr(main_handler, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

	CreateWindowW(L"button", L"OK", WS_VISIBLE | WS_CHILD, 20, 40, 100, 30, main_handler, (HMENU)SSW_OKEY, NULL, NULL);
	CreateWindowW(L"button", L"Cancel", WS_VISIBLE | WS_CHILD, 160, 40, 100, 30, main_handler, (HMENU)SSW_CANCEL, NULL, NULL);

	edit_handler = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE, 10, 10, 260, 20, main_handler, NULL, NULL, NULL);
}

void ShortStringWindow::Initialize(LPCWSTR title, HWND parent, void (*ptr) (const char*, EditorWindow*))
{
	callbackEW = ptr;
	CreateSSWindow(title, parent);
}

void ShortStringWindow::Initialize(LPCWSTR title, HWND parent, void(*ptr)(const char*, SceneViewer*))
{
	callbackSV = ptr;
	CreateSSWindow(title, parent);
}

LRESULT CALLBACK ShortStringProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR pointer)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case SSW_OKEY:
			GetWindowText(((ShortStringWindow*)pointer)->edit_handler, const_cast<char*>(((ShortStringWindow*)pointer)->str.c_str()), 100);

			if(((ShortStringWindow*)pointer)->callbackEW != 0)
				((ShortStringWindow*)pointer)->callbackEW(((ShortStringWindow*)pointer)->str.c_str(), &EditorWindow::Get());

			if (((ShortStringWindow*)pointer)->callbackSV != 0)
				((ShortStringWindow*)pointer)->callbackSV(((ShortStringWindow*)pointer)->str.c_str(), EditorWindow::Get().GetSceneViewer());


			DestroyWindow(hwnd);
			break;
		case SSW_CANCEL:
			DestroyWindow(hwnd);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		//EnableWindow(EditorWindow::Get().Handler, true);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
		break;
	}

	//return DefSubclassProc(hwnd, msg, wParam, lParam);
}

