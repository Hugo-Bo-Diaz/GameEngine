#ifndef FILE_EXPLORER__H
#define FILE_EXPLORER__H

#include "../Dialog.h"

#include "../UIclasses/TreeViewer.h"

#include "../UIclasses/UIListBox.h"

#include <vector>
#include <string>


LRESULT CALLBACK FileExplorerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);

struct FoldInHier {
	HTREEITEM parentitem;
	HTREEITEM treeitem;
	std::string folder;
	std::string fullpath;
};

struct FileInList {
	std::string fullpath;
	std::string file;
	int id_in_listbox;

	FileInList() {};

	FileInList(const FileInList &fin) {
		fullpath = fin.fullpath;
		file = fin.file;
		id_in_listbox = -1;
	}
};

class GameObject;

class FileExplorer : public Dialog
{
private:
	FoldInHier* FindFoldInHierarchy(HTREEITEM item);
	void DeleteFromHierarchy(int id);
public:

	int margin_x = 20;
	TreeViewer folders_tree;

	UIListBox folder_list;
	UIListBox scene_list;

	FileExplorer(LPCWSTR dialog_name, int min_size = 250, WindowPosition pos = LEFT, HWND editor_handler = NULL);

	//RECURSIVE FUNCTION
	void FillFolderDirectory(const char* base_dir, TreeViewer*, HTREEITEM parent = NULL);
	FoldInHier* GetFolderFromHierarchy(HTREEITEM treeitem,TreeViewer* from);

	void FillFileListBox(const char* dir, UIListBox*);
	void ClearFileListBox(std::vector<FileInList*>*);
	FileInList* GetFileFromListIndex(int index, std::vector<FileInList*>* from);

	void AddFileToScene(FileInList* file);//SENDS MESSAGE
	void AddFileToSceneList(int resourceid);
	bool IsFileInList(FileInList* file, std::vector<FileInList*>*);
	bool IsFileInList(const char* path, std::vector<FileInList*>*);
	void RemoveFileFromScene(FileInList* file);

	void HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar);
	void OnResize(int newx, int newy);

	std::vector<FoldInHier*> folders;
	std::vector<FileInList*> files_in_folder;
	std::vector<FileInList*> files_in_scene;

	HWND button_add;
	HWND button_rem;

	//TO DO
	HWND selected_file_in_folder;
	HWND selected_file_in_scene;

};




#endif // !SCENE_VIEWER__H

