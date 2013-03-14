/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* env.c */
/* some load_* routines for special environments */

#include "glob.h"

/* loads the arena level into Level*/
void load_arena(void)
{
    int i,j;
    char site;
    Object* box=((Object*)checkmalloc(sizeof(Object)));
    map *glad; /* too glad to gladiate */

    *box = Objects[OB_GARAGE_OPENER];

    TempLevel = Level;
    if (ok_to_free(TempLevel)) {
        free_level(TempLevel);

        TempLevel = NULL;
    }

    Level = ((plv) checkmalloc(sizeof(levtype)));

    clear_level(Level);
    Level->environment = E_ARENA;

    glad = map_open(MAP_arena);
    map_setLevel(glad,0);

    Level->level_width = map_getWidth(glad);
    Level->level_length = map_getLength(glad);

    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            Level->site[i][j].lstatus = SEEN+LIT;
            Level->site[i][j].roomnumber = RS_ARENA;
            site = map_getSiteChar(glad,i,j);
            Level->site[i][j].p_locf = L_NO_OP;
            switch(site) {
            case 'P':
                Level->site[i][j].locchar = PORTCULLIS;
                Level->site[i][j].p_locf = L_PORTCULLIS;
                break;
            case 'X':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_ARENA_EXIT;
                break;
            case '#':
                Level->site[i][j].locchar = WALL;
                break;
            case '.':
                Level->site[i][j].locchar = FLOOR;
                break;
            }
            Level->site[i][j].showchar = Level->site[i][j].locchar;
        }
    }
    map_close(glad);
    Level->site[60][7].creature = Arena_Monster;
    Arena_Monster->x = 60;
    Arena_Monster->y = 7;
    Arena_Monster->sense = 50;
    Arena_Monster->m_pickup(box);
    m_status_set(Arena_Monster,AWAKE);
    Level->mlist = (MonsterList*) checkmalloc(sizeof(MonsterList));
    Level->mlist->monster = Arena_Monster;
    Level->mlist->next = NULL;
    /* hehehehe cackled the dungeon master.... */
    print2("Your opponent holds the only way you can leave!");
}

/* loads the sorcereror's circle into Level*/
void load_circle(int populate)
{
    int i,j;
    int safe = (Player.rank[CIRCLE] > 0);
    char site;
    map *nook; /* I'll never run out of 4-letter names! */

    TempLevel = Level;
    if (ok_to_free(TempLevel)) {
        free_level(TempLevel);

        TempLevel = NULL;
    }

    Level = ((plv) checkmalloc(sizeof(levtype)));

    clear_level(Level);
    Level->environment = E_CIRCLE;
    nook = map_open(MAP_circle);
    map_setLevel(nook,0);

    Level->level_width = map_getWidth(nook);
    Level->level_length = map_getLength(nook);

    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            Level->site[i][j].lstatus = 0;
            Level->site[i][j].roomnumber = RS_CIRCLE;
            Level->site[i][j].p_locf = L_NO_OP;
            site = map_getSiteChar(nook,i,j);
            switch(site) {
            case 'P':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_prime(i,j); /* prime sorceror */
                    Level->site[i][j].creature->specialf = M_SP_PRIME;
                    if (! safe) m_status_set(Level->site[i][j].creature,HOSTILE);
                }
                break;
            case 'D':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,DEMON_PRINCE); /* prime circle demon */
                    if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                }
                break;
            case 's':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,SERV_CHAOS); /* servant of chaos */
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                    if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
                }
                break;
            case 'e':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,ENCHANTOR); /* enchanter */
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                    if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
                }
                break;
            case 'n':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,NECROMANCER); /* necromancer */
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                    if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
                }
                break;
            case 'T':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,THAUMATURGIST); /* High Thaumaturgist */
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                    if (safe) m_status_reset(Level->site[i][j].creature,HOSTILE);
                }
                break;
            case '#':
                Level->site[i][j].locchar = WALL;
                Level->site[i][j].aux = 1000;
                break;
            case 'L':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_CIRCLE_LIBRARY;
                break;
            case '?':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_TOME1;
                break;
            case '!':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_TOME2;
                break;
            case 'S':
                Level->site[i][j].locchar = FLOOR;
                lset(i,j,SECRET);
                break;
            case '.':
                Level->site[i][j].locchar = FLOOR;
                break;
            case '-':
                Level->site[i][j].locchar = CLOSED_DOOR;
                break;
            }
        }
    }
    map_close(nook);
}

