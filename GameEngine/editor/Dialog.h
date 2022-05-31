#ifndef DIALOG__H
#define DIALOG__H

#include <Windows.h>
#include <list>

enum WindowPosition {
	LEFT,
	BOTTOM,
	RIGHT,
	TOP,
};

class Dialog {
public:

	HWND Handler;
	Dialog() {};

	Dialog(LPCWSTR dialog_name, int min_size = 250, WindowPosition pos = LEFT, HWND editor_handler = NULL);

	//POSITION
	int min_size;
	int x, y;
	int width;
	int height;
	void SetDialogPos(int x, int y, int width, int height);
	int priority = 0;
	WindowPosition pos = LEFT;

	int type = 0;

	//
	virtual void HandleCommands(HWND hwnd,WPARAM wpar, LPARAM lpar) {};
	virtual void HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar) {};


	virtual void OnResize(int newx, int newy) {};


	bool operator< (const Dialog& lhs) 
	{
		if (lhs.priority < priority)
			return true;
		else
			return false;
	}

	std::list<HWND> handlers;

};

bool comparePriority(Dialog* i, Dialog* j);

#endif // !WINDOW__H
