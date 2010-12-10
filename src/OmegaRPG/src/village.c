/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* village.c */
/* some functions to make the village levels */

#include "glob.h"


/* loads the village level into Level*/
void load_village(int villagenum, int populate)
{
  int i,j;
  char site;
  map *village = 0;
  
  TempLevel = Level;
  if (ok_to_free(TempLevel)) {
#ifndef SAVE_LEVELS
    free_level(TempLevel);
#endif
    TempLevel = NULL;
  }

  initrand(Current_Environment, villagenum);
  
  assign_village_function(0,0,TRUE);
  
#ifndef SAVE_LEVELS
  Level = ((plv) checkmalloc(sizeof(levtype)));
#else
  msdos_changelevel(TempLevel,0,-1);
  Level = &TheLevel;
#endif
  
  switch( villagenum )
    {
      case VIL_STARVIEW: village = map_open(MAP_starview); break;
      case VIL_WOODMERE: village = map_open(MAP_woodmere); break;
      case VIL_STORMWAT: village = map_open(MAP_stormwat); break;
      case VIL_THAUMARI: village = map_open(MAP_thaumari); break;
      case VIL_SKORCH  : village = map_open(MAP_skorch  ); break;
      case VIL_WHORFEN : village = map_open(MAP_whorfen ); break;
      default: abort();
    }
  map_setLevel(village,0);
  
  clear_level(Level);
  Level->environment = E_VILLAGE;
  Level->level_width = map_getWidth(village);
  Level->level_length = map_getLength(village);
  
  for(j=0;j<Level->level_length;j++) {
    for(i=0;i<Level->level_width;i++) {
      lset(i,j,SEEN);
      site = map_getSiteChar(village,i,j);
      Level->site[i][j].p_locf = L_NO_OP;
      switch(site) {
      case 'f':
	Level->site[i][j].locchar = FLOOR;
	if (populate)
	  make_food_bin(i,j);
	break;
      case 'g':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_GRANARY;
	break;
      case 'h':
	Level->site[i][j].locchar = FLOOR;
	if (populate)
	  make_horse(i,j);
	break;
      case 'S':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_STABLES;
	break;
      case 'H':
	Level->site[i][j].locchar = FLOOR;
	if (populate)
	  make_merchant(i,j);
	break;
      case 'C':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_COMMONS;
	break;
      case 's':
	Level->site[i][j].locchar = FLOOR;
	if (populate)
	  make_sheep(i,j);
	break;
      case 'x':
	assign_village_function(i,j,FALSE);
	break;
      case 'X':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_COUNTRYSIDE;
	break;
      case 'G':
	Level->site[i][j].locchar = FLOOR;
	if (populate) {
	  make_guard(i,j);
	  Level->site[i][j].creature->aux1 = i;
	  Level->site[i][j].creature->aux2 = j;
	}
	break;
      case '^':
	Level->site[i][j].locchar = FLOOR;
	Level->site[i][j].p_locf = L_TRAP_SIREN;
	break;
      case '"':
	Level->site[i][j].locchar = HEDGE;
	Level->site[i][j].p_locf = L_HEDGE;
	break;
      case '~':
	Level->site[i][j].locchar = WATER;
	Level->site[i][j].p_locf = L_WATER;
	break;
      case '+':
	Level->site[i][j].locchar = WATER;
	Level->site[i][j].p_locf = L_CHAOS;
	break;
      case '\'':
	Level->site[i][j].locchar = HEDGE;
	Level->site[i][j].p_locf = L_TRIFID;
	break;
      case '!':
	special_village_site(i,j,villagenum);
	break;
      case '#':
	Level->site[i][j].locchar = WALL;
	Level->site[i][j].aux = 100;
	break;
      case '.':
	Level->site[i][j].locchar = FLOOR;
	break;
      case '-':
	Level->site[i][j].locchar = CLOSED_DOOR;
	break;
      case '1':
	Level->site[i][j].locchar = STATUE;
	break;
      }
      if (loc_statusp(i,j,SECRET))
	Level->site[i][j].showchar = WALL;
      else Level->site[i][j].showchar = Level->site[i][j].locchar;
    }
  }
  map_close(village);
  initrand(E_RESTORE, 0);
}

void make_guard(int i, int j)
{
  pml tml = ((pml) (checkmalloc(sizeof(mltype))));
  tml->m = (Level->site[i][j].creature = make_creature(GUARD));
  tml->m->x = i;
  tml->m->y = j;
  tml->next = Level->mlist;
  Level->mlist = tml;
}

