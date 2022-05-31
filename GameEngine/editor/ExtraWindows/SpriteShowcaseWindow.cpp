#include "SpriteShowcaseWindow.h"
#include "../EditorWindow.h"
#include "../dialogs/SceneViewer.h"
#include "../../../Globals.h"
#include "../../../engine/ResLoaders/TexLoader.h"
#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")

//std::string ret;
//HWND edit_handler;


//void* ShortStringWindow::Initialize(LPCWSTR title, HWND parent, std::function<const char* (EditorWindow)ptr ()>)



void SpriteShowcaseWindow::Initialize(const char* path, LPCWSTR title, HWND parent, int curr_x, int curr_y, int curr_w, int curr_h){

	parent_win = parent;
	sprite = TextureLoader::LoadWinTexture(path);

	main_handler = CreateWindowW(L"static", title, WS_VISIBLE | WS_POPUP | WS_OVERLAPPEDWINDOW | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, EditorWindow::Get().width / 2 - sprite->size_x / 2, EditorWindow::Get().height / 2, sprite->size_x+250, sprite->size_y + 120, parent, NULL, NULL, NULL);
	SetWindowSubclass(main_handler, SpriteShowcaseProc, 0, (DWORD_PTR)this);

	HWND imagedisplay = CreateWindowW(L"static",L"",WS_VISIBLE|WS_CHILD,30,30,sprite->size_x,sprite->size_y,main_handler,NULL,NULL,NULL);
	SetWindowSubclass(imagedisplay, SpriteProc, 0, (DWORD_PTR)this);

	//SendMessageW(imagedisplay, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)sprite->bitmap);

												//CHANGE THIS TO IMAGE PROPORTIONS
	slider_x.Initialize(30, 0, sprite->size_x, 28, main_handler, L"x",0 , sprite->size_x, true, SBO_BOTTOM, curr_x, curr_x + curr_w);
	slider_y.Initialize(0, 30, 28, sprite->size_y, main_handler, L"y",0 , sprite->size_y, true, SBO_RIGHT, curr_x, curr_x + curr_w);

	CreateWindowW(L"button", L"Start", WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 0, 90, 30, main_handler, (HMENU)SPR_SET_START, NULL, NULL);
	CreateWindowW(L"button", L"End", WS_VISIBLE | WS_CHILD, sprite->size_x + 140, 0, 90, 30, main_handler, (HMENU)SPR_SET_END, NULL, NULL);

	CreateWindowW(L"static", L"Str x", WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 40, 90, 20, main_handler, NULL, NULL, NULL);
	CreateWindowW(L"static", L"Str y", WS_VISIBLE | WS_CHILD, sprite->size_x + 140, 40, 90, 20, main_handler, NULL, NULL, NULL);

	edit_stride_x = CreateWindowW(L"edit", L"8",ES_NUMBER | WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 60, 90, 17, main_handler, (HMENU)SPR_STRIDE_C, NULL, NULL);
	edit_stride_y = CreateWindowW(L"edit", L"8",ES_NUMBER | WS_VISIBLE | WS_CHILD, sprite->size_x + 140, 60, 90, 17, main_handler, (HMENU)SPR_STRIDE_C, NULL, NULL);

	slider_x.SetStride(GetLabelfloat(edit_stride_x));
	slider_y.SetStride(GetLabelfloat(edit_stride_y));

	CreateWindowW(L"static", L"Starting position", WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 90, 200, 20, main_handler, NULL, NULL, NULL);
	start_x = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 120, 90, 20, main_handler, NULL, NULL, NULL);
	start_y = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, sprite->size_x + 140, 120, 90, 20, main_handler, NULL, NULL, NULL);

	CreateWindowW(L"static", L"Ending position", WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 150, 200, 20, main_handler, NULL, NULL, NULL);
	end_x = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, sprite->size_x + 40, 180, 90, 20, main_handler, NULL, NULL, NULL);
	end_y = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD, sprite->size_x + 140, 180, 90, 20, main_handler, NULL, NULL, NULL);

	CreateWindowW(L"button", L"OK", WS_VISIBLE | WS_CHILD, 10, sprite->size_y + 40, 150, 30, main_handler, (HMENU)SPR_OKEY, NULL, NULL);
	CreateWindowW(L"button", L"Cancel", WS_VISIBLE | WS_CHILD, 170, sprite->size_y + 40, 150, 30, main_handler, (HMENU)SPR_CANCEL, NULL, NULL);
	CreateWindowW(L"button", L"Apply", WS_VISIBLE | WS_CHILD, 330, sprite->size_y + 40, 150, 30, main_handler, (HMENU)SPR_APPLY, NULL, NULL);
}

