#ifndef MESSAGES__H
#define MESSAGES__H

#include <list>

enum destinatary {
	ENGINE,
	EDITOR
};

enum MESSAGE
{
	GOT_FOCUS,

	//FROM EDITOR WINDOW TO ENGINE
	RESIZE_ENGINE_WINDOW,
	
	CREATE_GAME_OBJECT,
	UPDATE_GAME_OBJECT,
	DELETE_GAME_OBJECT,
	
	SAVE_SCENE,
	LOAD_SCENE,

	STOP_ENGINE,
	PLAY_ENGINE,
	PAUSE_ENGINE,

	UPDATE_RESOURCES,//SEND RES PATH


	//FROM ENGINE TO EDITOR WINDOW
	UPDATE_SCENE_VIEWER,

	UPDATE_RESOURCE_VIEWER,//SEND RES ID

	STOPPED_BUTTON,
	PLAYED_BUTTON,
	PAUSED_BUTTON,

	QUIT
};


//BASED
struct msg {
	destinatary dest;
	MESSAGE code;
};

//EDITOR >>>>>>>>>>>>>>>>>>>>>> ENGINE
struct resizemsg : msg {

	resizemsg() { code = RESIZE_ENGINE_WINDOW; }

	int x;
	int y;
	int w;
	int h;
};


struct scenemsg : msg {
	std::string path;
};

struct addobjmsg : msg {
	addobjmsg() { code = CREATE_GAME_OBJECT; }
	std::string name;
};

struct deleteobjmsg : msg {
	deleteobjmsg() { code = DELETE_GAME_OBJECT; }
	int to_delete;
};

struct updateresourcesmsg : msg {
	updateresourcesmsg() { code = UPDATE_RESOURCES; }
	std::string path;
	std::string fullpath;
};

//ENGINE >>>>>>>>>>>>>>>>>>>>>>>> EDITOR


struct updatesceneviewermsg : msg {
	updatesceneviewermsg() { code = UPDATE_SCENE_VIEWER; }
	int objectid;
};

struct updateresourceviewermsg : msg {
	updateresourceviewermsg() { code = UPDATE_RESOURCE_VIEWER; }
	int id;
};
#endif // !MESSAGES__H
