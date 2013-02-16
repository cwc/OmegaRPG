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

		void m_pulse();
		void movemonster(int newx, int newy);
		void m_pickup(struct object*);
		void m_dropstuff();
		void m_damage(int dmg, int dtype);
		void m_remove();
		void m_death();
		void determine_npc_behavior(int, int);
		void m_abyss();
		void m_altar();
		void m_fire();
		void m_lava();
		void m_trap_abyss();
		void m_trap_acid();
		void m_trap_blade();
		void m_trap_dart();
		void m_trap_disintegrate();
		void m_trap_door();
		void m_trap_fire();
		void m_trap_manadrain();
		void m_trap_pit();
		void m_trap_sleepgas();
		void m_trap_snare();
		void m_trap_teleport();
		void m_water();
		void monster_action(int);
		void monster_move();
		void monster_special();
		void monster_strike();
		void monster_talk();
		void strengthen_death();

		void m_confused_move();
		void m_flutter_move();
		void m_follow_move();
		void m_move_animal();
		void m_normal_move();
		void m_random_move();
		void m_scaredy_move();
		void m_simple_move();
		void m_smart_move();
		void m_spirit_move();
		void m_teleport();
		void m_vanish();
		void m_move_leash();

		char* mstatus_string();
		void fight_monster();
		void surrender();
		void threaten();
	};

	class MonsterList {
	public:
		Monster* monster;
		MonsterList* next;
	};

//} // namespace Omega
