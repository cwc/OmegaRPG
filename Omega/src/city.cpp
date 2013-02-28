/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* city.c */
/* some functions to make the city level */

#include "glob.h"

void mazesite( char,int,int,int );
extern int WIDTH, LENGTH;
void randommazesite( int,int,int );

/* loads the city level */
void load_city(int populate)
{
    int i,j;
    MonsterList* ml;
    char site;
    map *city;
    map *maze;
	Village village;



    initrand(E_CITY, 0);

    /* Get the hedge maze (which is in the city). */
    maze = map_open(MAP_hedges);
    /* Choose a random level from it. */
    map_setLevel(maze, random_range(map_getDepth(maze)));

    /* Get the city. */
    city = map_open(MAP_city);
    map_setLevel(city,0);

    TempLevel = Level;
    if (ok_to_free(TempLevel)) {
        free_level(TempLevel);
        TempLevel = NULL;
    }

    Level = ((plv) checkmalloc(sizeof(levtype)));

    clear_level(Level);
    Level->depth = 0;
    Level->environment = E_CITY;

    /* WDT: Rampart, as a special case, has its width stored in a
    * global variable. */
    WIDTH = Level->level_width = map_getWidth(city);
    LENGTH = Level->level_length = map_getLength(city);

    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            lset(i,j,SEEN);
            site = map_getSiteChar(city,i,j);
            switch(site) {
                /*
                case '@':
                Player.x = i; Player.y = j;
                Level->site[i][j].locchar = FLOOR;
                break;
                */
            case 'g':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_GARDEN;
                break;
            case 'y':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_CEMETARY;
                break;
            case 'p': /* WDT: each of these places needs to be assigned
					  * a function (or replaced with an 'x' in the map)
					  */
            case '!':
            case 'I':
            case 'E':
            case 'e':
            case 'x':
                assign_city_function(i,j);
                break;
            case 't':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_TEMPLE;
                CitySiteList[L_TEMPLE-CITYSITEBASE][0] = true;
                CitySiteList[L_TEMPLE-CITYSITEBASE][1] = i;
                CitySiteList[L_TEMPLE-CITYSITEBASE][2] = j;
                break;
#if 0 /* WDT: HACK!  The new city doesn't use portcullis traps, but has other
                * uses for 'T'.  Um...  I'd rather have a use for them (that's what
                * the jail is supposed to be), so this will stay only for now; with
                    * any luck we'll have things fixed up before the next release. */
                    case 'T':
                    Level->site[i][j].locchar = FLOOR;
                    Level->site[i][j].p_locf = L_PORTCULLIS_TRAP;
                    Level->site[i][j].aux = NOCITYMOVE;
                    break;
#endif /* end of hack */
                    case 'R':
                    Level->site[i][j].locchar = FLOOR;
                    Level->site[i][j].p_locf = L_RAISE_PORTCULLIS;
                    Level->site[i][j].aux = NOCITYMOVE;
                    break;
                    case '7':
                    Level->site[i][j].locchar = FLOOR;
                    Level->site[i][j].p_locf = L_PORTCULLIS;
                    Level->site[i][j].aux = NOCITYMOVE;
                    break;
                    case 'C':
                    Level->site[i][j].locchar = OPEN_DOOR;
                    Level->site[i][j].p_locf = L_COLLEGE;
                    CitySiteList[L_COLLEGE-CITYSITEBASE][0] = true;
                    CitySiteList[L_COLLEGE-CITYSITEBASE][1] = i;
                    CitySiteList[L_COLLEGE-CITYSITEBASE][2] = j;
                    break;
                    case 's':
                    Level->site[i][j].locchar = OPEN_DOOR;
                    Level->site[i][j].p_locf = L_SORCERORS;
                    CitySiteList[L_SORCERORS-CITYSITEBASE][0] = true;
                    CitySiteList[L_SORCERORS-CITYSITEBASE][1] = i;
                    CitySiteList[L_SORCERORS-CITYSITEBASE][2] = j;
                    break;
                    case 'M':
                    Level->site[i][j].locchar = OPEN_DOOR;
                    Level->site[i][j].p_locf = L_MERC_GUILD;
                    CitySiteList[L_MERC_GUILD-CITYSITEBASE][0] = true;
                    CitySiteList[L_MERC_GUILD-CITYSITEBASE][1] = i;
                    CitySiteList[L_MERC_GUILD-CITYSITEBASE][2] = j;
                    break;
                    case 'K':
                    Level->site[i][j].locchar = OPEN_DOOR;
                    Level->site[i][j].p_locf = L_MONASTERY;
                    CitySiteList[L_MONASTERY-CITYSITEBASE][0] = true;
                    CitySiteList[L_MONASTERY-CITYSITEBASE][1] = i;
                    CitySiteList[L_MONASTERY-CITYSITEBASE][2] = j;
                    break;
                    case 'c':
                    Level->site[i][j].locchar = OPEN_DOOR;
                    Level->site[i][j].p_locf = L_CASTLE;
                    CitySiteList[L_CASTLE-CITYSITEBASE][0] = true;
                    CitySiteList[L_CASTLE-CITYSITEBASE][1] = i;
                    CitySiteList[L_CASTLE-CITYSITEBASE][2] = j;
                    break;
                    case '?':
            {
                static int myI = -1, myJ;
                char site;
                if ( myI == -1 )
            {
                /* If this is the first time we've seen the hedge maze,
                * set this as its corner. */
                myI = i;
                myJ = j;
            }
    site = map_getSiteChar(maze, i - myI, j - myJ);
        mazesite(site,i,j,populate);
    }
    break;
    case 'P':
        Level->site[i][j].locchar = OPEN_DOOR;
        Level->site[i][j].p_locf = L_ORDER;
        CitySiteList[L_ORDER-CITYSITEBASE][0] = true;
        CitySiteList[L_ORDER-CITYSITEBASE][1] = i;
        CitySiteList[L_ORDER-CITYSITEBASE][2] = j;
        break;
    case 'H':
        Level->site[i][j].locchar = OPEN_DOOR;
        Level->site[i][j].p_locf = L_CHARITY;
        CitySiteList[L_CHARITY-CITYSITEBASE][0] = true;
        CitySiteList[L_CHARITY-CITYSITEBASE][1] = i;
        CitySiteList[L_CHARITY-CITYSITEBASE][2] = j;
        break;
    case 'h':
        Level->site[i][j].locchar = FLOOR;
        if (populate)
		{
            village.make_horse(i,j); /* from village.c */
		}
        break;
    case 'j':
        Level->site[i][j].locchar = FLOOR;
        if (populate)
            make_justiciar(i,j);
        break;
    case 'J':
        Level->site[i][j].locchar = CLOSED_DOOR;
        Level->site[i][j].p_locf = L_JAIL;
        break;
    case 'A':
        Level->site[i][j].locchar = OPEN_DOOR;
        Level->site[i][j].p_locf = L_ARENA;
        CitySiteList[L_ARENA-CITYSITEBASE][0] = true;
        CitySiteList[L_ARENA-CITYSITEBASE][1] = i;
        CitySiteList[L_ARENA-CITYSITEBASE][2] = j;
        break;
    case 'B':
        Level->site[i][j].locchar = OPEN_DOOR;
        Level->site[i][j].p_locf = L_BANK;
        CitySiteList[L_BANK-CITYSITEBASE][0] = true;
        CitySiteList[L_BANK-CITYSITEBASE][1] = i;
        CitySiteList[L_BANK-CITYSITEBASE][2] = j;
        lset(i,j,STOPS);
        lset(i,j+1,STOPS);
        lset(i+1,j,STOPS);
        lset(i-1,j,STOPS);
        lset(i,j-1,STOPS);
        break;
    case 'i':
        Level->site[i][j].locchar = OPEN_DOOR;
        Level->site[i][j].p_locf = L_TOURIST;
        CitySiteList[L_TOURIST-CITYSITEBASE][1] = i;
        CitySiteList[L_TOURIST-CITYSITEBASE][2] = j;
        lset(i,j,STOPS);
        lset(i,j+1,STOPS);
        lset(i+1,j,STOPS);
        lset(i-1,j,STOPS);
        lset(i,j-1,STOPS);
        break;
    case 'X':
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].p_locf = L_COUNTRYSIDE;
        CitySiteList[L_COUNTRYSIDE-CITYSITEBASE][0] = true;
        CitySiteList[L_COUNTRYSIDE-CITYSITEBASE][1] = i;
        CitySiteList[L_COUNTRYSIDE-CITYSITEBASE][2] = j;
        break;
    case 'v':
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].p_locf = L_VAULT;
        Level->site[i][j].aux = NOCITYMOVE;
        lset(i,j,SECRET);
        break;
    case 'S':
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].aux = NOCITYMOVE;
        lset(i,j,SECRET);
        break;
    case 'G':
        Level->site[i][j].locchar = FLOOR;
        if (populate) {
            make_site_monster(i,j,GUARD);
            Level->site[i][j].creature->aux1 = i;
            Level->site[i][j].creature->aux2 = j;
        }
        break;
    case 'u':
        Level->site[i][j].locchar = FLOOR;
        if (populate)
            make_minor_undead(i,j);
        break;
    case 'U':
        Level->site[i][j].locchar = FLOOR;
        if (populate)
            make_major_undead(i,j);
        break;
    case 'V':
        Level->site[i][j].showchar = WALL;
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].p_locf = L_VAULT;
        if (populate)
            make_site_treasure(i,j,5);
        Level->site[i][j].aux = NOCITYMOVE;
        lset(i,j,SECRET);
        break;
    case '%':
        Level->site[i][j].showchar = WALL;
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].p_locf = L_TRAP_SIREN;
        if (populate)
            make_site_treasure(i,j,5);
        Level->site[i][j].aux = NOCITYMOVE;
        lset(i,j,SECRET);
        break;
    case '$':
        Level->site[i][j].locchar = FLOOR;
        if (populate)
            make_site_treasure(i,j,5);
        break;
    case '2':
        Level->site[i][j].locchar = ALTAR;
        Level->site[i][j].p_locf = L_ALTAR;
        Level->site[i][j].aux = ODIN;
        break;
    case '3':
        Level->site[i][j].locchar = ALTAR;
        Level->site[i][j].p_locf = L_ALTAR;
        Level->site[i][j].aux = SET;
        break;
    case '4':
        Level->site[i][j].locchar = ALTAR;
        Level->site[i][j].p_locf = L_ALTAR;
        Level->site[i][j].aux = ATHENA;
        break;
    case '5':
        Level->site[i][j].locchar = ALTAR;
        Level->site[i][j].p_locf = L_ALTAR;
        Level->site[i][j].aux = HECATE;
        break;
    case '6':
        Level->site[i][j].locchar = ALTAR;
        Level->site[i][j].p_locf = L_ALTAR;
        Level->site[i][j].aux = DESTINY;
        break;
    case '^':
        Level->site[i][j].showchar = WALL;
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS);
        lset(i,j,SECRET);
        break;
    case '"':
        Level->site[i][j].locchar = HEDGE;
        break;
    case '~':
        Level->site[i][j].locchar = WATER;
        Level->site[i][j].p_locf = L_WATER;
        break;
    case '=':
        Level->site[i][j].locchar = WATER;
        Level->site[i][j].p_locf = L_MAGIC_POOL;
        break;
    case '*':
        Level->site[i][j].locchar = WALL;
        Level->site[i][j].aux = 10;
        break;
    case '#':
        Level->site[i][j].locchar = WALL;
        Level->site[i][j].aux = 500;
        break;
    case 'T':/* currently meaningless in large city map. */
    case '>':  /* currently meaningless outside of maze.*/
    case '.':
        Level->site[i][j].locchar = FLOOR;
        break;
    case ',':
        Level->site[i][j].showchar = WALL;
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].aux = NOCITYMOVE;
        lset(i,j,SECRET);
        break;
    case '-':
    case 'D': /* WDT: should all Ds be changed to -, or should D be given
					  * special treatment? */
        Level->site[i][j].locchar = CLOSED_DOOR;
        break;
    case '1':
        Level->site[i][j].locchar = STATUE;
        break;
    default:
        printf("\nOops... missed a case [%d,%d]: '%c' (%d)\n", i,j,site,site);
        morewait();
    }

    if (loc_statusp(i,j,SEEN)) {
        if (loc_statusp(i,j,SECRET))
            Level->site[i][j].showchar = WALL;
        else Level->site[i][j].showchar = Level->site[i][j].locchar;
    }
}
}
map_close(maze);
map_close(city);
City = Level;

       /* make all city monsters asleep, and shorten their wakeup range to 2 */
       /* to prevent players from being molested by vicious monsters on */
       /* the streets */