void make_sheep(int i, int j)
{
  pml tml = ((pml) (checkmalloc(sizeof(mltype))));
  tml->m = (Level->site[i][j].creature = make_creature(SHEEP));
  tml->m->x = i;
  tml->m->y = j;
  tml->next = Level->mlist;
  Level->mlist = tml;
}

void make_food_bin(int i, int j)
{ 
  pol tol;
  int k;

  for(k=0;k<10;k++) {
    tol = ((pol) checkmalloc(sizeof(oltype)));
    tol->thing = ((pob) checkmalloc(sizeof(objtype)));
    make_food(tol->thing,15); /* grain */
    tol->next = Level->site[i][j].things;
    Level->site[i][j].things = tol;
  }
}

void make_horse(int i, int j)
{
  pml tml = ((pml) (checkmalloc(sizeof(mltype))));
  tml->m = (Level->site[i][j].creature = make_creature(HORSE));
  tml->m->x = i;
  tml->m->y = j;
  tml->next = Level->mlist;
  Level->mlist = tml;
}


void make_merchant(int i, int j)
{
  pml tml = ((pml) (checkmalloc(sizeof(mltype))));
  tml->m = (Level->site[i][j].creature = make_creature(MERCHANT));
  tml->m->x = i;
  tml->m->y = j;
  tml->next = Level->mlist;
  Level->mlist = tml;
}


void assign_village_function(int x, int y, int setup)
{
  static int next=0;
  static int permutation[24]; /* number of x's in village map */
  int i,j,k;


  if (setup) {
    next = 0;
    for(i=0;i<24;i++)
      permutation[i] = i;
    for(i=0;i<24;i++) {
      j = permutation[i];
      k = random_range(24);
      permutation[i] = permutation[k];
      permutation[k] = j;
    }
  }
  else {

    lset(x,y+1,STOPS);
    lset(x+1,y,STOPS);
    lset(x-1,y,STOPS);
    lset(x,y-1,STOPS);
    lset(x,y,STOPS);

    
    switch(permutation[next++]) {
    case 0:
      Level->site[x][y].locchar = OPEN_DOOR;
      Level->site[x][y].p_locf = L_ARMORER;
      break;
    case 1:
      Level->site[x][y].locchar = OPEN_DOOR;
      Level->site[x][y].p_locf = L_HEALER;
      break;
    case 2: 
      Level->site[x][y].locchar = OPEN_DOOR;
      Level->site[x][y].p_locf = L_TAVERN;
      break;
    case 3:
      Level->site[x][y].locchar = OPEN_DOOR;
      Level->site[x][y].p_locf = L_COMMANDANT;
      break;
    case 4:
      Level->site[x][y].locchar = OPEN_DOOR;
      Level->site[x][y].p_locf = L_CARTOGRAPHER;
      break;
    default:
      Level->site[x][y].locchar = CLOSED_DOOR;
      if (random_range(2)) 
	Level->site[x][y].aux = LOCKED;
      if (random_range(2))
	Level->site[x][y].p_locf = L_HOVEL;
      else Level->site[x][y].p_locf = L_HOUSE;
      break;
    }
  }
}  
  

/* WDT HACK!  Remove these literals, replace with VIL_* constants. */
void special_village_site(int i, int j, int villagenum)
{
  if (villagenum == 1) {
    Level->site[i][j].locchar = ALTAR;
    Level->site[i][j].p_locf = L_LAWSTONE;
  }
  if (villagenum == 2) {
    Level->site[i][j].locchar = ALTAR;
    Level->site[i][j].p_locf = L_BALANCESTONE;
  }
  else if (villagenum == 3) {
    Level->site[i][j].locchar = ALTAR; 
    Level->site[i][j].p_locf = L_CHAOSTONE; 
  }
  else if (villagenum == 4) {
    Level->site[i][j].locchar = ALTAR; 
    Level->site[i][j].p_locf = L_MINDSTONE; 
  }
  else if (villagenum == 5) {
    Level->site[i][j].locchar = ALTAR; 
    Level->site[i][j].p_locf = L_SACRIFICESTONE; 
  }
  else if (villagenum == 6) {
    Level->site[i][j].locchar = ALTAR; 
    Level->site[i][j].p_locf = L_VOIDSTONE; 
  }
}
