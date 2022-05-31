#ifndef TRANSFORM_VIEWER__H
#define TRANSFORM_VIEWER__H
#include "ComponentViewer.h"


class Transform;

class TransformViewer : public ComponentViewer {
public:
	TransformViewer(int y, int width, int height, Transform* tr, HWND parent);

	void ApplyComponent();
	void ChangeEditor(Transform* new_transform);

	void ChangeLabelText(HWND label, const char* text);
	float GetLabelfloat(HWND label);

	//position
	HWND translate_text;

	HWND t_x;
	HWND t_y;
	HWND t_z;

	HWND rotate_text;
	HWND r;

	HWND scale_text;
	HWND s_x;
	HWND s_y;

	Transform* to_transform;

	static LRESULT CALLBACK TransformViewerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);


};


#endif // !TRANSFORM_VIEWER__H