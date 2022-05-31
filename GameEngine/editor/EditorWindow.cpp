#include "EditorWindow.h"

#include "Dialog.h"
#include "../Globals.h"

#include "../engine/Engine.h"
#include "../engine/Graphics.h"

#include "../MessageManager.h"

#include "dialogs/SceneViewer.h"
#include "dialogs/ObjectInspector.h"
#include "dialogs/FileExplorer.h"
#include "dialogs/TopBar.h"

#include "ExtraWindows/ShortStringWindow.h"

#include <algorithm>

EditorWindow* EWind = nullptr;

EditorWindow& EditorWindow::Get()
{

	if (EWind == nullptr)
		EWind = new EditorWindow();

	return *EWind;
}

bool EditorWindow::RecieveEngineMessages()
{
	std::list<msg*> mess = MessageManager::Get().GetMessages(EDITOR);

	for (msg* m : mess)
	{
		switch (m->code)
		{
		case UPDATE_SCENE_VIEWER:
			if(GetSceneViewer() != nullptr)
				GetSceneViewer()->UpdateGameObjectHierarchy(((updatesceneviewermsg*)m)->objectid);
			break;
		case UPDATE_RESOURCE_VIEWER:
			if (GetFileExplorer() != nullptr)
				GetFileExplorer()->AddFileToSceneList(((updateresourceviewermsg*)m)->id);
			break;
		case QUIT:
			return false;
			break;
		case PLAYED_BUTTON:
		case PAUSED_BUTTON:
		case STOPPED_BUTTON:
			if(GetTopBar() != nullptr)
				GetTopBar()->HandleMessage(m);
			break;
		default:
			break;
		case GOT_FOCUS:
			if (GetTopBar() != nullptr)
				GetTopBar()->SetFocusText(false);
			break;
		}
	}

	MessageManager::DeleteMessagesFromList(&mess);

	return true;

}

void EditorWindow::SendQuitMessage()
{
	msg* m = new msg();

	m->code = QUIT;
	m->dest = EDITOR;

	MessageManager::Get().AddMessage(m);

}


void EditorWindow::OpenFileNameDialog(int action)
{
	ShortStringWindow* ssw = new ShortStringWindow();

	switch (action)
	{
	case 0:
		ssw->Initialize(L"New file name", Handler, SSWNew);
		break;
	case 1:
		ssw->Initialize(L"Save file", Handler, SSWSave);
		break;
	case 2:
		ssw->Initialize(L"Open file", Handler, SSWLoad);
		break;
	default:
		break;
	}



}

void EditorWindow::SSWNew(const char*, EditorWindow*)
{
}

//STATIC
void EditorWindow::SSWSave(const char* res, EditorWindow* win)
{

	scenemsg* sm = new scenemsg();

	sm->code = SAVE_SCENE;
	sm->dest = ENGINE;

	sm->path = res;

	MessageManager::Get().AddMessage(sm);

}

void EditorWindow::SSWLoad(const char* res, EditorWindow*)
{

	scenemsg* sm = new scenemsg();

	sm->code = LOAD_SCENE;
	sm->dest = ENGINE;

	sm->path = res;

	MessageManager::Get().AddMessage(sm);

}


Dialog* EditorWindow::GetDialog(DialogType type)
{

	for (Dialog* d : dialogs)
	{
		if (d->type == type)
			return d;
	}

	return nullptr;
}

SceneViewer* EditorWindow::GetSceneViewer()
{
	return (SceneViewer*)GetDialog(SCENE_VIEWER);
}

ObjectInspector* EditorWindow::GetObjectInspector()
{
	return (ObjectInspector*)GetDialog(OBJECT_INSPECTOR);
}

FileExplorer* EditorWindow::GetFileExplorer()
{
	return (FileExplorer*)GetDialog(FILE_EXPLORER);
}

TopBar* EditorWindow::GetTopBar()
{
	return (TopBar*)GetDialog(TOP_BAR);
}



void EditorWindow::SetEngineView(HWND view)
{
	EngineView = view;

	MoveWindow(EngineView, 0, 0, 640, 480, TRUE);
}

