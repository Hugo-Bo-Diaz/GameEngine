#ifndef GLOBALS__H
#define GLOBALS__H
//if engine is on edit mode (1 edit on; 0 edit off)
#define EDIT_MODE 1


//DEFINE ALL BUTTONS IN THE APPLICATION 

//hierarchy
#define BUT_ADD_OBJECT 101
#define BUT_REM_OBJECT 102

//file explorer
#define BUT_ADD_RESOURCE 201
#define BUT_REM_RESOURCE 202

#define FOLDER_LIST_CB 203
#define SCENE_LIST_CB 204

//object inspector
#define BUT_ADD_COMPONENT 301
#define BUT_OPEN_SPRITE 302

//top bar
#define BUT_PLAY 400
#define BUT_PAUSE 401
#define BUT_STOP 402

//MENU BAR
#define MB_FILE 1000
	#define MB_FILE_NEW 1001
	#define MB_FILE_OPEN 1002
	#define MB_FILE_SAVE 1003

#define MB_EDIT 2000
	#define MB_EDIT_ADD_GO 2001
	#define MB_EDIT_REMOVE_GO 2002

#define MB_ASSETS 3000
	#define MB_ASSETS_LOAD 3001

#define MAIN_BAR_HELP 4000

//short string window
#define SSW_OKEY 500
#define SSW_CANCEL 501

//sprite showcase window
#define SPR_OKEY 600
#define SPR_CANCEL 601
#define SPR_APPLY 602

#define SPR_SET_START 603
#define SPR_SET_END 604

#define SPR_STRIDE_C 605

//
//#ifdef _WIN32
//	
//#elif _WIN64
//
//#elif __APPLE__ || __MACH__
//
//#elif __linux__
//
//#elif __FreeBSD__
//
//#elif __unix || __unix__
//
//#else
//
//#endif



#endif
