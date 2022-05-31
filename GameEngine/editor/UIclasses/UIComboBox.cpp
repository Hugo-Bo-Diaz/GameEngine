#include "UIComboBox.h"
#include <CommCtrl.h>

void UIComboBox::Initialize(int x, int y, int w, int h, HWND parent, int elements)
{

	ComboBox = CreateWindowW(WC_COMBOBOXW, L"", CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | CBS_DROPDOWNLIST, x, y, w,h*elements, parent, NULL, NULL, NULL);
}

int UIComboBox::AddItem(const char* name)
{
	int cbret = SendMessage(ComboBox, CB_ADDSTRING, 0, (LPARAM)name);
	//SendMessage(ComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

	if (cbret == CB_ERR)
	{
		return 1;
	}
	return 0;
}

void UIComboBox::DeleteItem(int id)
{
	SendMessage(ComboBox, CB_DELETESTRING, id, NULL);

}

void UIComboBox::UpdateItem(int id, const char* newitem)
{
	
	SendMessage(ComboBox,	CB_SETITEMDATA, id, (LPARAM)newitem);

}

void UIComboBox::ClearItems()
{
	int length = SendMessage(ComboBox, CB_GETCOUNT, NULL, NULL);

	for (size_t i = 0; i <= length; ++i)
	{
//		SendMessage(ComboBox, CB_DELETESTRING, 0, NULL);
	}
}

int UIComboBox::GetSelectedItem()
{
	return SendMessage(ComboBox,CB_GETCURSEL,NULL,NULL);
}

void UIComboBox::SetCurrentItem(int id)
{
	SendMessage(ComboBox, CB_SETCURSEL, id, NULL);
}