void EditorWindow::Initialize(HINSTANCE hInst)
{
	defaultClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	defaultClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	defaultClass.hInstance = hInst;
	defaultClass.lpszClassName = L"EditorWindow";
	defaultClass.lpfnWndProc = EditorWindowProcedure;

	int ret = RegisterClassW(&defaultClass);

	DWORD args = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	Handler = CreateWindowW(defaultClass.lpszClassName, L"Game Engine", args, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,NULL, NULL, NULL, NULL);

	width = 1280;
	height = 720;

	/*DWORD engargs =  WS_VISIBLE | WS_CHILD | WS_BORDER;
	EngineView = CreateWindowW(L"static", L"Game", engargs, 100, 100, 1280, 720, Handler, NULL, NULL, NULL);*/

	//ShowWindow(Handler, SW_MAXIMIZE);

	SceneViewer* d1 = new SceneViewer(L"HIERARCHY",250,LEFT,Handler);
	d1->priority = 1;

	ObjectInspector* d2 = new ObjectInspector(L"INSPECTOR", 250, RIGHT, Handler);
	d2->priority = 0;

	FileExplorer* d3 = new FileExplorer(L"FILE EXPLORER", 250, BOTTOM, Handler);
	d3->priority = 2;
	d3->FillFolderDirectory("",&d3->folders_tree);

	TopBar* d4 = new TopBar(L"TOP BAR", 40, TOP, Handler);
	d4->priority = 5;

	dialogs.push_back(d1);
	dialogs.push_back(d2);
	dialogs.push_back(d3);
	dialogs.push_back(d4);

	PlaceLayout();
	
}

void EditorWindow::AddMenu(HWND addto)
{

	HMENU FullMenu = CreateMenu();
	{
		HMENU FileMenu = CreateMenu();
		{
			AppendMenuW(FileMenu, MF_STRING, MB_FILE_NEW, L"New");
			AppendMenuW(FileMenu, MF_STRING, MB_FILE_OPEN, L"Load");
			AppendMenuW(FileMenu, MF_STRING, MB_FILE_SAVE, L"Save");
			AppendMenuW(FileMenu, MF_STRING, MB_FILE_SAVE, L"Save As");// TO DO
		}
		AppendMenuW(FullMenu, MF_POPUP,(UINT_PTR)FileMenu,L"File");

		HMENU EditMenu = CreateMenu();
		{
			AppendMenuW(EditMenu, MF_STRING, MB_EDIT_ADD_GO, L"Add GameObject");
			AppendMenuW(EditMenu, MF_STRING, MB_EDIT_REMOVE_GO, L"Remove GameObject");
		}
		AppendMenuW(FullMenu, MF_POPUP, (UINT_PTR)EditMenu, L"Edit");

		HMENU AssetsMenu = CreateMenu();
		{
			AppendMenuW(AssetsMenu, MF_STRING, MB_ASSETS_LOAD, L"Load Asset");
		}
		AppendMenuW(FullMenu, MF_POPUP, (UINT_PTR)AssetsMenu, L"Edit");

		AppendMenuW(FullMenu, MF_STRING, MB_HELP, L"Help");
	}
	SetMenu(addto, FullMenu);

}



void EditorWindow::OnResize(int newx, int newy)
{
	width = newx;
	height = newy;

	PlaceLayout();


}

int EditorWindow::GetMinWidth()
{
	int size = 0;

	for (Dialog* d : dialogs)
	{
		if (d->pos == LEFT || d->pos == RIGHT)
			size += d->min_size;
	}

	return size;
}

int EditorWindow::GetMinHeight()
{
	int size = 0;

	for (Dialog* d : dialogs)
	{
		if (d->pos == TOP || d->pos == BOTTOM)
			size += d->min_size;
	}
	return size;
}