for(ml=Level->mlist; ml!=NULL; ml=ml->next) {
    m_status_reset(ml->monster,AWAKE);
    ml->monster->wakeup = 2;
}
initrand(E_RESTORE, 0);
}


void assign_city_function(int x, int y)
{
    static int setup=0;
    static int next=0;
    static int permutation[64]; /* number of x's in city map */
    int i,j,k,l;

    Level->site[x][y].aux = true;

    lset(x,y+1,STOPS);
    lset(x+1,y,STOPS);
    lset(x-1,y,STOPS);
    lset(x,y-1,STOPS);
    lset(x,y,STOPS); /* FIXED! 12/30/98 */

    if (setup == 0) {
        setup = 1;
        for(i=0; i<64; i++)
            permutation[i] = i;
        for(i=0; i<500; i++) {
            j = random_range(64);
            k = random_range(64);
            l = permutation[j];
            permutation[j] = permutation[k];
            permutation[k] = l;
        }
    }
    if (next > 63) { /* in case someone changes the no. of x's */
        Level->site[x][y].locchar = CLOSED_DOOR;
        Level->site[x][y].p_locf = L_HOUSE;
        if(random_range(5)) Level->site[x][y].aux = LOCKED;
    }
    else switch(permutation[next]) {
        case 0:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_ARMORER;
            CitySiteList[L_ARMORER-CITYSITEBASE][1] = x;
            CitySiteList[L_ARMORER-CITYSITEBASE][2] = y;
            break;
        case 1:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_CLUB;
            CitySiteList[L_CLUB-CITYSITEBASE][1] = x;
            CitySiteList[L_CLUB-CITYSITEBASE][2] = y;
            break;
        case 2:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_GYM;
            CitySiteList[L_GYM-CITYSITEBASE][1] = x;
            CitySiteList[L_GYM-CITYSITEBASE][2] = y;
            break;
        case 3:
            Level->site[x][y].locchar = CLOSED_DOOR;
            Level->site[x][y].p_locf = L_THIEVES_GUILD;
            CitySiteList[L_THIEVES_GUILD-CITYSITEBASE][1] = x;
            CitySiteList[L_THIEVES_GUILD-CITYSITEBASE][2] = y;
            break;
        case 4:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_HEALER;
            CitySiteList[L_HEALER-CITYSITEBASE][1] = x;
            CitySiteList[L_HEALER-CITYSITEBASE][2] = y;
            break;
        case 5:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_CASINO;
            CitySiteList[L_CASINO-CITYSITEBASE][1] = x;
            CitySiteList[L_CASINO-CITYSITEBASE][2] = y;
            break;
        case 7:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_DINER;
            CitySiteList[L_DINER-CITYSITEBASE][1] = x;
            CitySiteList[L_DINER-CITYSITEBASE][2] = y;
            break;
        case 8:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_CRAP;
            CitySiteList[L_CRAP-CITYSITEBASE][1] = x;
            CitySiteList[L_CRAP-CITYSITEBASE][2] = y;
            break;
        case 6:
        case 9:
        case 20:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_COMMANDANT;
            CitySiteList[L_COMMANDANT-CITYSITEBASE][1] = x;
            CitySiteList[L_COMMANDANT-CITYSITEBASE][2] = y;
            break;
        case 21:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_TAVERN;
            CitySiteList[L_TAVERN-CITYSITEBASE][1] = x;
            CitySiteList[L_TAVERN-CITYSITEBASE][2] = y;
            break;
        case 10:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_ALCHEMIST;
            CitySiteList[L_ALCHEMIST-CITYSITEBASE][1] = x;
            CitySiteList[L_ALCHEMIST-CITYSITEBASE][2] = y;
            break;
        case 11:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_DPW;
            CitySiteList[L_DPW-CITYSITEBASE][1] = x;
            CitySiteList[L_DPW-CITYSITEBASE][2] = y;
            break;
        case 12:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_LIBRARY;
            CitySiteList[L_LIBRARY-CITYSITEBASE][1] = x;
            CitySiteList[L_LIBRARY-CITYSITEBASE][2] = y;
            break;
        case 13:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_PAWN_SHOP;
            CitySiteList[L_PAWN_SHOP-CITYSITEBASE][1] = x;
            CitySiteList[L_PAWN_SHOP-CITYSITEBASE][2] = y;
            break;
        case 14:
            Level->site[x][y].locchar = OPEN_DOOR;
            Level->site[x][y].p_locf = L_CONDO;
            CitySiteList[L_CONDO-CITYSITEBASE][1] = x;
            CitySiteList[L_CONDO-CITYSITEBASE][2] = y;
            break;
        case 15:
            Level->site[x][y].locchar = CLOSED_DOOR;
            Level->site[x][y].p_locf = L_BROTHEL;
            CitySiteList[L_BROTHEL-CITYSITEBASE][1] = x;
            CitySiteList[L_BROTHEL-CITYSITEBASE][2] = y;
            break;
        default:
            Level->site[x][y].locchar = CLOSED_DOOR;
            switch(random_range(6)) {
            case 0:
                Level->site[x][y].p_locf = L_HOVEL;
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                Level->site[x][y].p_locf = L_HOUSE;
                break;
            case 5:
                Level->site[x][y].p_locf = L_MANSION;
                break;
            }
            if(random_range(5)) Level->site[x][y].aux = LOCKED;
            break;
        }
    next++;
}




