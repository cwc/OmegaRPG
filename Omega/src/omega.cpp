/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */

/* this file includes main() and some top-level functions */
/* omega.c */

#include "glob.h"

/* Note: in order to avoid a memory bug I've been told about, I'm
   explicitly initializing every global to something. */

/* most globals originate in omega.c */

const char* Omegalib;		/* contains the path to the library files */

#ifdef DEBUG
FILE *DG_debug_log; /* debug log file pointer */
int DG_debug_flag = 0; /* debug flag -- set by -d commandline option */
#endif

/* Objects and Monsters are allocated and initialized in init.c */

/* one of each spell */
struct spell Spells[NUMSPELLS+1] = {0};

/* locations of city sites [0] - found, [1] - x, [2] - y */
int CitySiteList[NUMCITYSITES][3] = {0};

/* Currently defined in caps since it is now a variable, was a constant */
int LENGTH=MAXLENGTH;
int WIDTH=MAXWIDTH;

long GameStatus=0L;                   /* Game Status bit vector */
int ScreenLength = 0;                 /* How large is level window */
int ScreenWidth = 0;                 /* How large is level window */
struct player Player = {0};           /* the player */

struct terrain Country[COUNTRY_WIDTH][COUNTRY_LENGTH] = {0};/* The countryside */

struct level *City=NULL;              /* The city of Rampart */
struct level *TempLevel=NULL;         /* Place holder */
struct level *Level=NULL;             /* Pointer to current Level */
struct level *Dungeon=NULL;           /* Pointer to current Dungeon */
int Villagenum = 0;                   /* Current Village number */
int ScreenOffset = 0;                 /* Offset of displayed screen to level */
int ScreenXOffset = 0;                 /* Offset of displayed screen to level */
int MaxDungeonLevels = 0;             /* Deepest level allowed in dungeon */
int Current_Dungeon= -1;              /* What is Dungeon now */
int Current_Environment= E_CITY;      /* Which environment are we in */
int Last_Environment= E_COUNTRYSIDE;  /* Which environment were we in */

int Dirs[2][9]=                       /* 9 xy directions */
{ { 1,1,-1,-1,1,-1,0,0,0} , { 1,-1,1,-1,0,0,1,-1,0 } };

int Cmd='s';                         /* last player command */
int Command_Duration = 0;             /* how long does current command take */
Monster *Arena_Monster=NULL;   /* Opponent in arena */
int Arena_Opponent=0;                 /* case label of opponent in l_arena()*/
int Arena_Victory = 0;                /* did player win in arena? */
int Imprisonment=0;                   /* amount of time spent in jail */
int Precipitation=0;                  /* Hours of rain, snow, etc */
int Lunarity=0;                       /* Effect of the moon on character */
int Phase = 0;                        /* Phase of the moon */
int Date = 0;                         /* Starting date */
int Pawndate = 0;                     /* Pawn Shop item generation date */
Object* Pawnitems[PAWNITEMS] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
/* items in pawn shop */
int SymbolUseHour= -1;                /* holy symbol use marker */
int SymbolUseDay= -1;                /* holy symbol use marker */
int ViewHour= -1;                     /* crystal ball use marker */
int ViewDay= -1;                     /* crystal ball use marker */
int ZapHour= -1;                      /* staff of enchantment use marker */
int ZapDay= -1;                      /* staff of enchantment use marker */
int HelmHour= -1;                     /* helm of teleportation use marker*/
int HelmDay= -1;                     /* helm of teleportation use marker*/
int Constriction=0;                   /* Dragonlord Attack State */
int Blessing=false;                   /* Altar Blessing State */
int LastDay= -1;                      /* DPW date of dole */
int LastFreeGymDay = -1;              // Date of last free gym session
int LastFreeYogaDay = -1;              /* Last free gifted Yoga session */
int RitualHour= -1;                   /* last use of ritual magic */
int RitualRoom= -1;                   /* last room of ritual magic */
int Lawstone=0;                       /* magic stone counter */
int Chaostone=0;                      /* magic stone counter */
int Mindstone=0;                      /* magic stone counter */
int Searchnum = 1;                    /* number of times to search on 's' */
int Behavior;			      /* Player NPC behavior */
int Verbosity = VERBOSE;              /* verbosity level */
long Time = 0;                        /* turn number */
int Tick = 0;                         /* 10 a turn; action coordinator */
char Stringbuffer[STRING_BUFFER_SIZE][80];	/* last strings printed */
long Gymcredit = 0;                   /* credit at rampart gym */
int YogaSessions = 0;				  /* free yoga sessions for the underprivileged */
int Spellsleft = 0;                   /* research allowance at college */
int SalaryAmount = 0;                 /* legion salary */
int SalaryAccount = 0;                /* account number to which legion salary is paid */

int Studiesleft = 0;                  /* study allowance at monastery */

int StarGemUse = 0;                   /* last date of star gem use */
int HiMagicUse = 0;                   /* last date of high magic use */
int HiMagic = 0;                      /* current level for l_throne */

bank_account *bank = NULL;            /* ptr to a list of bank accounts */

