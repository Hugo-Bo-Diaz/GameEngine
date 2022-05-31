#ifndef TOP_BAR__H
#define TOP_BAR__H

#include "../Dialog.h"

#include <vector>
#include <string>

LRESULT CALLBACK TopBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);

struct msg;

class TopBar : public Dialog
{
private:
public:

	TopBar(LPCWSTR dialog_name, int min_size = 40, WindowPosition pos = TOP, HWND editor_handler = NULL);

	HWND button_play;
	HWND button_pause;
	HWND button_stop;

	HWND FocusText;
	void SetFocusText(bool editor_is_focused);
	std::string editorfocus = "Editor";
	std::string enginefocus = "Engine";

	void HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void OnResize(int newx, int newy);

	void HandleMessage(msg*);

};




#endif // !SCENE_VIEWER__H

