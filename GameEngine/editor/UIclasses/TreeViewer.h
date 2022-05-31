#ifndef TREE_VIEWER__H
#define TREE_VIEWER__H

#include <Windows.h>
#include <CommCtrl.h>


class TreeViewer
{
public:
	HWND TreeView;

	void Initialize(int x, int y, int w, int h, HWND parent);

	HTREEITEM AddItem(const char* name, HTREEITEM parent = NULL, const char* image = "");
	void DeleteItem(HTREEITEM to_delete);
	void UpdateItem(HTREEITEM to_update, const char* newname, HTREEITEM newparent = NULL, const char* image = "");
};

#endif