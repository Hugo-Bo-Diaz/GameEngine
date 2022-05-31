#ifndef UI_LIST_BOX__H
#define UI_LIST_BOX__H

#include <Windows.h>

enum SliderBarOrientation {
	SBO_TOP,
	SBO_BOTTOM,
	SBO_LEFT,
	SBO_RIGHT
};

class UISliderBar
{
public:
	HWND SliderBar;

	bool flipped;
	bool ranged;
	int min_r;
	int max_r;

	void Initialize(int x, int y, int w, int h, HWND parent, LPCWSTR name,int min, int max,bool ranged,SliderBarOrientation ori = SBO_TOP, int curr_min = -1, int curr_max = -1, bool flip = false);

	int GetCurrentPosition();

	void SetRange(int min_x = 0, int max_x = 0);
	void SetStride(int str);

	int GetSelectionStart();
	int GetSelectionEnd();
	void SetSelectionPoint(bool first);
};

#endif