/* makes a hiscore npc for mansions */
void make_justiciar(int i, int j)
{
    MonsterList* ml = ((MonsterList*) checkmalloc(sizeof(MonsterList)));
    ml->monster = ((Monster*) checkmalloc(sizeof(Monster)));
    *(ml->monster) = Monsters[NPC];
    make_hiscore_npc(ml->monster,15);
    ml->monster->x = i;
    ml->monster->y = j;
    Level->site[i][j].creature = ml->monster;
    ml->monster->click = (Tick + 1) % 60;
    ml->next = Level->mlist;
    Level->mlist = ml;
    m_status_reset(ml->monster,AWAKE);
}



/* loads the city level */
void resurrect_guards(void)
{
    int i,j;
    char site;

    FILE *fd;

    strcpy(Str3,Omegalib);
    strcat(Str3,"city.dat");
    fd = checkfopen(Str3,"rb");
    site = cryptkey("city.dat");
    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            site = getc(fd)^site;
            if (site == 'G') {
                make_site_monster(i,j,GUARD);
                Level->site[i][j].creature->name = "undead guardsman";
                Level->site[i][j].creature->meleef = M_MELEE_SPIRIT;
                Level->site[i][j].creature->movef = M_MOVE_SPIRIT;
                Level->site[i][j].creature->strikef = M_STRIKE_MISSILE;
                Level->site[i][j].creature->immunity = EVERYTHING-pow2(NORMAL_DAMAGE);
                Level->site[i][j].creature->hp *= 2;
                Level->site[i][j].creature->hit *= 2;
                Level->site[i][j].creature->dmg *= 2;
                Level->site[i][j].creature->ac *= 2;
                m_status_set(Level->site[i][j].creature,HOSTILE);
                m_status_set(Level->site[i][j].creature,AWAKE);
            }
        }
        site = getc(fd)^site;
    }
    fclose(fd);
}


