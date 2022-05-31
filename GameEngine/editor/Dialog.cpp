#include "Dialog.h"
#include <Windows.h>


Dialog::Dialog(LPCWSTR dialog_name, int width, WindowPosition _pos, HWND editor_handler)
{
	pos = _pos;
	min_size = width;
	Handler = CreateWindowW(L"static", dialog_name, WS_VISIBLE | WS_CHILD | WS_BORDER , 0, 0, 100, 100, editor_handler, NULL, NULL, NULL);

}

void Dialog::SetDialogPos(int _x, int _y, int _width, int _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;

	MoveWindow(Handler,x,y,width,height, TRUE);

	OnResize(width, height);
}

bool comparePriority(Dialog* i, Dialog* j)
{
	return (i->priority < j->priority);
}
