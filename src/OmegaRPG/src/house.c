/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* house.c */
/* some functions to make the house levels */

#include "glob.h"


/* loads the house level into Level*/
void load_house(int kind, int populate)
{
  map *home;
  int i,j;
  char site;
  int stops;
  
  TempLevel = Level;
  initrand(Current_Environment, Player.x + Player.y + hour()*10);
  if (ok_to_free(TempLevel)) {
#ifndef SAVE_LEVELS
    free_level(TempLevel);
#endif
    TempLevel = NULL;
  }
#ifndef SAVE_LEVELS
  Level = ((plv) checkmalloc(sizeof(levtype)));
#else
  msdos_changelevel(TempLevel,0,-1);
  Level = &TheLevel;
#endif
  clear_level(Level);
  switch(kind) {
  case E_HOUSE:
    home = map_open(MAP_house);
    break;
  case E_MANSION:
    home = map_open(MAP_mansion);
    break;
  default:
  case E_HOVEL:
    home = map_open(MAP_hovel);
    break;
  }
  Level->level_width=map_getWidth(home);
  Level->level_length=map_getLength(home);
  map_setLevel(home, 0);
  
  stops = 0;
  for(j=0;j<Level->level_length;j++) {
    for(i=0;i<Level->level_width;i++) {
      if (kind == E_HOVEL) Level->site[i][j].lstatus = SEEN;
      else Level->site[i][j].lstatus = 0;
      Level->site[i][j].roomnumber = RS_CORRIDOR;
      Level->site[i][j].p_locf = L_NO_OP;
      site = map_getSiteChar(home,i,j);
      switch(site) {
      case 'N':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BEDROOM;
	if (random_range(2) && populate) make_house_npc(i,j);
	break;
      case 'H':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BEDROOM;
	if (random_range(2) && populate) make_mansion_npc(i,j);
	break;
      case 'D':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_DININGROOM;
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	if (stops) {
	  lset(i,j,STOPS);
	  stops = 0;
	}
	break;
      case 'c':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CLOSET;
	break;
      case 'G':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BATHROOM;
	break;
      case 'B':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_BEDROOM;
	break;
      case 'K':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_KITCHEN;
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].showchar = WALL;
	lset(i,j,SECRET);
	Level->site[i][j].roomnumber = RS_SECRETPASSAGE;
	break;
      case '3':
	Level->site[i][j].locchar = SAFE;
	Level->site[i][j].showchar = WALL;
	lset(i,j,SECRET);
	Level->site[i][j].p_locf = L_SAFE;
	break;
      case '^':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
	break;
      case 'P':
	Level->site[i][j].locchar = PORTCULLIS;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'R':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_RAISE_PORTCULLIS;
	break;
      case 'p':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_PORTCULLIS;
	break;
      case 'T':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_HOUSE_EXIT;
	stops = 1;
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	switch (kind) {
	  case E_HOVEL: Level->site[i][j].aux = 10; break;
	  case E_HOUSE: Level->site[i][j].aux = 50; break;
	  case E_MANSION: Level->site[i][j].aux = 150; break;
	}
	break;
      case '|':
	Level->site[i][j].locchar = OPEN_DOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	lset(i,j,STOPS);
	break;
      case '+':
	Level->site[i][j].locchar = CLOSED_DOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	Level->site[i][j].aux = LOCKED;
	lset(i,j,STOPS);
	break;
      case 'd':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	if (populate)
	  make_site_monster(i,j,DOBERMAN);
	break;
      case 'a':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	Level->site[i][j].p_locf = L_TRAP_SIREN;
	break;
      case 'A':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].roomnumber = RS_CORRIDOR;
	if (populate)
	  make_site_monster(i,j,AUTO_MINOR); /* automaton */
	break;
      }
      Level->site[i][j].showchar = ' ';
    }
  }
  map_close(home);
  initrand(E_RESTORE, 0);
}


  
/* makes a log npc for houses and hovels */
void make_house_npc(int i, int j)
{
  pml ml = ((pml) checkmalloc(sizeof(mltype)));
  pob ob;
  ml->m = ((pmt) checkmalloc(sizeof(montype)));
  *(ml->m) = Monsters[NPC];
  make_log_npc(ml->m);
  if (ml->m->id == NPC) mprint("You detect signs of life in this house.");
  else mprint("An eerie shiver runs down your spine as you enter....");
  /* if not == NPC, then we got a ghost off the npc list */
  ml->m->x = i;
  ml->m->y = j;
  Level->site[i][j].creature = ml->m;
  ml->m->click = (Tick + 1) % 50;
  ml->next = Level->mlist;
  Level->mlist = ml;
  m_status_set(ml->m,HOSTILE);
  if (nighttime())
    m_status_reset(ml->m,AWAKE);
  else m_status_set(ml->m,AWAKE);
  if (ml->m->startthing > -1) {
    ob = ((pob) checkmalloc(sizeof(objtype)));    
    *ob = Objects[ml->m->startthing];
    m_pickup(ml->m,ob);
  }
}


/* makes a hiscore npc for mansions */
void make_mansion_npc(int i, int j)
{
  pml ml = ((pml) checkmalloc(sizeof(mltype)));
  ml->m = ((pmt) checkmalloc(sizeof(montype)));
  *(ml->m) = Monsters[NPC];
  make_hiscore_npc(ml->m,random_range(14) + 1);
  mprint("You detect signs of life in this house.");
  ml->m->x = i;
  ml->m->y = j;
  Level->site[i][j].creature = ml->m;
  ml->m->click = (Tick + 1) % 50;
  ml->next = Level->mlist;
  Level->mlist = ml;
  m_status_set(ml->m,HOSTILE);
  if (nighttime())
    m_status_reset(ml->m,AWAKE);
  else m_status_set(ml->m,AWAKE);
}

