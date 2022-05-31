#include "TransformViewer.h"
#include "../../../engine/GameObject/GameObject.h"
#include "../../EditorWindow.h"
#include "../ObjectInspector.h"

#include <CommCtrl.h>

#pragma comment(lib, "comctl32.lib")

TransformViewer::TransformViewer(int y, int width, int height, Transform* tr, HWND Handler)
{
	handler = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, y, width, height, Handler, NULL, NULL, NULL);
	SetWindowSubclass(handler, TransformViewerProc, 0, 0);

	title = CreateWindowW(L"static", L"Transform", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, handler, NULL, NULL, NULL);

	to_transform = tr;
	
	int label_w = 100;
	translate_text = CreateWindowW(L"static", L"Position", WS_VISIBLE | WS_CHILD | SS_CENTER, width / 2 - label_w / 2, 30, label_w, 20, handler, NULL, NULL, NULL);

	std::wstring num_text;
	std::wstring rounded;

	num_text = std::to_wstring(tr->pos.x);
	rounded = num_text.substr(0, num_text.find(L".") + 3);
	t_x = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, 10, 50, (width / 3) - 20, 17, handler, NULL, NULL, NULL);
	num_text = std::to_wstring(tr->pos.y);
	rounded = num_text.substr(0, num_text.find(L".") + 3);
	t_y = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, 20 + ((width / 3) - 20), 50, (width / 3) - 20, 17, handler, NULL, NULL, NULL);
	num_text = std::to_wstring(tr->pos.z);
	rounded = num_text.substr(0, num_text.find(L".") + 3);
	t_z = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, 30 + ((width / 3) - 20)*2, 50, (width / 3) - 20, 17, handler, NULL, NULL, NULL);


	rotate_text = CreateWindowW(L"static", L"Rotation", WS_VISIBLE | WS_CHILD | SS_CENTER, width / 2 - label_w / 2, 70, label_w, 20, handler, NULL, NULL, NULL);

	num_text = std::to_wstring(tr->rotation);
	rounded = num_text.substr(0, num_text.find(L".") + 3);
	r = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, width / 2 - label_w / 2, 90, (width / 3) - 20, 17, handler, NULL, NULL, NULL);


	scale_text = CreateWindowW(L"static", L"Scale", WS_VISIBLE | WS_CHILD | SS_CENTER, width / 2 - label_w / 2, 110, label_w, 20, handler, NULL, NULL, NULL);

	num_text = std::to_wstring(tr->scale.x);
	rounded = num_text.substr(0, num_text.find(L".") + 3);
	s_x = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, width/6 -10, 130, (width / 3) - 20, 17, handler, NULL, NULL, NULL);
	num_text = std::to_wstring(tr->scale.y);
	rounded = num_text.substr(0, num_text.find(L".") + 3);
	s_y = CreateWindowW(L"edit", rounded.c_str(), ES_NUMBER | WS_CHILD | WS_VISIBLE | SS_RIGHT, width - ( (width / 3) - 20) -(width/6 - 10), 130, (width / 3) - 20, 17, handler, NULL, NULL, NULL);

}

void TransformViewer::ApplyComponent()
{
	to_transform->pos = { GetLabelfloat(t_x), GetLabelfloat(t_y), GetLabelfloat(t_z)};
	to_transform->rotation = GetLabelfloat(r);
	to_transform->scale = { GetLabelfloat(s_x),GetLabelfloat(s_y) };
	to_transform->CalculateMatrix();
}

void TransformViewer::ChangeEditor(Transform* new_transform)
{


}

void TransformViewer::ChangeLabelText(HWND label, const char* text)
{
}

float TransformViewer::GetLabelfloat(HWND label)
{
	char buffer[20];//20 figure number
	GetWindowText(label, buffer, 20);
	std::string ret = buffer;

	if (ret.length() == 0)
		return -1;

	return std::stof(ret);
}



LRESULT TransformViewer::TransformViewerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			EditorWindow::Get().GetObjectInspector()->ComponentChanged(hwnd);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


	return DefSubclassProc(hwnd, msg, wParam, lParam);
}