/* make the prime sorceror */
void make_prime(int i, int j)
{
    MonsterList* ml = ((MonsterList*) checkmalloc(sizeof(MonsterList)));
    Monster* m = ((Monster*) checkmalloc(sizeof(Monster)));
    pol ol;
    Object* o;
    make_hiscore_npc(m,10); /* 10 is index for prime */
    m->x = i;
    m->y = j;
    Level->site[i][j].creature = m;
    ml->monster = m;
    ml->next = Level->mlist;
    Level->mlist = ml;

    if (Objects[OB_STARGEM].uniqueness != UNIQUE_TAKEN) {
        ol  = ((pol) checkmalloc(sizeof(oltype)));
        o  = ((Object*) checkmalloc(sizeof(Object)));
        *o = Objects[OB_STARGEM];
        ol->thing = o;
        ol->next = NULL;
        m->possessions = ol;
    }
}


/* loads the court of the archmage into Level*/
void load_court(int populate)
{
    int i,j;
    char site;
    map *mage;

    TempLevel = Level;
    if (ok_to_free(TempLevel)) {
        free_level(TempLevel);

        TempLevel = NULL;
    }

    Level = ((plv) checkmalloc(sizeof(levtype)));

    clear_level(Level);
    Level->environment = E_COURT;
    mage = map_open(MAP_court);
    map_setLevel(mage,0);

    Level->level_width = map_getWidth(mage);
    Level->level_length = map_getLength(mage);

    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            Level->site[i][j].lstatus = 0;
            Level->site[i][j].roomnumber = RS_COURT;
            Level->site[i][j].p_locf = L_NO_OP;
            site = map_getSiteChar(mage,i,j);
            switch(site) {
            case '5':
                Level->site[i][j].locchar = CHAIR;
                Level->site[i][j].p_locf = L_THRONE;
                if (populate) {
                    make_specific_treasure(i,j,OB_SCEPTRE);
                    make_archmage(i,j);
                    m_status_reset(Level->site[i][j].creature,HOSTILE);
                    m_status_reset(Level->site[i][j].creature,MOBILE);
                }
                break;
            case 'e':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,ENCHANTOR); /* enchanter */
                    m_status_reset(Level->site[i][j].creature,HOSTILE);
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                }
                break;
            case 'n':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,NECROMANCER); /* necromancer */
                    m_status_reset(Level->site[i][j].creature,HOSTILE);
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                }
                break;
            case 'T':
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,THAUMATURGIST); /* High Thaumaturgist */
                    m_status_reset(Level->site[i][j].creature,HOSTILE);
                    Level->site[i][j].creature->specialf = M_SP_COURT;
                }
                break;
            case '#':
                Level->site[i][j].locchar = WALL;
                Level->site[i][j].aux = 1000;
                break;
            case 'G':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].locchar = FLOOR;
                if (populate) {
                    make_site_monster(i,j,GUARD); /* guard */
                    m_status_reset(Level->site[i][j].creature,HOSTILE);
                }
                break;
            case '<':
                Level->site[i][j].locchar = STAIRS_UP;
                Level->site[i][j].p_locf = L_ESCALATOR;
                break;
            case '.':
                Level->site[i][j].locchar = FLOOR;
                break;
            }
        }
    }
    map_close(mage);
}



/* make the archmage */
void make_archmage(int i, int j)
{
    MonsterList* ml = ((MonsterList*) checkmalloc(sizeof(MonsterList)));
    Monster* m = ((Monster*) checkmalloc(sizeof(Monster)));
    make_hiscore_npc(m,9); /* 9 is index for archmage */
    m->x = i;
    m->y = j;
    Level->site[i][j].creature = m;
    ml->monster = m;
    ml->next = Level->mlist;
    Level->mlist = ml;
    m->specialf = M_SP_COURT;
}