void mazesite(char site, int i, int j, int populate)
{
    switch(site) {
    case '"':
        Level->site[i][j].locchar = HEDGE;
        if (random_range(10))
            Level->site[i][j].p_locf = L_HEDGE;
        else
            Level->site[i][j].p_locf = L_TRIFID;
        break;
    case '-':
        Level->site[i][j].locchar = CLOSED_DOOR;
        break;
    case '.':
        Level->site[i][j].locchar = FLOOR;
        break;
    case '>':
        Level->site[i][j].locchar = STAIRS_DOWN;
        Level->site[i][j].p_locf = L_SEWER;
        CitySiteList[L_SEWER-CITYSITEBASE][1] = i;
        CitySiteList[L_SEWER-CITYSITEBASE][2] = j;
        break;
    case 'z':
        Level->site[i][j].locchar = FLOOR;
        Level->site[i][j].p_locf = L_MAZE;
        break;
    case 'O':
        Level->site[i][j].locchar = OPEN_DOOR;
        Level->site[i][j].p_locf = L_ORACLE;
        CitySiteList[L_ORACLE-CITYSITEBASE][1] = i;
        CitySiteList[L_ORACLE-CITYSITEBASE][2] = j;
        break;
    case '?':
        randommazesite(i,j,populate);
        break;
    }
    lreset(i,j,SEEN);
}

