#include "UIListBox.h"
#include <CommCtrl.h>

void UIListBox::Initialize(int x, int y, int w, int h, HWND parent, LPCWSTR name, int column_width, int callback)
{

	DWORD style = WS_VISIBLE | WS_CHILD | WS_BORDER | LBS_HASSTRINGS | LBS_NOTIFY;
	
	int col_w = w;
	if (column_width != -1)
	{
		col_w = column_width;
		style += LBS_MULTICOLUMN;
	}

	ListBox = CreateWindowW(WC_LISTBOXW, name,style , x, y, w,h, parent, (HMENU)callback, NULL, NULL);


	SendMessage(ListBox, LB_SETCOLUMNWIDTH, col_w, NULL);


}

int UIListBox::AddItem(const char* name)
{
	return SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)TEXT(name));
}

void UIListBox::DeleteItem(int id)
{
	SendMessage(ListBox, LB_DELETESTRING, id, NULL);

}

void UIListBox::UpdateItem(int id, const char* newitem)
{
	
	SendMessage(ListBox,LB_SETITEMDATA, id, (LPARAM)TEXT(newitem));

}

void UIListBox::ClearItems()
{
	int length = GetTableLength();
	for (size_t i = 0; i <= length; ++i)
	{
		SendMessage(ListBox, LB_DELETESTRING, 0, NULL);
	}
}

int UIListBox::GetTableLength()
{
	return SendMessage(ListBox, LB_GETCOUNT, NULL, NULL);
}
