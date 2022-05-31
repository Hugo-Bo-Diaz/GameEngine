#ifndef SCRIPTING__H
#define SCRIPTING__H

struct Script;

extern "C" {
#include "../lua/include/lua.h"
#include "../lua/include/lauxlib.h"
#include "../lua/include/lualib.h"
}

//#ifdef _WIN32
//#pragma comment(lib, "../lua/liblua54.a")
//#endif
//
//namespace Scripting
//{
//	void Initialize();
//	void ShutDown();
//	namespace {
//		lua_State* state;
//	}
//
//	Script* LoadScript(const char*);
//
//	void ExectuteFunction(const Script* scr, const char* function);
//
//};

#endif // !ENGINE__H
