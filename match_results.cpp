/*
 * match_result.cpp
 * std::cmatch的封装
 * MIT License
 */
#include <regex>
#include <lua.hpp>
#include "regex.hpp"

using namespace std;
using namespace regex_constants;

const char match_results_tname[]="std::cmatch";

namespace match_results{
	int __new(lua_State *L){
		void* ptr=lua_newuserdatauv(L,sizeof(std::cmatch),1);
		new(ptr)std::cmatch();
		luaL_setmetatable(L,match_results_tname);
		return 1;
	}
	int __gc(lua_State *L){
		((std::cmatch*)lua_touserdata(L,1))->std::cmatch::~cmatch();
		return 0;
	}
	int empty(lua_State *L){	// bool()
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		lua_pushboolean(L,cm->empty());
		return 1;
	}
	int size(lua_State *L){	// int()
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		lua_pushinteger(L,cm->size());
		return 1;
	}
	int length(lua_State *L){	// int({int})
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		luaL_argcheck(L,lua_isinteger(L,2)||lua_isnoneornil(L,2),1,nullptr);
		lua_pushinteger(L,cm->length(lua_tointeger(L,2)));
		return 1;
	}
	int position(lua_State *L){	// int({int})
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		luaL_argcheck(L,lua_isinteger(L,2)||lua_isnoneornil(L,2),2,nullptr);
		lua_pushinteger(L,cm->position(lua_tointeger(L,2))+1);
		return 1;
	}
	int index(lua_State *L){	// str operator[](int)
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		if(lua_isinteger(L,2)){
			const string&s=cm->str(lua_tointeger(L,2));
			lua_pushlstring(L,s.c_str(),s.length());
			return 1;
		}else return 0;
	}
	int str(lua_State *L){	// str operator[](int)
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		luaL_argcheck(L,lua_isinteger(L,2)||lua_isnoneornil(L,2),2,nullptr);
		const string&s=cm->str(lua_tointeger(L,2));
		lua_pushlstring(L,s.c_str(),s.length());
		return 1;
	}
	int prefix(lua_State *L){	// str()
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		const string&s=cm->prefix();
		lua_pushlstring(L,s.c_str(),s.length());
		return 1;
	}
	int suffix(lua_State *L){	// str()
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		const string&s=cm->suffix();
		lua_pushlstring(L,s.c_str(),s.length());
		return 1;
	}
	int format(lua_State *L){	// str(str fmt, match_flag_type/uint flag)
		auto cm=(std::cmatch*)luaL_checkudata(L,1,match_results_tname);
		const char *fmt=luaL_checkstring(L,2);
		auto flag=format_default;
		if(lua_isinteger(L,3))flag=(decltype(flag))lua_tointeger(L,3);
		const string &s=cm->format(fmt,flag);
		lua_pushlstring(L,s.c_str(),s.length());
		return 1;
	}

	static const luaL_Reg __indexTable[]={
			{"empty",empty},
			{"size",size},
			{"length",length},
			{"position",position},
			{"prefix",prefix},
			{"suffix",suffix},
			{"str",str},
			{"format",format},
			{nullptr,nullptr}
	};
	int pushTable__index(lua_State *L){
		luaL_newlib(L,__indexTable);
		return 1;
	}
	int __index(lua_State *L){
		if(index(L))return 1;
		lua_pushvalue(L,2);
		lua_gettable(L,lua_upvalueindex(1));
		return 1;
	}
	int create_metatable(lua_State *L){
		luaL_newmetatable(L,match_results_tname);

		pushTable__index(L);
		lua_pushcclosure(L,__index,1);
		lua_setfield(L,-2,"__index");

		lua_pushcfunction(L,__gc);
		lua_setfield(L,-2,"__gc");

		lua_pop(L,1);
		return 0;
	}
}