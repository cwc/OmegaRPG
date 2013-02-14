#pragma once

#ifdef __cplusplus 
extern "C" {
#endif

typedef chtype Symbol;

#include "curses.h"

#ifdef __cplusplus
namespace Omega {
	class monster {
#else
	typedef struct monster {
#endif
		struct objectlist *possessions;
		unsigned char attacked;
		int aux1,aux2,x,y,click;
		int id,hp,hit,ac,dmg,sense,wakeup,level,speed;
		unsigned char sleep,treasure;
		long xpv;
		int corpseweight,corpsevalue,transformid,startthing;
		unsigned char uniqueness;
		int talkf,movef,meleef,strikef,specialf;
		long status,immunity;
		Symbol monchar;
		char *monstring,*corpsestr,*meleestr;
#ifdef __cplusplus
	};
#else
	} Monster;
#endif

#ifdef __cplusplus
	class monsterlist {
#else
	typedef struct monsterlist {
#endif
		struct monster* m;
		struct monsterlist* next;
#ifdef __cplusplus
	};
#else
	} MonsterList;
#endif

#ifdef __cplusplus
} // namespace Omega
} // extern "C"
#endif
