#include "TreeViewer.h"


void TreeViewer::Initialize(int x, int y, int w, int h, HWND parent)
{
	TreeView = CreateWindowEx(0, WC_TREEVIEW, "TreeView", WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS, x, y, w, h, parent, NULL, NULL, NULL);

}

HTREEITEM TreeViewer::AddItem(const char* name, HTREEITEM parent, const char* image)
{


	TVITEM tvi = { 0 };
	tvi.mask = TVIF_TEXT;
	LPTSTR pszValue = LPTSTR(name);
	tvi.pszText = pszValue;
	tvi.cchTextMax = sizeof(tvi.pszText) / sizeof(tvi.pszText[0]);


	TVINSERTSTRUCT tvins;
	tvins.item = tvi;
	tvins.hParent = parent;
	tvins.hInsertAfter = TVI_LAST;


	 HTREEITEM hti = TreeView_InsertItem(TreeView, &tvins);

	return hti;
}

void TreeViewer::DeleteItem(HTREEITEM to_delete)
{
	TreeView_DeleteItem(TreeView, to_delete);

}

void TreeViewer::UpdateItem(HTREEITEM to_update, const char* newname, HTREEITEM newparent, const char* image)
{
	TVITEM edit = { 0 };
	edit.hItem = to_update;
	TreeView_GetItem(TreeView, &edit);

	LPTSTR pszValue = LPTSTR(newname);
	edit.pszText = pszValue;
	edit.cchTextMax = sizeof(edit.pszText) / sizeof(edit.pszText[0]);


	TreeView_SetItem(TreeView, &edit);

}
