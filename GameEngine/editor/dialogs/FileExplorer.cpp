#include <Windows.h>

#include "FileExplorer.h"

#include <CommCtrl.h>
#pragma comment(lib, "comctl32.lib")


#include "../../Globals.h"

#include "../EditorWindow.h"
#include "ObjectInspector.h"

#include "../../engine/Engine.h"
#include "../../engine/Resources.h"

#include "../../rapidjson/rapidjson.h"

#include "../../MessageManager.h"

FileExplorer::FileExplorer(LPCWSTR dialog_name, int width, WindowPosition _pos, HWND editor_handler)
{
	type = FILE_EXPLORER;
	pos = _pos;
	min_size = width;
	Handler = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 0, 0, 100, 100, editor_handler, NULL, NULL, NULL);
	SetWindowSubclass(Handler, FileExplorerProc, 0, 0);

	//add title

	folders_tree.Initialize(margin_x, margin_x*2, 200, width-(margin_x * 2), Handler);
	folder_list.Initialize(margin_x + 200, margin_x*2, 200, width - (margin_x * 2), Handler,L"list in folder",250,FOLDER_LIST_CB);
	scene_list.Initialize(margin_x + 400, margin_x*2, 200, width - (margin_x * 2), Handler, L"list in scene", 250,SCENE_LIST_CB);


	button_add = CreateWindowW(L"Button", L"Add->", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 0, 0, 100, 30, Handler, (HMENU)BUT_ADD_RESOURCE, NULL, NULL);
	button_rem = CreateWindowW(L"Button", L"Remove", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 0, 0, 100, 30, Handler, (HMENU)BUT_REM_RESOURCE, NULL, NULL);

	selected_file_in_folder = CreateWindowW(L"static", L"sel", WS_VISIBLE | WS_CHILD, 0, 0, 100, 20, Handler, (HMENU)NULL, NULL, NULL);
	selected_file_in_scene = CreateWindowW(L"static", L"sel", WS_VISIBLE | WS_CHILD, 0, 0, 100, 20, Handler, (HMENU)NULL, NULL, NULL);
}


void FileExplorer::FillFolderDirectory(const char* base_dir, TreeViewer* to_fill, HTREEITEM parent)
{

	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pwd);

	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	//std::string fulldir = base_dir;// = pwd; 

	std::string fulldir = pwd;
	

	//BASE DIRECTORY ADDITION
	if (base_dir == "")
	{
		FoldInHier* newfold = new FoldInHier();
		newfold->folder = "Assets";
		newfold->fullpath = fulldir;
		newfold->parentitem = NULL;
		parent = newfold->treeitem = to_fill->AddItem(newfold->folder.c_str(),NULL);
		folders.push_back(newfold);
		//parent = to_fill->AddItem("root", 0);
	}


	if (base_dir != "")
	{
		//fulldir += "\\";
		fulldir += base_dir;
	}
	fulldir += "\\*";
	if ((hFind = FindFirstFile(fulldir.c_str(), &fdFile)) == INVALID_HANDLE_VALUE)
	{
		return;
	}
	fulldir = fulldir.erase(fulldir.size()-2);

	do
	{
		//first two files
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			//is it a folder
			if (fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				std::string folder_rel = base_dir;
				folder_rel += "\\";
				folder_rel += fdFile.cFileName;

				FoldInHier* newfold = new FoldInHier();
				newfold->folder = fdFile.cFileName;
				newfold->fullpath = fulldir + "\\" + fdFile.cFileName;
				newfold->parentitem = parent;
				newfold->treeitem = to_fill->AddItem(newfold->folder.c_str(), parent);
				folders.push_back(newfold);

				FillFolderDirectory(folder_rel.c_str(), to_fill, newfold->treeitem);


			}
		}

	}
	while (FindNextFile(hFind, &fdFile));

	FindClose(hFind);

}

FoldInHier* FileExplorer::GetFolderFromHierarchy(HTREEITEM treeitem, TreeViewer* from)
{
	return nullptr;
}

