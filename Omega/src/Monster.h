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
		const char* name;
		const char* corpseString;
		char* combatManeuvers;

		void m_pulse();
		void movemonster(int newx, int newy);
		void m_pickup( Object * );
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

		bool player_hit(int hitmod, char hitloc);

		void m_movefunction(int movef);

		void m_aggravate();
		void m_huge_sounds();
		void m_illusion();
		void m_sp_acid_cloud();
		void m_sp_angel();
		void m_sp_av();
		void m_sp_blackout();
		void m_sp_bogthing();
		void m_sp_court();
		void m_sp_demon();
		void m_sp_demonlover();
		void m_sp_dragonlord();
		void m_sp_eater();
		void m_sp_escape();
		void m_sp_explode();
		void m_sp_ghost();
		void m_sp_lair();
		void m_sp_lw();
		void m_sp_mb();
		void m_sp_merchant();
		void m_sp_mirror();
		void m_sp_mp();
		void m_sp_ng();
		void m_sp_poison_cloud();
		void m_sp_prime();
		void m_sp_raise();
		void m_sp_seductor();
		void m_sp_servant();
		void m_sp_spell();
		void m_sp_surprise();
		void m_sp_swarm();
		void m_sp_were();
		void m_sp_whistleblower();
		void m_summon();
		void m_thief_f();

		void m_talk_animal();
		void m_talk_archmage();
		void m_talk_assassin();
		void m_talk_beg();
		void m_talk_burble();
		void m_talk_demonlover();
		void m_talk_druid();
		void m_talk_ef();
		void m_talk_evil();
		void m_talk_gf();
		void m_talk_greedy();
		void m_talk_guard();
		void m_talk_hint();
		void m_talk_horse();
		void m_talk_hungry();
		void m_talk_hyena();
		void m_talk_im();
		void m_talk_man();
		void m_talk_merchant();
		void m_talk_mimsy();
		void m_talk_mp();
		void m_talk_ninja();
		void m_talk_parrot();
		void m_talk_prime();
		void m_talk_robot();
		void m_talk_scream();
		void m_talk_seductor();
		void m_talk_servant();
		void m_talk_silent();
		void m_talk_slithy();
		void m_talk_stupid();
		void m_talk_thief();
		void m_talk_titter();
		void m_talk_maharaja();

		void m_blind_strike();
		void m_fireball();
		void m_firebolt();
		void m_lball();
		void m_nbolt();
		void m_snowball();
		void m_strike_sonic();

		int monster_hit(char,int );
		void m_hit(int );
		void monster_melee(char,int );
		void tacmonster();
		void transcribe_monster_actions();
	};

	class MonsterList {
	public:
		Monster* monster;
		MonsterList* next;
	};

//} // namespace Omega