SpriteShowcaseWindow::~SpriteShowcaseWindow()
{
	delete sprite;
}

void SpriteShowcaseWindow::PrintImage(HDC& h)
{
	sprite->Draw(h, 0, 0, sprite->size_x, sprite->size_y);
}


void SpriteShowcaseWindow::SetDataFromWindowUI()
{

	x = slider_x.GetSelectionStart();
	w = slider_x.GetSelectionEnd() - slider_x.GetSelectionStart();

	//y = slider_y.max_r- slider_y.GetSelectionEnd();
	//h = -slider_y.GetSelectionEnd() + slider_y.GetSelectionStart();

	y = slider_y.GetSelectionStart();
	h = slider_y.GetSelectionEnd() - slider_y.GetSelectionStart();

}

LRESULT CALLBACK SpriteShowcaseProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR pointer)
{
	SpriteShowcaseWindow* s = ((SpriteShowcaseWindow*)pointer);

	switch (msg)
	{
	case WM_COMMAND:
		switch (wParam)
		{
		case SPR_OKEY:

			s->SetDataFromWindowUI();

			SendMessage(s->parent_win, WM_COMMAND, (BN_CLICKED << 16) | SPR_OKEY, (LPARAM)hwnd);

			DestroyWindow(hwnd);


			break;
		case SPR_APPLY:

			s->SetDataFromWindowUI();

			SendMessage(s->parent_win, WM_COMMAND, (BN_CLICKED << 16) | SPR_APPLY, (LPARAM)hwnd);

			break;
		case SPR_CANCEL:

			
			//FREE IMAGE :^)

			SendMessage(s->parent_win, WM_COMMAND, (BN_CLICKED << 16) | SPR_CANCEL, (LPARAM)hwnd);

			DestroyWindow(hwnd);
			break;
		case SPR_SET_START:
			s->slider_x.SetSelectionPoint(true);
			s->slider_y.SetSelectionPoint(true);
			SetWindowText(s->start_x, std::to_string(s->slider_x.GetSelectionStart()).c_str());
			SetWindowText(s->start_y, std::to_string(s->slider_y.GetSelectionStart()).c_str());
			break;
		case SPR_SET_END:
			s->slider_x.SetSelectionPoint(false);
			s->slider_y.SetSelectionPoint(false);
			SetWindowText(s->end_x, std::to_string(s->slider_x.GetSelectionEnd()).c_str());
			SetWindowText(s->end_y, std::to_string(s->slider_y.GetSelectionEnd()).c_str());
			break;
		default:
			break;
		}

		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			s->slider_x.SetStride(s->GetLabelfloat(s->edit_stride_x));
			s->slider_y.SetStride(s->GetLabelfloat(s->edit_stride_y));
			break;
		default:
			break;
		}


		break;
	case WM_DESTROY:
		//EnableWindow(EditorWindow::Get().Handler, true);
		delete (SpriteShowcaseWindow*)pointer;
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);

	//return DefSubclassProc(hwnd, msg, wParam, lParam);
}


LRESULT CALLBACK SpriteProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR, DWORD_PTR pointer) {
	switch (msg)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC h = BeginPaint(hwnd, &ps);

			((SpriteShowcaseWindow*)pointer)->PrintImage(h);

			EndPaint(hwnd, &ps);
		}
			break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}




void WinTexture::Draw(HDC& hdc, int x, int y, int w, int h)
{
	SetStretchBltMode(hdc, HALFTONE);

	StretchDIBits(hdc, x, y, w, h, 0, 0, size_x, size_y, image_buffer, (LPBITMAPINFO)&info, DIB_RGB_COLORS, SRCCOPY);
};