#include "UISliderBar.h"
#include <CommCtrl.h>


void UISliderBar::Initialize(int x, int y, int w, int h, HWND parent, LPCWSTR name, int _min, int _max, bool _ranged, SliderBarOrientation ori, int curr_x, int curr_y, bool _flipped)
{
	DWORD style = WS_VISIBLE | WS_CHILD | WS_BORDER| TBS_AUTOTICKS | TBS_NOTIFYBEFOREMOVE;
	
	min_r = _min;
	max_r = _max;
	ranged = _ranged;

	if (_ranged)
		style += TBS_ENABLESELRANGE;

	flipped = _flipped;

	switch (ori)
	{
	case SBO_TOP:
		style += TBS_TOP |TBS_HORZ;
		break;
	case SBO_BOTTOM:
		style += TBS_BOTTOM | TBS_HORZ;
		break;
	case SBO_LEFT:
		style += TBS_LEFT | TBS_VERT;
		break;
	case SBO_RIGHT:
		style += TBS_RIGHT | TBS_VERT;
		break;
	default:
		break;
	}

	SliderBar = CreateWindowW(TRACKBAR_CLASSW, name, style, x, y, w, h, parent, NULL, NULL, NULL);


	SendMessage(SliderBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(_min,_max));

	SendMessage(SliderBar, TBM_SETPOS, TRUE, curr_x);

	if (!flipped)
	{
		SendMessage(SliderBar, TBM_SETSELSTART, TRUE, curr_x);
		SendMessage(SliderBar, TBM_SETSELEND, TRUE, curr_y);
	}
	else
	{
		SendMessage(SliderBar, TBM_SETSELSTART, TRUE, max_r-curr_y);
		SendMessage(SliderBar, TBM_SETSELEND, TRUE, max_r-curr_x);
	}
	//if (_ranged)
	
	SendMessage(SliderBar, TBM_SETTIC, TRUE, 400);

	//if(curr_x !=-1 && curr_y != -1)
	//	SendMessage(SliderBar, TBM_SETSEL, (WPARAM)TRUE, (LPARAM)MAKELONG(curr_x, curr_y));
	//else
	//	SendMessage(SliderBar, TBM_SETSEL, (WPARAM)TRUE, (LPARAM)MAKELONG(min_r,max_r));


}

int UISliderBar::GetCurrentPosition()
{
	int ret = SendMessage(SliderBar, TBM_GETPOS, NULL, NULL);

	//if (flipped)
	//	ret = max_r - ret;

	return ret;

}

void UISliderBar::SetRange(int min_x, int max_x)
{
	SendMessage(SliderBar, TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(min_x, max_x));
}

void UISliderBar::SetStride(int str)
{
	SendMessage(SliderBar, TBM_SETTICFREQ, str, NULL);
	SendMessage(SliderBar, TBM_SETPAGESIZE, NULL, str);

}

int UISliderBar::GetSelectionStart()
{
	int ret = SendMessage(SliderBar, TBM_GETSELSTART, NULL, NULL);

	//if (flipped)
	//	ret = max_r - ret;

	return ret;
}

int UISliderBar::GetSelectionEnd()
{
	int ret = SendMessage(SliderBar, TBM_GETSELEND, NULL, NULL);
	
	//if (flipped)
	//	ret = max_r - ret;

	return ret;
}

void UISliderBar::SetSelectionPoint(bool first)
{
	int currpos = SendMessage(SliderBar, TBM_GETPOS, NULL, NULL);

	if (first)
	{
		SendMessage(SliderBar, TBM_SETSELSTART, TRUE, currpos);
	}
	else
	{
		SendMessage(SliderBar, TBM_SETSELEND, TRUE,currpos);
	}

}