void EditorWindow::PlaceLayout()
{

	std::sort(dialogs.begin(), dialogs.begin() + dialogs.size(), comparePriority);

	//place editor view through all window


	std::vector<Dialog*> placedwindows;

	for (Dialog* d : dialogs)
	{
		switch (d->pos)
		{
		case LEFT:

			d->SetDialogPos(0, 0, d->min_size, height);

			for (Dialog* placed : placedwindows)
			{
				if (placed->pos != RIGHT)
				{
					placed->SetDialogPos(placed->x + d->min_size,
						placed->y,
						std::max(placed->width - d->min_size, 0),
						placed->height);
				}
			}

			placedwindows.push_back(d);

			break;
		case RIGHT:

			d->SetDialogPos(width - d->min_size, 0, d->min_size, height);

			for (Dialog* placed : placedwindows)
			{
				if (placed->pos != LEFT)
				{
					placed->SetDialogPos(std::max(placed->x - d->min_size, 0),
						placed->y,
						std::max(placed->width - d->min_size, 0),
						placed->height);
				}
			}

			placedwindows.push_back(d);

			break;
		case TOP:

			d->SetDialogPos(0, 0, width, d->min_size);

			for (Dialog* placed : placedwindows)
			{
				if (placed->pos != BOTTOM)
				{
					placed->SetDialogPos(placed->x,
						placed->y + d->min_size,
						placed->width,
						std::max(placed->height - d->min_size, 0));
				}
			}

			placedwindows.push_back(d);

			break;
		case BOTTOM:

			d->SetDialogPos(0, height - d->min_size, width, d->min_size);

			for (Dialog* placed : placedwindows)
			{
				if (placed->pos != TOP)
				{
					placed->SetDialogPos(placed->x,
						std::max(placed->y - d->min_size, 0),
						placed->width,
						placed->height - d->min_size);
				}
			}

			placedwindows.push_back(d);

			break;
		default:
			break;
		}
	}

	int x_eng = 0;
	int y_eng = 0;
	int width_eng = width;
	int height_eng = height;

	for (Dialog* d : dialogs)
	{
		switch (d->pos)
		{
		case LEFT:
			x_eng += d->min_size;
			width_eng -= d->min_size;
			break;
		case RIGHT:
			width_eng -= d->min_size;
			break;
		case TOP:
			y_eng += d->min_size;
			height_eng -= d->min_size;
			break;
		case BOTTOM:
			height_eng -= d->min_size;
		default:
			break;
		}
	}
	//Engine::Get().gr->MoveEngineWindow(x_eng, y_eng, width_eng, height_eng);
	//MoveWindow(EngineView, x_eng, y_eng, width_eng, height_eng, TRUE);


	resizemsg* m = new resizemsg();
	m->code = RESIZE_ENGINE_WINDOW;
	m->dest = ENGINE;
	m->x = x_eng;
	m->y = y_eng;
	m->w = width_eng;
	m->h = height_eng;
	MessageManager::Get().AddMessage(m);


}







void EditorWindow::ShutDown()
{
	for (Dialog* d : dialogs)
	{
		delete d;
	}
	dialogs.clear();
	UnregisterClassW(defaultClass.lpszClassName, defaultClass.hInstance);
}




LRESULT CALLBACK EditorWindowProcedure(HWND hwnd, UINT MSG, WPARAM wPar, LPARAM lPar) {

	switch (MSG)
	{
	case WM_CREATE:
		EditorWindow::Get().AddMenu(hwnd);

		break;

	case WM_KEYDOWN:
		//TO DO

		switch (wPar)
		{
		case VK_LEFT:
			MessageBox(NULL, L"kekw", L"kekw", MB_OK);
			break;
		default:
			break;
		}

		break;

	case WM_COMMAND:
		switch (wPar)
		{
		case MB_FILE_SAVE:
			EditorWindow::Get().OpenFileNameDialog(1);
			break;
		case MB_FILE_OPEN:
			EditorWindow::Get().OpenFileNameDialog(2);
			break;
		default:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		EditorWindow::Get().SendQuitMessage();
		break;
	case WM_SIZE:
		EditorWindow::Get().OnResize(LOWORD(lPar),HIWORD(lPar));
		break;
	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpMMI = (LPMINMAXINFO)lPar;
			lpMMI->ptMinTrackSize.x = EditorWindow::Get().GetMinWidth();
			lpMMI->ptMinTrackSize.y = EditorWindow::Get().GetMinHeight();
		}
		break;
	case WM_SETFOCUS:
		if (EditorWindow::Get().GetTopBar() != nullptr)
			EditorWindow::Get().GetTopBar()->SetFocusText(true);
		break;
	case WM_NOTIFY:
		//break;
	default:
		return DefWindowProc(hwnd, MSG, wPar, lPar);
		break;
	}

}
