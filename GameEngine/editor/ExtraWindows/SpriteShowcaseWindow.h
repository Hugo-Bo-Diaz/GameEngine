#ifndef SHORT_STRING_WINDOW__H
#define SHORT_STRING_WINDOW__H

#include <Windows.h>
#include <string>
#include <functional>
#include "../UIclasses/UISliderBar.h"

#include "../EditorWindow.h"

LRESULT CALLBACK SpriteShowcaseProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);

LRESULT CALLBACK SpriteProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

struct WinTexture {

	BITMAPINFOHEADER info{ 0 };
	HWND handler;
	HBITMAP bitmap;
	int size_x;
	int size_y;

	unsigned char* image_buffer;

	void Draw(HDC& hdc, int x, int y, int w, int h);


	~WinTexture() {
		delete[] image_buffer;
	}
};


class SpriteShowcaseWindow {
public:

	HWND main_handler;

	UISliderBar slider_x;
	UISliderBar slider_y;

	WinTexture* sprite;

	int x,y,w,h;
	void SetDataFromWindowUI();

	HWND edit_stride_x;
	HWND edit_stride_y;

	HWND start_x;
	HWND start_y;

	HWND end_x;
	HWND end_y;

	HWND parent_win;


	void(*callback) (int, int, int, int);

	void Initialize(const char* path, LPCWSTR title, HWND parent, int curr_x, int curr_y, int curr_w, int curr_h);
	~SpriteShowcaseWindow();

	void PrintImage(HDC& hdc);


	float GetLabelfloat(HWND label)
	{
		char buffer[20];//20 figure number
		GetWindowText(label, buffer, 20);
		std::string ret = buffer;

		if (ret.length() == 0)
			return -1;

		return std::stoi(ret);
	}


};




#endif // !SHORT_STRING_WINDOW__H


//subclass?
//wait editor thread here??
//sendmessage??