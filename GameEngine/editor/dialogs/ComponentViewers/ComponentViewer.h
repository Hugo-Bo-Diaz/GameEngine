#ifndef COMPONENT_VIEWER__H
#define COMPONENT_VIEWER__H
#include <Windows.h>


class ComponentViewer {
public:
	HWND handler;
	int height;
	int width;
	HWND title;

	virtual void ApplyComponent() {};
};

#endif