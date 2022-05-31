#ifndef UI_COMBO_BOX__H
#define UI_COMBO_BOX__H

#include <Windows.h>


class UIComboBox
{
public:
	HWND ComboBox;

	void Initialize(int x, int y, int w, int h, HWND parent, int elements_shown_at_once = 10);

	int AddItem(const char* name);
	void DeleteItem(int id);
	void UpdateItem(int id, const char* newitem);
	void ClearItems();
	int GetSelectedItem();
	void SetCurrentItem(int id);
};

#endif