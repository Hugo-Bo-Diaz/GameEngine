#include "TopBar.h"
#include <Windows.h>
#include <CommCtrl.h>
#include <windowsx.h>

#include "../../Globals.h"

#include "../EditorWindow.h"
#include "ObjectInspector.h"

#include "../../../MessageManager.h"

TopBar::TopBar(LPCWSTR dialog_name, int width, WindowPosition _pos, HWND editor_handler)
{
	type = TOP_BAR;
	pos = _pos;
	min_size = width;
	Handler = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 100, 100, editor_handler, NULL, NULL, NULL);
	SetWindowSubclass(Handler, TopBarProc, 0,(DWORD_PTR)this);


	button_play = CreateWindowW(L"button",L">", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER ,width/2 - 55, 5,30,30,Handler,(HMENU)BUT_PLAY,NULL,NULL);
	button_pause = CreateWindowW(L"button", L"||", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, width / 2 - 15, 5, 30, 30, Handler, (HMENU)BUT_PAUSE, NULL, NULL);
	button_stop = CreateWindowW(L"button", L"[]", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, width / 2 + 25, 5, 30, 30, Handler, (HMENU)BUT_STOP, NULL, NULL);


	CreateWindowW(L"static", L"Focus:", WS_VISIBLE | WS_CHILD, 10, width / 2 - 8, 50, 17, Handler, NULL, NULL, NULL);
	FocusText = CreateWindowW(L"static", L"Editor", WS_VISIBLE | WS_CHILD, 60, width / 2 - 8, 100, 17, Handler, NULL, NULL, NULL);
}

void TopBar::SetFocusText(bool editor_is_focused)
{
	if (editor_is_focused)
		SetWindowText(FocusText, editorfocus.c_str());
	else
		SetWindowText(FocusText, enginefocus.c_str());
}

void TopBar::HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	switch (wpar)
	{
	case BUT_PLAY:
	{
		Button_Enable(button_play, FALSE);

		msg* m = new msg();
		m->code = PLAY_ENGINE;
		m->dest = ENGINE;
		MessageManager::Get().AddMessage(m);
		
	}
		break;
	case BUT_PAUSE:
	{
		Button_Enable(button_pause, FALSE);

		msg* m = new msg();
		m->code = PAUSE_ENGINE;
		m->dest = ENGINE;
		MessageManager::Get().AddMessage(m);
	}
		break;
	case BUT_STOP:
	{
		Button_Enable(button_stop, FALSE);

		msg* m = new msg();
		m->code = STOP_ENGINE;
		m->dest = ENGINE;
		MessageManager::Get().AddMessage(m);
	}
		break;
	default:
		break;
	}

}

void TopBar::HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	


}

void TopBar::OnResize(int newx, int newy)
{

	MoveWindow(button_play, newx / 2 - 55, 5, 30, 30,TRUE);
	MoveWindow(button_pause, newx / 2 - 15, 5, 30, 30, TRUE);
	MoveWindow(button_stop, newx / 2 + 25, 5, 30, 30, TRUE);

}

void TopBar::HandleMessage(msg* m)
{
	switch (m->code)
	{
	case STOPPED_BUTTON:
		Button_Enable(button_stop,TRUE);
		break;
	case PLAYED_BUTTON:
		Button_Enable(button_play, TRUE);
		break;
	case PAUSED_BUTTON:
		Button_Enable(button_pause, TRUE);
		break;
	default:
		break;
	}
}

LRESULT CALLBACK TopBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR top_bar)
{
	switch (msg) {
	case WM_COMMAND:
		((TopBar*)top_bar)->HandleCommands(hwnd, wParam, lParam);
		break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, TopBarProc, 0);
		break;
	case WM_NOTIFY:
		((TopBar*)top_bar)->HandleNotify(hwnd, wParam, lParam);
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}