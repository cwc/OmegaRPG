#pragma once

typedef chtype Symbol;

#include "curses.h"

//namespace Omega {

	class Monster {
	public:
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
	};

	class MonsterList {
	public:
		Monster* m;
		MonsterList* next;
	};

//} // namespace Omega
