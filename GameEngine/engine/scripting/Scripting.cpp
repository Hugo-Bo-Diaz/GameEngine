#include "Scripting.h"
#include "EngineAPI.h"

#include "../Resources.h"

#include <string>
#include <fstream>
#include <sstream>



//
//void Scripting::Initialize()
//{
//	state = luaL_newstate();
//}
//
//void Scripting::ShutDown()
//{
//	lua_close(state);
//}
//
//Script* Scripting::LoadScript(const char* file)
//{
//	Script* newscript = new Script();
//
//	std::ifstream filestream;
//	std::stringstream stringstream;
//
//	filestream.open(file);
//
//	stringstream << filestream.rdbuf();
//
//	newscript->script = stringstream.str();
//
//	if (newscript->script == "")
//	{
//		delete newscript;
//		return nullptr;
//	}
//
//}
//
//void Scripting::ExectuteFunction(const Script* scr, const char* function)
//{
//}