void FileExplorer::FillFileListBox(const char* dir, UIListBox* to_fill)
{
	to_fill->ClearItems();
	ClearFileListBox(&files_in_folder);

	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	std::string fulldir = dir;
				fulldir += "//*";


	if ((hFind = FindFirstFile(fulldir.c_str(), &fdFile)) == INVALID_HANDLE_VALUE)
	{
		return;
	}

	fulldir = fulldir.erase(fulldir.size() - 3);

	//to_fill->AddItem(dir);

	do
	{
		//first two files
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			//is it a folder
			if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{}
			else
			{
				FileInList* FiL = new FileInList();
				FiL->file = fdFile.cFileName;
				FiL->fullpath = fulldir + "\\" + FiL->file;
				FiL->id_in_listbox = to_fill->AddItem(FiL->file.c_str());
				files_in_folder.push_back(FiL);
			}
		}

	} while (FindNextFile(hFind, &fdFile) != 0);

	FindClose(hFind);

}

void FileExplorer::ClearFileListBox(std::vector<FileInList*>* to_clear)
{
	if (to_clear->empty())
		return;

	for (FileInList* fin : *to_clear)
	{
		if(fin != nullptr)
			delete fin;
	}

	to_clear->clear();

}

FileInList* FileExplorer::GetFileFromListIndex(int index, std::vector<FileInList*>* from)
{
	if (from->empty())
		return nullptr;

	for (FileInList* fin : *from)
	{
		if (fin->id_in_listbox == index)
			return fin;
	}

	return nullptr;
}

void FileExplorer::AddFileToScene(FileInList* file)
{
	updateresourcesmsg* m1 = new updateresourcesmsg();

	m1->dest = ENGINE;
	m1->path = file->file.c_str();
	m1->fullpath = file->fullpath.c_str();

	MessageManager::Get().AddMessage(m1);

	//bool fileinlist = true;

	//if (file != nullptr) {
	//	fileinlist = IsFileInList(file, &files_in_scene);
	//}

	//if (!fileinlist)
	//{
	//	//posible data dup
	//	FileInList* nfile = new FileInList(*file);
	//	nfile->id_in_listbox = scene_list.AddItem(nfile->file.c_str());
	//	files_in_scene.push_back(nfile);
	//	//load this file into resources
	//}
}

void FileExplorer::AddFileToSceneList(int resourceid)
{

	Resource* r = Engine::Get().rs->GetResource(resourceid);

	r->path;

	bool fileinlist = true;

	if (r != nullptr) {
		fileinlist = IsFileInList(r->path.c_str(), &files_in_scene);
	}

	if (!fileinlist)
	{
		//posible data dup
		FileInList* nfile = new FileInList();
		nfile->file = r->name;
		nfile->fullpath = r->path;
		nfile->id_in_listbox = scene_list.AddItem(nfile->file.c_str());
		files_in_scene.push_back(nfile);
		//load this file into resources
	}


}

bool FileExplorer::IsFileInList(FileInList* file, std::vector<FileInList*>* list)
{
	for (FileInList* f : files_in_scene)
	{
		if (strcmp(file->fullpath.c_str(), f->fullpath.c_str()) == 0)
			return true;
	}

	return false;
}

bool FileExplorer::IsFileInList(const char* path, std::vector<FileInList*>*)
{
	for (FileInList* f : files_in_scene)
	{
		if (strcmp(path, f->fullpath.c_str()) == 0)
			return true;
	}

	return false;
}

void FileExplorer::RemoveFileFromScene(FileInList* file)
{
	bool fileinlist = IsFileInList(file, &files_in_scene);

	if (file != nullptr && fileinlist)
	{
		bool item_has_been_deleted = false;

		for (auto it = files_in_scene.begin(); it != files_in_scene.end(); )
		{
			if (strcmp((*it)->fullpath.c_str(),file->fullpath.c_str()) == 0) 
			{
				item_has_been_deleted = true;
				scene_list.DeleteItem((*it)->id_in_listbox);
				delete* it;
				it = files_in_scene.erase(it);
			}
			else 
			{
				if (item_has_been_deleted)
					(*it)->id_in_listbox -= 1;

				++it;
			}
		}


	}
}