void randommazesite(int i, int j, int populate)
{
    switch(random_range(7)) {
    case 0:
    case 1:
        Level->site[i][j].locchar = FLOOR;
        /*    Level->site[i][j].p_locf = TRAP_BASE+random_range(NUMTRAPS); */
        Level->site[i][j].p_locf = TRAP_BASE+random_range(NUM_SAFE_TRAPS);
        break;
    case 2:
    case 3:
        Level->site[i][j].locchar = FLOOR;
        if (populate)
            make_site_monster(i,j,-1);
        break;
    case 4:
    case 5:
        Level->site[i][j].locchar = FLOOR;
        if (populate)
            make_site_treasure(i,j,5);
        break;
    default:
        Level->site[i][j].locchar = FLOOR;
    }
}


/* undead are not hostile unless disturbed.... */
void make_minor_undead(int i, int j)
{
    int mid;
    if (random_range(2)) mid = GHOST;
    else mid = HAUNT;
    make_site_monster(i,j,mid);
    m_status_reset(Level->site[i][j].creature,AWAKE);
    m_status_reset(Level->site[i][j].creature,HOSTILE);
}

/* undead are not hostile unless disturbed.... */
void make_major_undead(int i, int j)
{
    int mid;
    if (random_range(2)) mid = LICHE; /* lich */
    else mid = VAMP_LORD; /*vampire lord*/
    make_site_monster(i,j,mid);
    m_status_reset(Level->site[i][j].creature,AWAKE);
    m_status_reset(Level->site[i][j].creature,HOSTILE);
}