long FixedPoints = 0;                 /* points are frozen after adepthood*/
int LastTownLocX=0;            /* previous position in village or city */
int LastTownLocY=0;            /* previous position in village or city */
int LastCountryLocX=0;            /* previous position in countryside */
int LastCountryLocY=0;            /* previous position in countryside */

char Str1[STRING_LEN] = {0},Str2[STRING_LEN] = {0},Str3[STRING_LEN] = {0},Str4[STRING_LEN] = {0};
/* Some string space, random uses */

pol Condoitems=NULL;                        /* Items in condo */
pol Bagitems=NULL;                          /* Items in bag of holding */

/* high score names, levels, behavior */
int Grandmasterbehavior = 0;
int Grandmasterlevel = 0;
char Grandmaster[80] = {0};

int Shadowlordbehavior = 0,Archmagebehavior = 0,Primebehavior = 0,Commandantbehavior = 0;
int Championbehavior = 0,Priestbehavior[7] = {0},Hibehavior = 0,Dukebehavior = 0;
int Chaoslordbehavior = 0,Lawlordbehavior = 0,Justiciarbehavior = 0;
char Shadowlord[80] = {0},Archmage[80] = {0},Prime[80] = {0},Commandant[80] = {0},Duke[80] = {0};
char Champion[80] = {0},Priest[7][80] = {0},Hiscorer[80] = {0},Hidescrip[80] = {0};
char Chaoslord[80] = {0},Lawlord[80] = {0},Justiciar[80] = {0};
int Shadowlordlevel = 0,Archmagelevel = 0,Primelevel = 0,Commandantlevel = 0,Dukelevel = 0;
int Championlevel = 0,Priestlevel[7] = {0},Hilevel = 0,Justiciarlevel = 0;
long Hiscore = 0L;
int Chaoslordlevel = 0,Lawlordlevel = 0,Chaos = 0,Law = 0;

/* New globals which used to be statics */
int twiddle = false;
int saved=false;
int onewithchaos=false;
int club_hinthour = 0;
int winnings = 0;
int tavern_hinthour = -1;
int scroll_ids[30];
int potion_ids[30];
int stick_ids[30];
int ring_ids[30];
int cloak_ids[30];
int boot_ids[30];

int deepest[E_MAX + 1];
int level_seed[E_MAX + 1];	/* random number seed that generated level */

/* This may be implementation dependent */
/* SRANDFUNCTION is defined in odefs.h */
/* environment is the environment about to be generated, or -1 for the first */
/* time, or -2 if we want to restore the random number point */
void initrand(int environment, int level)
{
    static int store;
    int seed;

    if (environment >= 0)
        store = RANDFUNCTION();
    /* Pseudo Random Seed */
    if (environment == E_RANDOM)
        seed = (int) time(NULL);
    else if (environment == E_RESTORE)
        seed = store;
    else
        seed = level_seed[environment] + 1000*level;
    SRANDFUNCTION(seed);
}


int game_restore(const char *savefile)
{
    int ok;
    ok = restore_game(savefile);
    if (! ok) {
        endgraf();
        printf("Try again with the right save file, luser!\n");
        exit(1);
    }
    change_to_user_perms();
#ifdef WIN32
    _unlink(savefile);
#else
    unlink(savefile);
#endif
    change_to_game_perms();
    return(true);
}

/* Start up game with new dungeons; start with player in city */
void init_world(void)
{
    int env, i;

    City = Level = TempLevel = Dungeon = NULL;
    for (env = 0; env <= E_MAX; env++)
        level_seed[env] = RANDFUNCTION();
    load_country();
    for(i=0; i<NUMCITYSITES; i++)
        CitySiteList[i][0] = false;
    load_city(true);
    Level = City;
    Current_Environment = E_CITY;
    setPlayerXY(62, 20);
    print1("You pass through the massive gates of Rampart, the city.");
    screencheck(Player.x, Player.y);
}

/* set variable item names */
void inititem(int reset)
{
    int i;

    if (reset) {
        shuffle(scroll_ids, 30);
        shuffle(potion_ids, 20);
        shuffle(stick_ids, 20);
        shuffle(boot_ids, 20);
        shuffle(cloak_ids, 20);
        shuffle(ring_ids, 20);
    }
    
    for(i=0; i<NUMSCROLLS; i++)
        Objects[SCROLLID+i].objstr = scrollname(i);
    for(i=0; i<NUMPOTIONS; i++)
        Objects[POTIONID+i].objstr = potionname(i);
    Objects[OB_POTION_DEATH].objstr = potionname(18);
    Objects[OB_POTION_LIFE].objstr = potionname(19);
    for(i=0; i<NUMSTICKS; i++)
        Objects[STICKID+i].objstr = stickname(i);
    for(i=0; i<NUMBOOTS; i++)
        Objects[BOOTID+i].objstr = bootname(i);
    for(i=0; i<NUMCLOAKS; i++)
        Objects[CLOAKID+i].objstr = cloakname(i);
    for(i=0; i<NUMRINGS; i++)
        Objects[RINGID+i].objstr = ringname(i);
}
