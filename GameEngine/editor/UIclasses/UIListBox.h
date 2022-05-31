#ifndef UI_LIST_BOX__H
#define UI_LIST_BOX__H

#include <Windows.h>


class UIListBox
{
public:
	HWND ListBox;

	bool multicolumn;


	void Initialize(int x, int y, int w, int h, HWND parent, LPCWSTR name, int column_w = -1, int callback = 0);

	int AddItem(const char* name);
	void DeleteItem(int id);
	void UpdateItem(int id, const char* newitem);
	void ClearItems();

	int GetTableLength();

};

#endif