void FileExplorer::HandleCommands(HWND hwnd, WPARAM wpar, LPARAM lpar)
{

	std::string temp = "";
	int selected_index_on_folder_list = SendMessage((HWND)folder_list.ListBox, LB_GETCURSEL, NULL, NULL);
	int selected_index_on_scene_list = SendMessage((HWND)scene_list.ListBox, LB_GETCURSEL, NULL, NULL);

	FileInList* file_in_folder_list = GetFileFromListIndex(selected_index_on_folder_list, &files_in_folder);
	FileInList* file_in_scene_list = GetFileFromListIndex(selected_index_on_scene_list, &files_in_scene);

	int wmId = LOWORD(wpar);
	int wmEvent = HIWORD(wpar);

	switch (wpar)
	{
	case BUT_ADD_RESOURCE:
		AddFileToScene(file_in_folder_list);
		break;
	case BUT_REM_RESOURCE:
		RemoveFileFromScene(file_in_scene_list);
		break;
	default:
		break;
	}

	switch (wmEvent)
	{
	case LBN_SELCHANGE:
		switch (wmId)
		{
		case FOLDER_LIST_CB:
			temp = GetFileFromListIndex(selected_index_on_folder_list,&files_in_folder)->fullpath + std::to_string(selected_index_on_folder_list);
			SetWindowText(selected_file_in_folder,temp.c_str());
			break;
		case SCENE_LIST_CB:
			temp = GetFileFromListIndex(selected_index_on_scene_list,&files_in_scene)->fullpath + std::to_string(selected_index_on_scene_list);
			SetWindowText(selected_file_in_scene, temp.c_str());
			break;
		}

		break;
	}


}

void FileExplorer::HandleNotify(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	std::string temp;
	
	switch (((LPNMHDR)lpar)->code)
	{
	case TVN_SELCHANGEDW:
	case TVN_SELCHANGEDA:
	case TCN_SELCHANGE:
	
		NMTREEVIEW* pnmtv = (LPNMTREEVIEW)lpar;
		temp = FindFoldInHierarchy(pnmtv->itemNew.hItem)->fullpath;
		FillFileListBox(temp.c_str() , &folder_list);
	
		break;

	}

}

void FileExplorer::OnResize(int newx, int newy)
{

	MoveWindow(folders_tree.TreeView,	margin_x,	margin_x,	(newx/5)-margin_x*2,				newy - margin_x * 3,TRUE);

	MoveWindow(folder_list.ListBox,		newx / 5,	margin_x*2,	(1.5 * newx / 5) - margin_x*2,		newy - margin_x * 3, TRUE);
	MoveWindow(selected_file_in_folder, newx / 5, margin_x, (1.5 * newx / 5) - margin_x * 2, margin_x, TRUE);

	int butt_width = 100;
	int butt_height = 30;
	MoveWindow(button_add,			-10 +newx * 2.5 / 5,	(newy / 2) - butt_height ,	butt_width,		butt_height,TRUE);
	MoveWindow(button_rem,			-10 +newx * 2.5 / 5,	(newy / 2) + butt_height ,	butt_width,		butt_height, TRUE);

	MoveWindow(scene_list.ListBox,		butt_width + margin_x + newx*2.5/5,	margin_x*2,	(2.5 * newx / 5)-butt_width-(margin_x * 3),		newy - margin_x * 3, TRUE);
	MoveWindow(selected_file_in_scene,	butt_width + margin_x + newx * 2.5 / 5, margin_x, (2.5 * newx / 5) - butt_width - (margin_x * 3), margin_x, TRUE);
}



FoldInHier* FileExplorer::FindFoldInHierarchy(HTREEITEM item)
{
	if (folders.empty())
		return nullptr;

	for (FoldInHier* fih : folders)
	{
		if (fih->treeitem == item)
			return fih;
	}

	return nullptr;
}

void FileExplorer::DeleteFromHierarchy(int id)
{
	//for (auto it = objects.begin(); it != objects.end(); ) {
	//	if ((*it)->gameobjectid == id) 
	//	{
	//		folders.DeleteItem((*it)->treeitem);
	//		delete* it;
	//		it = objects.erase(it);
	//	}
	//	else 
	//	{
	//		++it;
	//	}
	//}
}


LRESULT CALLBACK FileExplorerProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR)
{
	switch (msg) {
	case WM_COMMAND:
		EditorWindow::Get().GetFileExplorer()->HandleCommands(hwnd, wParam, lParam);
		break;
	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, FileExplorerProc, 0);
		break;
	case WM_NOTIFY:
		EditorWindow::Get().GetFileExplorer()->HandleNotify(hwnd, wParam, lParam);
		break;
	}
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}