static char jail[5][11] =
{
    { '#', '#', '*', '#', '#', '*', '#', '#', '*', '#', '#' },
    { '#', '#', '*', '#', '#', '*', '#', '*', '#', '#', '#' },
    { '#', '#', 'T', '#', 'T', '#', 'T', '#', 'T', '#', '#' },
    { '#', '#', '7', '#', '7', '#', '7', '#', '7', '#', '#' },
    { '#', '#', 'R', '#', 'R', '#', 'R', '#', 'R', '#', '#' }
};

/* fixes up the jail in case it has been munged by player action */
void repair_jail(void)
{
    int i,j;
    for(i=0; i<11; i++)
        for(j=0; j<5; j++) {
            switch(jail[j][i]) {
            case '#':
                City->site[i+35][j+52].locchar = WALL;
                City->site[i+35][j+52].p_locf = L_NO_OP;
                City->site[i+35][j+52].aux = NOCITYMOVE;
                break;
            case '*':
                City->site[i+35][j+52].locchar = WALL;
                City->site[i+35][j+52].p_locf = L_NO_OP;
                City->site[i+35][j+52].aux = 10;
                break;
            case 'T':
                City->site[i+35][j+52].locchar = FLOOR;
                City->site[i+35][j+52].p_locf = L_PORTCULLIS_TRAP;
                City->site[i+35][j+52].aux = NOCITYMOVE;
                break;
            case '7':
                City->site[i+35][j+52].locchar = FLOOR;
                City->site[i+35][j+52].p_locf = L_PORTCULLIS;
                City->site[i+35][j+52].aux = NOCITYMOVE;
                break;
            case 'R':
                City->site[i+35][j+52].locchar = FLOOR;
                City->site[i+35][j+52].p_locf = L_RAISE_PORTCULLIS;
                City->site[i+35][j+52].aux = NOCITYMOVE;
                break;
            }
            lreset(i+35, j+52, CHANGED);
        }
}
