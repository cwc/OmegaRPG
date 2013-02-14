#pragma once

typedef chtype Symbol;

#include "curses.h"

//namespace Omega {

	class Monster {
	public:
		struct objectlist *possessions;
		bool wasAttackedByPlayer;
		int aux1;
		int aux2;
		int x;
		int y;
		int click;
		int id;
		int hp;
		int hit;
		int ac;
		int dmg;
		int sense;
		int wakeup;
		int level;
		int speed;
		unsigned char sleep;
		unsigned char treasure;
		long xpv;
		int corpseWeight;
		int corpseValue;
		int transformId;
		int startThing;
		unsigned char uniqueness;
		int talkf;
		int movef;
		int meleef;
		int strikef;
		int specialf;
		long status;
		long immunity;
		Symbol symbol;
		char* name;
		char* corpseString;
		char* combatManeuvers;
	};

	class MonsterList {
	public:
		Monster* m;
		MonsterList* next;
	};

//} // namespace Omega
