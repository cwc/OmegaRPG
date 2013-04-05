/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* This file is the header file for all omega modules */
/* defs.h */

/* omega will NOT function unless the implementor sets the appropriate
definitions in the following section. */

/*--------------------------USER DEFINITIONS--------------------------*/

#ifdef WIN32
#include <ShlObj.h>
#include <malloc.h>

#undef MOUSE_MOVED // Will be redefined when curses is included
#define strlen(x) (int)strlen(x) // Squashes numerous warnings
#define sleep(x) Sleep((DWORD) (x * 1000))
#define usleep(x) sleep(x / 1000000.0)

extern char* getlogin();
#else
// These includes are not supported or supplanted in Windows
#include <unistd.h>
#include <pwd.h>
#include <sys/file.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#endif

#include <signal.h>
#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h> // The assert macro (for ANSI/ISO C).  Hopefully this will always work!
#include <string.h>

#include "curses.h"
#include "colors.h"

#include "GameState.h"
#include "Object.h"
#include "Monster.h"
#include "Village.h"

/* Objects */
#include "Thing.h"

/* Update the display every turn to center on the player.  Rather heavy
 * on the bandwidth. */
#define CENTER_ON_PLAYER // TODO Make this a user-specifiable option

/* Implementor should define int32 as the type of integer which uses
 * 32 bits. */
typedef int int32;

/* Define the maximum length of a filename on your system.  If you don't */
/* define, will try to make an educated guess.  If you have one,  */
/* /usr/include/limits.h is a good place to look for this value. */
#define FNAME_MAX_LEN 48

/* OMEGALIB is where all the data files reside.
   Note the final / is necessary.
   msdos note: \ is the C string escape character, so you need \\ in the
   path given in OMEGALIB
   This might usually be "/usr/games/lib/omegalib/", for unix,
   or something like "c:\\games\\omega\\omegalib\\" for msdos */

#define OMEGALIB "data/"

/* TILEFILE is the name of the file you want to use for graphics tiles. You */
/* aren't really free to use any file you want here. It should be the Omega */
/* distribution graphics files provided for your computer. Of course a file */
/* of the same format and size as the "correct" file will also work if you */
/* want to change things around. This file should be located in OMEGALIB */

#define TILEFILE "omegatiles.xpm"

/*---------------------------SYSTEM DEFINITIONS---------------------------*/

/* Don't change anything from here on (unless you know what you're doing) */
#define VERSION 9100
#define VERSIONSTRING "Omega version 0.91"

#ifndef WIN32
#define UNIX // Assuming *nix
#endif

#define VACANT 0
#define ABORT -1
#define CASHVALUE -2

/* moderately arbitrary but probably cannot be easily changed */
/*#define MAXWIDTH 64*/
#define MAXWIDTH 128
#define MAXLENGTH 64
#define SMALLSCREEN_LENGTH 16
#define SMALLSCREEN_WIDTH 64

#define STANDARD_LENGTH 64
#define STANDARD_WIDTH 64

#define ARENA_WIDTH SMALLSCREEN_WIDTH
#define ARENA_LENGTH SMALLSCREEN_LENGTH

#define ABYSS_WIDTH STANDARD_WIDTH
#define ABYSS_LENGTH SMALLSCREEN_LENGTH

#define ASTRAL_WIDTH STANDARD_WIDTH
#define ASTRAL_LENGTH STANDARD_LENGTH

#define CASTLE_WIDTH STANDARD_WIDTH
#define CASTLE_LENGTH STANDARD_LENGTH

#define CAVES_WIDTH STANDARD_WIDTH
#define CAVES_LENGTH STANDARD_LENGTH

#define CIRCLE_WIDTH SMALLSCREEN_WIDTH
#define CIRCLE_LENGTH SMALLSCREEN_LENGTH

#define COUNTRY_WIDTH STANDARD_WIDTH
#define COUNTRY_LENGTH STANDARD_LENGTH

#define COURT_WIDTH SMALLSCREEN_WIDTH
#define COURT_LENGTH 24

#define DLAIR_WIDTH SMALLSCREEN_WIDTH
#define DLAIR_LENGTH SMALLSCREEN_LENGTH

#define HOUSE_WIDTH SMALLSCREEN_WIDTH
#define HOUSE_LENGTH SMALLSCREEN_LENGTH

#define HOVEL_WIDTH SMALLSCREEN_WIDTH
#define HOVEL_LENGTH SMALLSCREEN_LENGTH

#define MANSION_WIDTH SMALLSCREEN_WIDTH
#define MANSION_LENGTH SMALLSCREEN_LENGTH

#define MISLE_WIDTH SMALLSCREEN_WIDTH
#define MISLE_LENGTH SMALLSCREEN_LENGTH

#define PALACE_WIDTH STANDARD_WIDTH
#define PALACE_LENGTH STANDARD_LENGTH

#define RAMPART_WIDTH 128
#define RAMPART_LENGTH STANDARD_LENGTH

#define SEWERS_WIDTH STANDARD_WIDTH
#define SEWERS_LENGTH STANDARD_LENGTH

#define SPEAK_WIDTH SMALLSCREEN_WIDTH
#define SPEAK_LENGTH SMALLSCREEN_LENGTH

#define TACTICAL_WIDTH SMALLSCREEN_WIDTH
#define TACTICAL_LENGTH SMALLSCREEN_LENGTH

#define TEMPLE_WIDTH SMALLSCREEN_WIDTH
#define TEMPLE_LENGTH SMALLSCREEN_LENGTH

#define VILLAGE_WIDTH SMALLSCREEN_WIDTH
#define VILLAGE_LENGTH SMALLSCREEN_LENGTH

#define VOLCANO_WIDTH STANDARD_WIDTH
#define VOLCANO_LENGTH STANDARD_LENGTH

/* number of slots in inventory. Cannot be changed without work */
#define MAXITEMS 16

/* number of slots in pack. Should be <= 26. */
#define MAXPACK 26

/* number of items in pawn shop. Should be <= 26 */
#define PAWNITEMS 20 /* DG -- the more the merrier.  WDT -- I agree. */

/* number of lines back strings are recalled */
#define STRING_BUFFER_SIZE 25

/* number of rerolls allowed +1 */  /* added by dagibbs (DG) */
#define REROLLS -1

/* Verbosity levels */
#define TERSE 0
#define MEDIUM 1
#define VERBOSE 2

/* Arbitrary. Max of the levels in the dungeons */
#define MAXLEVELS 21

/* levels in each dungeon */
#define ASTRALLEVELS 5
#define SEWERLEVELS 18
#define CASTLELEVELS 16
#define CAVELEVELS 10
#define VOLCANOLEVELS 20
#define PALACELEVELS 14

/* non-existant environments for the random number seeding routine */
/* added 12/30/98 (DG) */
#define E_RESTORE -2
#define E_RANDOM -1
/* general environment types */
#define E_NEVER_NEVER_LAND 0
#define E_COUNTRYSIDE 1
#define E_CITY 2
#define E_VILLAGE 3
#define E_TACTICAL_MAP 4
#define E_SEWERS 5
#define E_CASTLE 6
#define E_CAVES 7
#define E_VOLCANO 8
#define E_ASTRAL 9
#define E_ARENA 10
#define E_HOVEL 11
#define E_MANSION 12
#define E_HOUSE 13
#define E_DLAIR 14
#define E_ABYSS 15
#define E_STARPEAK 16
#define E_MAGIC_ISLE 17
#define E_TEMPLE 18
#define E_CIRCLE 19
#define E_COURT 20
#define E_PALACE 21
#define E_MAX E_PALACE

/* player game status */
#define DEAD 1
#define QUIT 2
#define WIN 3
#define BIGWIN 4

/* kind of arbitrary */
#define MAXROOMS 48
#define MAXCONNECTIONS 4
#define STRING_LEN 100

/* some random characters */
#define ESCAPE 27
#define RETURN '\n' /* Aren't these backwards? WSS */
#define LINEFEED '\r' /* Aren't these backwards? WSS */
#define BACKSPACE '\b'
#define DELETE_CHAR 127

/* tac mode action definitions */
/* have to remember to find where these are used, mostly unused, now! */
#define DISENGAGE 10
#define BLOCK 20
#define CUT 30
#define THRUST 40
#define MAGIC 50
#define LUNGE 60
#define RIPOSTE 70
#define WIELD 80
#define PICK_UP 90

/* as in attack low, block high, etc. */
/* These values may be added to the ones above to get things like
   block high, cut low, etc. CLEVER is only used by some monsters
   to cheat with.... */
#define LOW 1
#define CENTER 2
#define HIGH 3
#define CLEVER 4

/* weapon types */
#define CUTTING 1
#define THRUSTING 2
#define STRIKING 3
#define MISSILE 4

/* random aux constants */
/* aux field for private residences in city */
#define BURGLED 2
#define LOCKED 3
#define UNLOCKED 0

/* cannot use M command on site with this aux value */
#define NOCITYMOVE 666

/* bow and crossbow object aux fields */
#define LOADED 1
#define UNLOADED 0

/* alignment used randomly throughout*/
#define LAWFUL 1
#define CHAOTIC 2
#define NEUTRAL 3

/* spells */
#define NUMSPELLS 42

#define S_MON_DET 0
#define S_OBJ_DET 1
#define S_MISSILE 2
#define S_FIREBOLT 3
#define S_TELEPORT 4
#define S_LBALL 5
#define S_SLEEP 6
#define S_DISRUPT 7
#define S_DISINTEGRATE 8
#define S_POLYMORPH 9
#define S_HEAL 10
#define S_DISPEL 11
#define S_IDENTIFY 12
#define S_BREATHE 13
#define S_INVISIBLE 14
#define S_WARP 15
#define S_ENCHANT 16
#define S_BLESS 17
#define S_RESTORE 18
#define S_CURE 19
#define S_TRUESIGHT 20
#define S_HELLFIRE 21
#define S_KNOWLEDGE 22
#define S_HERO 23
#define S_RETURN 24
#define S_DESECRATE 25
#define S_HASTE 26
#define S_SUMMON 27
#define S_SANCTUARY 28
#define S_ACCURACY 29
#define S_RITUAL 30
#define S_FEAR 31
#define S_APPORT 32
#define S_SHADOWFORM 33
#define S_ALERT 34
#define S_REGENERATE 35
#define S_SANCTIFY 36
#define S_CLAIRVOYANCE 37
#define S_DRAIN 38
#define S_LEVITATE 39
#define S_WISH 40
#define S_NUTRITION 41

/* ranks in guilds, etc */
#define NUMRANKS 10

#define LEGION 0
#define ARENA 1
#define COLLEGE 2
#define THIEVES 3
#define ORDER 4
#define CIRCLE 5
#define NOBILITY 6
#define PRIESTHOOD 7
#define MONKS 8
#define ADEPT 9

#define LEGIONAIRE 1
#define CENTURION 2
#define FORCE_LEADER 3
#define COLONEL 4
#define COMMANDANT 5

#define TRAINEE 1
#define BESTIARIUS 2
#define RETIARIUS 3
#define GLADIATOR 4
#define CHAMPION 5

#define NOVICE 1
#define STUDENT 2
#define PRECEPTOR 3
#define MAGE 4
#define ARCHMAGE 5

#define TMEMBER 1
#define ATHIEF 2
#define THIEF 3
#define TMASTER 4
#define SHADOWLORD 5

#define GALLANT 1
#define GUARDIAN 2
#define CHEVALIER 3
#define PALADIN 4
#define JUSTICIAR 5

#define INITIATE 1
#define ENCHANTER 2
#define SORCEROR 3
#define HIGHSORCEROR 4
#define PRIME 5

#define COMMONER 1
#define ESQUIRE 2
#define KNIGHT 3
#define LORD 4
#define DUKE 5

#define LAY 1
#define ACOLYTE 2
#define PRIEST 3
#define SPRIEST 4
#define HIGHPRIEST 5

#define MONK_TRAINEE 1
#define MONK_MONK 2
#define MONK_MASTER 3
#define MONK_MASTER_SIGHS 4
#define MONK_MASTER_PAINS 5
#define MONK_MASTER_TEARS 6
#define MONK_GRANDMASTER 7

/* different priesthoods */
#define ODIN 1
#define SET 2
#define ATHENA 3
#define HECATE 4
#define DRUID 5
#define DESTINY 6



/* MONSTER STATUS/ABILITY BITS */
/* currently a long */
#define AWAKE 1
#define MOBILE 2
#define HOSTILE 4
/* missing bit 3, 8 */
#define WANDERING 16
#define HUNGRY 32
#define GREEDY 64
#define NEEDY  128
#define ONLYSWIM 256
#define FLYING 512
#define INTANGIBLE 1024
#define M_INVISIBLE 2048
#define SWIMMING 4096
#define POISONOUS 8192
#define EDIBLE 16384
#define ALLOC 32768  /* allocated name & corpseString */


/* PLAYER STATUS INDICES */
#define NUMSTATI 25

#define ACCURACY 0
#define BLINDED 1
#define SLOWED 2
#define DISPLACED 3
#define SLEPT 4
#define DISEASED 5
#define POISONED 6
#define HASTED 7
#define BREATHING 8
#define INVISIBLE 9
#define REGENERATING 10
#define VULNERABLE 11
#define BERSERK 12
#define IMMOBILE 13
#define ALERT 14
#define AFRAID 15
#define HERO 16
#define LEVITATING 17
#define ACCURATE 18
#define TRUESIGHT 19
#define SHADOWFORM 20
#define ILLUMINATION 21
#define DEFLECTION 22
#define PROTECTION 23
/* PROTECTION is deviant -- indicates protective value, not duration */
#define RETURNING 24
/* RETURNING is somewhat deviant--how many turns 'til RETURN spell goes off */

/* player immunity indices */
/* also monster immunity bits (2^n) */
/* also damage types */
#define NUMIMMUNITIES 14

#define UNSTOPPABLE 0
#define NORMAL_DAMAGE 1
#define FLAME 2
#define COLD 3
#define ELECTRICITY 4
#define POISON 5
#define ACID 6
#define FEAR 7
#define SLEEP 8
#define NEGENERGY 9
#define OTHER_MAGIC 10
#define THEFT 11
#define GAZE 12
#define INFECTION 13
#define EVERYTHING -1


/* location lstatus bits */
#define SEEN 1
#define LIT 2
#define SECRET 4
#define STOPS 8
#define CHANGED 16


/* room string id */
/* for use in roomname() */
/* number of rooms above ROOMBASE */
#define NUMROOMNAMES 30

#define RS_WALLSPACE 1
#define RS_CORRIDOR 2
#define RS_CAVERN 3
#define RS_GOBLINKING 4
#define RS_DRAGONLORD 5
#define RS_PONDS 6
#define RS_OCEAN 7
#define RS_WYRM 8
#define RS_ADEPT 9
#define RS_DESTINY 10
#define RS_ODIN 11
#define RS_SET 12
#define RS_ATHENA 13
#define RS_HECATE 14
#define RS_DRUID 15
#define RS_COUNTRYSIDE 16
#define RS_ARENA 17
#define RS_SEWER_DUCT 18
#define RS_DRAINED_SEWER 19
#define RS_DROWNED_SEWER 20
#define RS_KITCHEN 21
#define RS_BEDROOM 22
#define RS_BATHROOM 23
#define RS_DININGROOM 24
#define RS_SECRETPASSAGE 25
#define RS_CLOSET 26
#define RS_LOWERASTRAL 27
#define RS_EARTHPLANE 28
#define RS_WATERPLANE 29
#define RS_AIRPLANE 30
#define RS_FIREPLANE 31
#define RS_HIGHASTRAL 32
#define RS_VOLCANO 33
#define RS_STARPEAK 34
#define RS_MAGIC_ISLE 35
#define RS_CIRCLE 36
#define RS_ZORCH 37
#define RS_COURT 38
/* normal room name indices start after the RS_ constants */
#define ROOMBASE 39
#define RS_GARDEROBE ROOMBASE+0
#define RS_CELL ROOMBASE+1
#define RS_TILED ROOMBASE+2
#define RS_CRYSTAL_CAVE ROOMBASE+3
#define RS_BEDROOM2 ROOMBASE+4
#define RS_STOREROOM ROOMBASE+5
#define RS_CHARRED ROOMBASE+6
#define RS_MARBLE_HALL ROOMBASE+7
#define RS_EERIE_CAVE ROOMBASE+8
#define RS_TREASURE ROOMBASE+9
#define RS_SMOKEY ROOMBASE+10
#define RS_APARTMENT ROOMBASE+11
#define RS_ANTECHAMBER ROOMBASE+12
#define RS_HAREM ROOMBASE+13
#define RS_MULTIPURPOSE ROOMBASE+14
#define RS_STALACTITES ROOMBASE+15
#define RS_GREENHOUSE ROOMBASE+16
#define RS_WATERCLOSET ROOMBASE+17
#define RS_STUDY ROOMBASE+18
#define RS_LIVING_ROOM ROOMBASE+19
#define RS_DEN ROOMBASE+20
#define RS_ABATOIR ROOMBASE+21
#define RS_BOUDOIR ROOMBASE+22
#define RS_STAR_CHAMBER ROOMBASE+23
#define RS_MANMADE_CAVE ROOMBASE+24
#define RS_SEWER_CONTROL ROOMBASE+25
#define RS_SHRINE ROOMBASE+26
#define RS_MAGIC_LAB ROOMBASE+27
#define RS_PENTAGRAM ROOMBASE+28
#define RS_OMEGA_DAIS ROOMBASE+29

// Color macros
#ifdef OMEGA_CLRGEN // Only defined when using the color-gen utility
#   define CLR(fg)     OMEGA_CLRGEN1 fg
#   define CLRS(fg, bg)        OMEGA_CLRGEN2 fg bg
#else
#   define CLR(fg)     CLR_##fg##_BLACK
#   define CLRS(fg, bg)        CLR_##fg##_##bg
#endif

/* objects, locations, and terrain; characters to draw */
#define NULL_ITEM '\0'
#define SPACE (' ' | CLR(WHITE))
#define WALL ('#' | CLR(GREY))
#define PORTCULLIS ('7' | CLR(WHITE))
#define OPEN_DOOR ('|' | CLR(BROWN))
#define CLOSED_DOOR ('-' | CLR(BROWN))
#define WHIRLWIND ('6' | CLR(LIGHT_BLUE))
#define ABYSS ('0' | CLRS(BLACK,BROWN))
#define VOID_CHAR (' ' | CLR(WHITE))
#define LAVA ('`' | CLR(RED))
#define HEDGE ('\"' | CLR(GREEN))
#define WATER ('~' | CLR(BLUE))
#define FIRE (';' | CLR(LIGHT_RED))
#define TRAP ('^' | CLR(RED))
#define LIFT ('_' | CLR(BRIGHT_WHITE))
#define STAIRS_UP ('<' | CLR(WHITE))
#define STAIRS_DOWN ('>' | CLR(WHITE))
#define FLOOR ('.' | CLR(BROWN))
#define PLAYER ('@' | CLR(WHITE))
#define CORPSE ('+' | CLR(RED))
#define STATUE ('1' | CLR(GREY))
#define RUBBLE ('4' | CLR(GREY))
#define ALTAR ('8' | CLR(LIGHT_BLUE))
#define CASH ('$' | CLR(YELLOW))	/* various kinds of money */
#define PILE ('*' | CLR(BRIGHT_WHITE))	/* several objects in one place */
#define FOOD ('%' | CLR(BROWN))
#define WEAPON (')' | CLR(GREY))
#define MISSILEWEAPON ('(' | CLR(BROWN))
#define SCROLL ('?' | CLR(YELLOW))
#define POTION ('!' | CLR(LIGHT_GREEN))
#define ARMOR (']' | CLR(GREY))
#define SHIELD ('[' | CLR(BROWN))
#define CLOAK ('}' | CLR(CYAN))
#define BOOTS ('{' | CLR(BROWN))
#define STICK ('/' | CLR(BROWN))

#define RING ('=' | CLR(YELLOW))
#define THING ('\\' | CLR(WHITE))
#define ARTIFACT ('&' | CLR(YELLOW))

/* TERRAIN TYPES */
#define PLAINS ('-' | CLR(LIGHT_GREEN))
#define TUNDRA ('_' | CLR(GREY))
#define ROAD ('.' | CLR(BROWN))
#define MOUNTAINS ('^' | CLR(GREY))
#define PASS ('v' | CLR(BROWN))
#define RIVER ('~' | CLR(BLUE))
#define CITY ('O' | CLR(WHITE))
#define VILLAGE ('o' | CLR(WHITE))
#define FOREST ('(' | CLR(LIGHT_GREEN))
#define JUNGLE (')' | CLR(GREEN))
#define SWAMP ('=' | CLR(GREEN))
#define VOLCANO ('!' | CLR(RED))
#define CASTLE ('%' | CLR(GREY))
#define TEMPLE ('X' | CLR(BROWN))
#define CAVES ('*' | CLRS(BLACK,BROWN))
#define DESERT ('\"' | CLR(YELLOW))
#define CHAOS_SEA ('+' | CLR(LIGHT_PURPLE))
#define STARPEAK ('|' | CLR(LIGHT_BLUE))
#define DRAGONLAIR ('$' | CLR(BROWN))
#define MAGIC_ISLE ('&' | CLR(PURPLE))
#define PALACE ('K' | CLR(PURPLE))

#define CHAIR ('5' | CLR(BROWN))
#define SAFE ('3' | CLR(GREY))
#define FURNITURE ('2' | CLR(BROWN))
#define BED ('9' | CLR(CYAN))

/* wow, all characters used! */

/* total number of editable stats */
#define NUMSTATS 11

/* total number of player options */
#define NUMOPTIONS 10

/* number of options with true/false values */
#define NUMTFOPTIONS 9

/* The slot number of the two options not in Player.options */
#define VERBOSITY_LEVEL 9
#define SEARCH_DURATION 10

/* Player.options bits */
#define BELLICOSE 1
#define JUMPMOVE 2
#define RUNSTOP 4
#define PICKUP 8
#define CONFIRM 16
#define TOPINV 32
#define PACKADD 64
#define SHOW_COLOUR 256

/* This has to be changed whenever an item is added */
#define NUMSCROLLS 24
#define NUMPOTIONS 18
#define NUMFOODS 16
#define NUMTHINGS 31 /* DAG for mirror of self-knowledge */ /* WSS cards */
#define NUMCARDS 4
#define NUMWEAPONS 41
#define NUMARMOR 17
#define NUMSHIELDS 8
#define NUMCLOAKS 7
#define NUMBOOTS 7
#define NUMRINGS 9  /* DAG loss of ring of self-knowledge */
#define NUMSTICKS 17
#define NUMARTIFACTS 26

/* running sum of itemtypes, for indexing into Objects array */
#define THINGID 0
#define CARDID (THINGID+NUMTHINGS-NUMCARDS)
#define FOODID NUMTHINGS    /* 26 */
#define SCROLLID (FOODID + NUMFOODS)  /* 42 */
#define POTIONID (SCROLLID + NUMSCROLLS) /* 66 */
#define WEAPONID (POTIONID + NUMPOTIONS)  /* 84 */
#define ARMORID (WEAPONID + NUMWEAPONS) /* 125 */
#define SHIELDID (ARMORID + NUMARMOR)  /* 142 */
#define CLOAKID (SHIELDID + NUMSHIELDS)  /* 150 */
#define BOOTID (CLOAKID + NUMCLOAKS)  /* 157 */
#define RINGID (BOOTID + NUMBOOTS)  /* 164 */
#define STICKID (RINGID + NUMRINGS) /* 174 */
#define ARTIFACTID (STICKID + NUMSTICKS)  /* 191 */
#define CASHID (ARTIFACTID+NUMARTIFACTS)  /* 216 */
/* Corpse's aux field is monster id */
#define CORPSEID (CASHID+1)

#define TOTALITEMS (CORPSEID+1)

/* descriptive constants for various object ids */
#define OB_GARAGE_OPENER (THINGID+0)
#define OB_LOCK_PICK (THINGID+2)
#define OB_SALT_WATER (THINGID+6)
#define OB_KEY (THINGID+7)
#define OB_TORCH (THINGID+8)
#define OB_JUSTICIAR_BADGE (THINGID+16)
#define OB_TRAP_DART (THINGID+17)
#define OB_TRAP_ACID (THINGID+18)
#define OB_TRAP_SNARE (THINGID+19)
#define OB_TRAP_FIRE (THINGID+20)
#define OB_TRAP_TELEPORT (THINGID+21)
#define OB_TRAP_SLEEP (THINGID+22)
#define OB_TRAP_DISINTEGRATE (THINGID+23)
#define OB_TRAP_ABYSS (THINGID+24)
#define OB_TRAP_MANADRAIN (THINGID+25)
#define OB_DEBIT_CARD (CARDID+0)
#define OB_CREDIT_CARD (CARDID+1)
#define OB_PREPAID_CARD (CARDID+2)
#define OB_SMART_CARD (CARDID+3)
#define OB_RATION (FOODID+0)
#define OB_LEMBAS (FOODID+1)
#define OB_GRAIN (FOODID+15)
#define OB_BLANK_SCROLL (SCROLLID+0)
#define OB_SPELLS_SCROLL (SCROLLID+1)
#define OB_SCROLL_LAW (SCROLLID+17)
#define OB_POTION_CHAOS (POTIONID+14)
#define OB_SHORT_SWORD (WEAPONID+1)
#define OB_GREAT_SWORD  (WEAPONID+5)
#define OB_GREAT_AXE  (WEAPONID+12)
#define OB_CLUB  (WEAPONID+17)
#define OB_QUARTERSTAFF  (WEAPONID+18)
#define OB_SPEAR  (WEAPONID+19)
#define OB_HALBERD  (WEAPONID+20)
#define OB_MACE_DISRUPT  (WEAPONID+25)
#define OB_LONGBOW  (WEAPONID+26)
#define OB_CROSSBOW  (WEAPONID+27)
#define OB_ARROW  (WEAPONID+28)
#define OB_BOLT  (WEAPONID+29)
#define OB_DESECRATOR  (WEAPONID+32)
#define OB_DEFENDER  (WEAPONID+34)
#define OB_VICTRIX  (WEAPONID+35)
#define OB_HEWER  (WEAPONID+36)
#define OB_GIANT_CLUB  (WEAPONID+38)
#define OB_SCYTHE_DEATH  (WEAPONID+39)
#define OB_LEATHER (ARMORID+1)
#define OB_MITHRIL_PLATE (ARMORID+11)
#define OB_DRAGONSCALE (ARMORID+12)
#define OB_LRG_RND_SHIELD (SHIELDID+2)
#define OB_DEFLECT (SHIELDID+7)
#define OB_CLOAK_PROTECT (CLOAKID+4)
#define OB_ORB_MASTERY (ARTIFACTID+0)
#define OB_ORB_FIRE (ARTIFACTID+1)
#define OB_ORB_WATER (ARTIFACTID+2)
#define OB_ORB_EARTH (ARTIFACTID+3)
#define OB_ORB_AIR (ARTIFACTID+4)
#define OB_DEAD_ORB (ARTIFACTID+5)
#define OB_ANTIOCH (ARTIFACTID+7)
#define OB_YENDOR (ARTIFACTID+8)
#define OB_KOLWYNIA (ARTIFACTID+9)
#define OB_POTION_DEATH (ARTIFACTID+10)
#define OB_POTION_LIFE (ARTIFACTID+13)
#define OB_SYMBOL_ODIN (ARTIFACTID+14)
#define OB_SYMBOL_SET (ARTIFACTID+15)
#define OB_SYMBOL_ATHENA (ARTIFACTID+16)
#define OB_SYMBOL_HECATE (ARTIFACTID+17)
#define OB_SYMBOL_DRUID (ARTIFACTID+18)
#define OB_SYMBOL_DESTINY (ARTIFACTID+19)
#define OB_KARNAK (ARTIFACTID+20)
#define OB_STARGEM (ARTIFACTID+21)
#define OB_SCEPTRE (ARTIFACTID+22)
#define OB_HOLDING (ARTIFACTID+24)

/* describing unique items and monsters */
#define COMMON 0
#define UNIQUE_UNMADE 1
#define UNIQUE_MADE 2
#define UNIQUE_TAKEN 3

/* general item function id's */
#define I_NO_OP 0
#define I_NOTHING 1

/* note some of these functions are for other types of items too */

/* scroll functions */
#define I_BLESS 101
#define I_ACQUIRE 102
#define I_ENCHANT 103
#define I_TELEPORT 104
#define I_WISH 105
#define I_CLAIRVOYANCE 106
#define I_DISPLACE 107
#define I_ID 108
#define I_FIREFLASH 109
#define I_SPELLS 110
#define I_JANE_T 111
#define I_ALERT 112
#define I_FLUX 113
#define I_CHARGE 114
#define I_WARP 115
#define I_KNOWLEDGE 116
#define I_LAW 117
#define I_HINT 118
#define I_HERO 119
#define I_TRUESIGHT 120
#define I_ILLUMINATE 121
#define I_DEFLECT 122

/* potion functions */
#define I_HEAL 201
#define I_OBJDET 202
#define I_MONDET 203
#define I_SLEEP_SELF 204
#define I_RESTORE 205
#define I_NEUTRALIZE_POISON 206
#define I_SPEED 207
#define I_AZOTH 208
#define I_REGENERATE 210
#define I_INVISIBLE 211
#define I_BREATHING 212
#define I_FEAR_RESIST 213
#define I_AUGMENT 214
#define I_CHAOS 215
#define I_ACCURACY 216
#define I_LEVITATION 217
#define I_CURE 218

/* stick functions */
#define I_FIREBOLT 301
#define I_LBOLT 302
#define I_MISSILE 303
#define I_SLEEP_OTHER 304
#define I_FIREBALL 305
#define I_LBALL 306
#define I_SUMMON 307
#define I_HIDE 308
#define I_DISRUPT 309
#define I_DISINTEGRATE 310
#define I_SNOWBALL 311
#define I_APPORT 312
#define I_DISPEL 313
#define I_POLYMORPH 314
#define I_FEAR 315

/* food functions */
#define I_FOOD 401
#define I_LEMBAS 402
#define I_STIM 403
#define I_POW 404
#define I_IMMUNE 405
#define I_POISON_FOOD 406
#define I_CORPSE 407
#define I_PEPPER_FOOD 408
#define I_CANNIBAL 409
#define I_INEDIBLE 410

/* boots functions */
#define I_PERM_SPEED 501
#define I_PERM_HERO 502
#define I_PERM_LEVITATE 503
#define I_PERM_AGILITY 504
#define I_BOOTS_JUMPING 505
#define I_BOOTS_7LEAGUE 506

/* cloak functions */
#define I_PERM_DISPLACE 601
#define I_PERM_NEGIMMUNE 602
#define I_PERM_INVISIBLE 603
#define I_PERM_ACCURACY 604
#define I_PERM_PROTECTION 605
#define I_PERM_TRUESIGHT 606

/* ring functions */
#define I_PERM_VISION 701
#define I_PERM_BURDEN 702
#define I_PERM_STRENGTH 703
#define I_PERM_GAZE_IMMUNE 704
#define I_PERM_FIRE_RESIST 705
#define I_PERM_POISON_RESIST 706
#define I_PERM_REGENERATE 707
#define I_PERM_KNOWLEDGE 708

/* armor functions */
#define I_PERM_ENERGY_RESIST 801
#define I_PERM_BREATHING 802
#define I_PERM_FEAR_RESIST 803
#define I_NORMAL_ARMOR 804

/* artifact functions */
#define I_ORBFIRE 901
#define I_ORBWATER 902
#define I_ORBEARTH 903
#define I_ORBAIR 904
#define I_ORBMASTERY 905
#define I_ORBDEAD 906
#define I_CRYSTAL 907
#define I_ANTIOCH 908
#define I_KOLWYNIA 909
#define I_DEATH 910
#define I_ENCHANTMENT 911
#define I_HELM 912
#define I_LIFE 913
#define I_JUGGERNAUT 914
#define I_SYMBOL 915
#define I_STARGEM 916
#define I_SCEPTRE 917
#define I_PLANES 918
#define I_HOLDING 919
#define I_SERENITY 920

/* weapons functions */
#define I_NORMAL_WEAPON 1001
#define I_LIGHTSABRE 1002
#define I_DEMONBLADE 1003
#define I_MACE_DISRUPT 1004
#define I_TANGLE 1005
#define I_ARROW 1006
#define I_BOLT 1007
#define I_VORPAL 1008
#define I_DESECRATE 1009
#define I_FIRESTAR 1010
#define I_DEFEND 1011
#define I_VICTRIX 1012
#define I_EMPIRE 1013
#define I_SCYTHE 1014
#define I_ACIDWHIP 1015

/* thing functions */
#define I_PICK 1101
#define I_KEY 1102
#define I_SHOVEL 1103 /* unused */
#define I_EXCAVATOR 1104 /* unused */
#define I_PERM_ILLUMINATE 1105
#define I_TRAP 1106
#define I_RAISE_PORTCULLIS 1107

/* shield functions */
#define I_PERM_DEFLECT 1201
#define I_NORMAL_SHIELD 1202

/* monster function ids */
/* Its conceivable for a function of one type to be called when another
would usually occur. A monster's special function may be an extra move,
for example. */

#define M_NO_OP -1

/* talk functions */
#define M_TALK_STUPID 101
#define M_TALK_SILENT 102
#define M_TALK_HUNGRY 103
#define M_TALK_GREEDY 104
#define M_TALK_TITTER 105
#define M_TALK_MAN 106
#define M_TALK_ROBOT 107
#define M_TALK_EVIL 108
#define M_TALK_BURBLE 109
#define M_TALK_SLITHY 110
#define M_TALK_MIMSY 111
#define M_TALK_SEDUCTOR 112
#define M_TALK_MP 113
#define M_TALK_IM 114
#define M_TALK_GUARD 115
#define M_TALK_GHOST 116
#define M_TALK_HINT 117
#define M_TALK_BEG 118
#define M_TALK_EF 119
#define M_TALK_GF 120
#define M_TALK_MORGON 121
#define M_TALK_LB 122
#define M_TALK_DEMONLOVER 123
#define M_TALK_ASSASSIN 124
#define M_TALK_NINJA 125
#define M_TALK_THIEF 126
#define M_TALK_MERCHANT 127
#define M_TALK_HORSE 128
#define M_TALK_PARROT 129
#define M_TALK_ANIMAL 130
#define M_TALK_HYENA 131
#define M_TALK_SERVANT 132
#define M_TALK_SCREAM 133
#define M_TALK_DRUID 134
#define M_TALK_ARCHMAGE 135
#define M_TALK_PRIME 136
#define M_TALK_MAHARAJA 137

/* ability functions */
#define M_SP_SURPRISE 201
#define M_SP_MP 202
#define M_SP_THIEF 203
#define M_SP_AGGRAVATE 204
#define M_SP_POISON_CLOUD 205
#define M_SP_HUGE 206
#define M_SP_SUMMON 207
#define M_SP_ILLUSION 208
#define M_SP_FLUTTER 209
#define M_SP_ESCAPE 210
#define M_SP_SPELL 211
#define M_SP_EXPLODE 212
#define M_SP_DEMON 213
#define M_SP_ACID_CLOUD 214
#define M_SP_WHIRL 215
#define M_SP_GHOST 216
#define M_SP_WHISTLEBLOWER 217
#define M_SP_EATER 218
#define M_SP_LAWBRINGER 219
#define M_SP_DRAGONLORD 220
#define M_SP_DE 221
#define M_SP_DEMONLOVER 222
#define M_SP_SEDUCTOR 223
#define M_SP_MASTER 224
#define M_SP_WYRM 225
#define M_SP_BLACKOUT 226
#define M_SP_BOG 227
#define M_SP_MERCHANT 228
#define M_SP_WERE 229
#define M_SP_SERVANT 231
#define M_SP_AV 232
#define M_SP_LW 233
#define M_SP_SWARM 234
#define M_SP_ANGEL 235
#define M_SP_MB 236
#define M_SP_MIRROR 237
#define M_SP_RAISE 238
#define M_SP_DEATH 239
#define M_SP_COURT 240
#define M_SP_LAIR 241
#define M_SP_PRIME 242

/* rangestrike functions */
#define M_STRIKE_MISSILE 301
#define M_STRIKE_FBOLT 302
#define M_STRIKE_LBALL 303
#define M_STRIKE_FBALL 304
#define M_STRIKE_BLIND 305
#define M_STRIKE_SNOWBALL 306
#define M_STRIKE_MASTER 307
#define M_STRIKE_SONIC 308

/* combat functions */
#define M_MELEE_NORMAL 401
#define M_MELEE_FIRE 402
#define M_MELEE_DRAGON 403
#define M_MELEE_MP 404
#define M_MELEE_ELEC 405
#define M_MELEE_POISON 406
#define M_MELEE_NG 407
#define M_MELEE_SUCCUBUS 408
#define M_MELEE_SPIRIT 409
#define M_MELEE_DISEASE 410
#define M_MELEE_SLEEP 411
#define M_MELEE_COLD 412
#define M_MELEE_MASTER 413
#define M_MELEE_GRAPPLE 414
#define M_MELEE_DEATH 415

/* movement functions */
#define M_MOVE_NORMAL 501
#define M_MOVE_FLUTTER 502
#define M_MOVE_TELEPORT 503
#define M_MOVE_FOLLOW 504
#define M_MOVE_RANDOM 505
#define M_MOVE_SMART 506
#define M_MOVE_SPIRIT 507
#define M_MOVE_SCAREDY 508
#define M_MOVE_CONFUSED 509
#define M_MOVE_ANIMAL 510
#define M_MOVE_LEASH 230

/* MLx -> index to Monsters starting for level x */
/* MLx -> number of monsters of level x or less */
/* NML_x -> number of monsters of level x */
/* NML-X must be changed whenever a monster is added */
/* This whole thing MUST be repaired.  Monster levels must
 * be represented elsewhere. */
#define ML0 0
#define NML_0 9
#define ML1 (ML0 + NML_0) /* 9 */
#define NML_1 22
#define ML2 (ML1 + NML_1) /* 31 */
#define NML_2 14
#define ML3 (ML2 + NML_2) /* 45 */
#define NML_3 15
#define ML4 (ML3 + NML_3) /* 60 */
#define NML_4 18
#define ML5 (ML4 + NML_4) /* 78 */
#define NML_5 14
#define ML6 (ML5 + NML_5) /* 92 */
#define NML_6 13
#define ML7 (ML6 + NML_6) /* 105 */
#define NML_7 15
#define ML8 (ML7 + NML_7) /* 120 */
#define NML_8 12
#define ML9 (ML8 + NML_8) /* 132 */
#define NML_9 8
#define ML10 (ML9 + NML_9) /* 140 */
#define NML_10 11

#define NUMMONSTERS (ML10 + NML_10)  /* 151 */

/* Some monster ID's : (Those that are explicitly named in code) */
/* Actually, there are still many magic constants floating around. */
/* Eventually I'll get around to making each monster's id a constant.... */
/* done, thanks to David Gibbs. */
#define RANDOM -1
#define HORNET (ML0+0)
#define MEND_PRIEST (ML0+1)
#define ITIN_MERCH (ML0+2)
#define GUARD (ML0+3)
#define NPC (ML0+4)
#define SHEEP (ML0+5)
#define MERCHANT (ML0+6)
#define ZERO_NPC (ML0+7)
#define HISCORE_NPC (ML0+8)
#define GRUNT (ML1+0)
#define TSETSE (ML1+1)
#define FNORD (ML1+2)
#define SEWER_RAT (ML1+3)
#define AGGRAVATOR (ML1+4)
#define BLIPPER (ML1+5)
#define GOBLIN (ML1+6)
#define PHANTASTICON (ML1+7)
#define ROBOT (ML1+8)
#define GEEK (ML1+9)
#define BOROGROVE (ML1+10)
#define QUAIL (ML1+11)
#define BADGER (ML1+12)
#define HAWK (ML1+13)
#define DEER (ML1+14)
#define CAMEL (ML1+15)
#define ANTEATER (ML1+16)
#define BUNNY (ML1+17)
#define TROUT (ML1+18)
#define BASS (ML1+19)
#define PARROT (ML1+20)
#define HYENA (ML1+21)
#define APPR_NINJA (ML2+0)
#define NIGHT_GAUNT (ML2+1)
#define SNEAK_THIEF (ML2+2)
#define EYE (ML2+3)
#define TOVE (ML2+4)
#define NASTY (ML2+5)
#define GHOST (ML2+6)
#define ENCHANTOR (ML2+7)  /* use 'OR' to avoid conflict with circle rank */
#define MURK (ML2+8)
#define GOBLIN_CHIEF (ML2+9)
#define WOLF (ML2+10)
#define ANT (ML2+11)
#define ELEPHANT (ML2+12)
#define HORSE (ML2+13)
#define SALAMANDER (ML3+0)
#define CATOBLEPAS (ML3+1)
#define L_FDEMON (ML3+2)
#define ACID_CLOUD (ML3+3)
#define PHANTOM (ML3+4)
#define GOBLIN_KING (ML3+5)
#define PTERODACTYL (ML3+6)
#define GOBLIN_SHAMAN (ML3+7)
#define LION (ML3+8)
#define BRIGAND (ML3+9)
#define BEAR (ML3+10)
#define MAMBA (ML3+11)
#define MANOWAR (ML3+12)
#define WEREHUMAN (ML3+13)
#define THOUGHTFORM (ML3+14)
#define MANTICORE (ML4+0)
#define TASMANIAN (ML4+1)
#define AUTO_MINOR (ML4+2)
#define DENEBIAN (ML4+3)
#define JUBJUB (ML4+4)
#define HAUNT (ML4+5)
#define INCUBUS (ML4+6)
#define SATYR (ML4+7)
#define CROC (ML4+8)
#define TORPOR (ML4+9)
#define DOBERMAN (ML4+10)
#define FUZZY (ML4+11)
#define SERV_LAW (ML4+12)
#define SERV_CHAOS (ML4+13)
#define SWARM (ML4+14)
#define BAN_SIDHE (ML4+15)
#define GRUE (ML4+16)
#define GENIN (ML4+17)
#define DRAGONETTE (ML5+0)
#define TESLA (ML5+1)
#define WYVERN (ML5+2)
#define CATEAGLE (ML5+3)
#define FROST_DEMON (ML5+4)
#define SPECTRE (ML5+5)
#define NECROMANCER (ML5+6)
#define SHADOW (ML5+7)
#define BOGTHING (ML5+8)
#define ASTRAL_VAMP (ML5+9)
#define LAVA_WORM (ML5+10)
#define MANABURST (ML5+11)
#define OUTER_DEMON (ML5+12)
#define MIRRORSHADE (ML5+13)
#define FIRE_ELEM (ML6+0)
#define AIR_ELEM (ML6+1)
#define WATER_ELEM (ML6+2)
#define EARTH_ELEM (ML6+3)
#define BANDERSNATCH (ML6+4)
#define LICHE (ML6+5)
#define TRITON (ML6+6)
#define MAST_THIEF (ML6+7)
#define TRICER (ML6+8)
#define RAKSHASA (ML6+9)
#define DEMON_SERP (ML6+10)
#define ANGEL (ML6+11)
#define CHUNIN (ML6+12)
#define BEHEMOTH (ML7+0)
#define NAZGUL (ML7+1)
#define UNICORN (ML7+2)
#define ROUS (ML7+3)
#define ILL_FIEND (ML7+4)
#define GREAT_WYRM (ML7+5)
#define FLAME_DEV (ML7+6)
#define LURKER (ML7+7)
#define SANDMAN (ML7+8)
#define MIRRORMAST (ML7+9)
#define ELDER_GRUE (ML7+10)
#define LOATHLY (ML7+11)
#define ZOMBIE (ML7+12)
#define RICOCHET (ML7+13)
#define INNER_DEMON (ML7+14)
#define GOOD_FAIRY (ML8+0)
#define BAD_FAIRY (ML8+1)
#define AUTO_MAJOR (ML8+2)
#define DRAGON (ML8+3)
#define JABBERWOCK (ML8+4)
#define FDEMON_L (ML8+5)
#define TIGERSHARK (ML8+6)
#define JONIN (ML8+7)
#define SHADOW_SLAY (ML8+8)
#define MIL_PRIEST (ML8+9)
#define COMA (ML8+10)
#define HIGH_ANGEL (ML8+11)
#define JOTUN (ML9+0)
#define INVIS_SLAY (ML9+1)
#define KING_WYV (ML9+2)
#define DEATHSTAR (ML9+3)
#define THAUMATURGIST (ML9+4)
#define VAMP_LORD (ML9+5)
#define ARCHANGEL (ML9+6)
#define DEMON_PRINCE (ML9+7)
#define DEATH (ML10+0)
#define EATER (ML10+1)
#define LAWBRINGER (ML10+2)
#define DRAGON_LORD (ML10+3)
#define DEMON_EMP (ML10+4)
#define LORD_EARTH (ML10+5)
#define LORD_AIR (ML10+6)
#define LORD_WATER (ML10+7)
#define LORD_FIRE (ML10+8)
#define ELEM_MASTER (ML10+9)
#define MAHARAJA (ML10+10)


/* location functions */
#define L_NO_OP 0

/* random sites, used many places */
#define L_LIFT 1
#define L_FIRE 2
#define L_ALTAR 3
#define L_TRIFID 4
#define L_RAISE_PORTCULLIS 5
/* spare entry: 6 */

/* city level shop and guild functions */
/* following are those in CitySiteList */
#define NUMCITYSITES 30
#define CITYSITEBASE 7
#define L_CHARITY 7
#define L_ARMORER 8
#define L_CLUB 9
#define L_GYM 10
#define L_THIEVES_GUILD 11
#define L_COLLEGE 12
#define L_HEALER 13
#define L_CASINO 14
#define L_TAVERN 15
#define L_MERC_GUILD 16
#define L_ALCHEMIST 17
#define L_SORCERORS 18
#define L_CASTLE 19
#define L_ARENA 20
#define L_DPW 21
#define L_LIBRARY 22
#define L_PAWN_SHOP 23
#define L_BANK 24
#define L_CONDO 25
#define L_ORACLE 26
#define L_ORDER 27
#define L_DINER 28
#define L_COMMANDANT 29
#define L_CRAP 30
#define L_TEMPLE 31
#define L_COUNTRYSIDE 32
#define L_BROTHEL 33
#define L_SEWER 34
#define L_TOURIST 35
#define L_MONASTERY 36 /* first available PGM */
/* end of city sites */

/* gap for a few more city sites, could be either list: 37, 38, 39, 40 */

/* other city sites, not in move list */
#define L_GARDEN 41
#define L_JAIL 42
#define L_MAZE 43      /* city */
#define L_CEMETARY 44  /* city */
#define L_VAULT 45     /* city */
#define L_MANSION 46   /* city */

/* sites in villages */
#define L_CARTOGRAPHER 47    /* villages */
#define L_STABLES 48         /* villages */
#define L_COMMONS 49         /* villages */
#define L_GRANARY 50         /* villages */
#define L_LAWSTONE 51
#define L_CHAOSTONE 52       /* shouldn't this be CHAOSSTONE ?? :) DAG */
#define L_SACRIFICESTONE 53
#define L_VOIDSTONE 54
#define L_BALANCESTONE 55
#define L_MINDSTONE 56

/* gap for a few more village or combined city/village sites: 57, 58, 59, 60 */
/* sites in city & villages */
#define L_HOVEL 61
#define L_HOUSE 62

/* sites in the countryside */
#define L_TEMPLE_WARNING 63  /* country */
#define L_ADEPT 64           /* country */
#define L_TACTICAL_EXIT 65   /* country, sort of */

/* allow a couple more country sites: 66, 67, 68 */

/* final abyss sites; ignore levitation */
#define L_EARTH_STATION 69
#define L_FIRE_STATION 70
#define L_WATER_STATION 71
#define L_AIR_STATION 72
#define L_VOID_STATION 73
#define L_VOID 74
#define L_VOICE1 75
#define L_VOICE2 76
#define L_VOICE3 77

/* circle hq sites */
#define L_TOME1 78
#define L_TOME2 79
#define L_ENTER_CIRCLE 80
#define L_CIRCLE_LIBRARY 81

/* other site functions, particular dungeons or special levels */

#define L_ARENA_EXIT 82    /* arena */
#define L_ENTER_COURT 83  /* castle */
#define L_THRONE 84       /* court of the archmage */
#define L_ESCALATOR 85    /* court of the archmage */
#define L_WHIRLWIND 86     /* astral */
#define L_HOUSE_EXIT 87    /* house */
#define L_SAFE 88          /* house */

/* unused site functions, these were defined, so I left them defined -- DAG */

#define L_OMEGA 89 /* not currently used */
#define L_DRUID 90  /* not currently used */
#define L_WARNING 91  /* not currently used */
#define L_OCCUPIED_HOUSE 92 /* not currently used */
#define L_FINAL_ABYSS 93 /* not currently used */

/* hold for 94, 95, 96, 97, 98, 99 still in the affect player while
   levitating range */

/* greater than LEVITATION_AVOIDANCE, no effect if player is levitating */
#define LEVITATION_AVOIDANCE 100

/* traps */
#define NUMTRAPS 13
#define TRAP_BASE 101

#define L_TRAP_DART 101
#define L_TRAP_PIT 102
#define L_TRAP_DOOR 103
#define L_TRAP_SNARE 104
#define L_TRAP_BLADE 105
#define L_TRAP_FIRE 106
#define L_TRAP_TELEPORT 107
#define L_TRAP_DISINTEGRATE 108
#define L_TRAP_SLEEP_GAS 109
#define L_TRAP_ACID 110
#define L_TRAP_MANADRAIN 111
#define L_TRAP_ABYSS 112
#define L_TRAP_SIREN 113

#define NUM_SAFE_TRAPS 7
/* the first NUM_SAFE_TRAPS are non-insta-lethal */

/* leave head room for extra traps: 114, 115, 116, 117, 118, 119, 120 */

/* random sites */
#define L_CHAOS 121
#define L_WATER 122
#define L_LAVA 123
#define L_ABYSS 124
#define L_MAGIC_POOL 125
#define L_PORTCULLIS_TRAP 126
#define L_PORTCULLIS 127
#define L_STATUE_WAKE 128
#define L_HEDGE 129
#define L_RUBBLE 130

#define L_DROP_EVERY_PORTCULLIS 131 /* tested, but never set */

/* unused site functions, these were defined, so I left them defined -- DAG */
#define L_STATUE_RANDOM 132  /* not currently used */

/* player possession slots */
/* slot 0 should not be filled when out of inventory_control() */

#define O_UP_IN_AIR 0
#define O_READY_HAND 1
#define O_WEAPON_HAND 2
#define O_LEFT_SHOULDER 3
#define O_RIGHT_SHOULDER 4
#define O_BELT1 5
#define O_BELT2 6
#define O_BELT3 7
#define O_SHIELD 8
#define O_ARMOR 9
#define O_BOOTS 10
#define O_CLOAK 11
#define O_RING1 12
#define O_RING2 13
#define O_RING3 14
#define O_RING4 15


/* Changing these would currently require changing the maps as well (although
 * there should be no other side effects).  I intend to remove the need for
 * this list later, by making the maps refer to each other (and thus removing
 * the need for the game to know about anything but map 0), but that comes
 * MUCH later. */
enum map_identifier {
    MAP_country=0,
    MAP_arena=1,
    MAP_circle=2,
    MAP_city=3,
    MAP_abyss=4,
    MAP_court=5,
    MAP_dlair=6,
    MAP_hedges=7,
    MAP_house=8,
    MAP_hovel=9,
    MAP_mansion=10,
    MAP_misle=11,
    MAP_skorch=12,
    MAP_speak=13,
    MAP_starview=14,
    MAP_stormwat=15,
    MAP_temple=16,
    MAP_thaumari=17,
    MAP_whorfen=18,
    MAP_woodmere=19,
};

/* typedefs needed by structs */

/* I don't ever define map_type (except in the C file where it's used); this
 * means that you _must_ declare only pointers to it.  I've defined, of
 * course, all of the needed accessor and mutator functions; see externs.h,
 * in the map.c section. */
struct map_type;
typedef struct map_type map;

/* structure definitions */

struct bank_account
{
    short player;
    long balance;
    long number;
    char *password;
    struct bank_account * next_account;
};
typedef struct bank_account bank_account;

struct room {
    int lighted;
    int left,right,top,bottom;
    int rsi; /* index into roomname switch */
};

struct spell {
    char known;
    char id;
    char powerdrain;
} ;

struct player {
    int str,con,dex,agi,iq,pow,maxstr,maxcon,maxdex,maxagi,maxiq,maxpow;
    long xp;
    int level,hp,maxhp,hit,dmg,absorption,speed,click;
    int defense,food,alignment;
    long mana,maxmana;
    long cash;
    int patron,birthday;
    char preference;
    int sx,sy; /* sanctuary coordinates */
    int x,y; /* current player coordinates */
    int itemweight,maxweight;
    int immunity[NUMIMMUNITIES];
    int status[NUMSTATI];
    long options;
    int rank[NUMRANKS];
    long guildxp[NUMRANKS];
    char name[64];
    char combatManeuvers[64];
    Object *possessions[MAXITEMS];
    Object *pack[MAXPACK];
    int packptr;
};

struct objectlist {
    Object *thing;
    struct objectlist *next;
};

/* terrain locations */
struct terrain {
    Symbol base_terrain_type;
    Symbol current_terrain_type;
    char aux;
    char status;
};

/* dungeon locations */
struct location {
    unsigned char p_locf; /* function executed when moved on */
    unsigned char lstatus; /* seen,stopsrun,lit,secret, */
    unsigned char roomnumber; /* so room can be named */
    Symbol locchar; /* terrain type */
    Symbol showchar; /*char instantaneously drawn for site */
    int aux; /* signifies various things */
    unsigned char buildaux; /* used in constructing level */
    struct objectlist *things;
    Monster *creature;
};

struct level {
    char depth; /* which level is this */
    struct level *next; /* pointer to next level in dungeon */
    struct location site[MAXWIDTH][MAXLENGTH]; /* dungeon data */
    char generated; /* has the level been made (visited) yet? */
    char numrooms; /* number of rooms on level */
    char tunnelled; /* amount of tunnelling done on this level */
    MonsterList *mlist; /* List of monsters on level */
    int environment; /* where kind of level is this? */
    int last_visited; /* time player was last on this level */
    int level_width; /* width of current level */
    int level_length; /* length of current level */
};

/* random typedef's */

typedef struct location loctype;
typedef loctype *plc;

typedef struct level levtype;
typedef levtype *plv;

typedef struct objectlist oltype;
typedef oltype *pol;

/* random  function declarations from system libraries */

#undef sign
#undef max
#undef min
#undef abs
/* These must be made to work for both longs and ints */
#define sign(n) (((n) < 0) ? -1 : (((n) > 0) ? 1 : 0))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define abs(n) (((n) < 0) ? (-(n)) : (n))

#define RANDFUNCTION genrand
#define SRANDFUNCTION sgenrand

/* WDT: This should be harmless under ANSI C, and will stop
 * some errors under bizarre platforms. */
#define pow2(n) (1L << (n))

/* these bit operations were functions, but are faster as macros... */

#define loc_statusp(x,y,stat) ((Level->site[x][y].lstatus&(stat))?1:0)
#define lset(x,y,stat) (Level->site[x][y].lstatus |= (stat))
#define lreset(x,y,stat) (Level->site[x][y].lstatus &= ~(stat))

#define c_statusp(x,y,stat) ((Country[x][y].status&(stat))?1:0)
#define c_set(x,y,stat) (Country[x][y].status |= (stat))
#define c_reset(x,y,stat) (Country[x][y].status &= ~(stat))

#define m_statusp(m,s) (((m)->status&(s))?1:0)
#define m_status_set(m,s) ((m)->status |= (s))
#define m_status_reset(m,s) ((m)->status &= ~(s))
#define m_immunityp(m,s) (((m)->immunity&pow2(s))?1:0)

#define optionp(o) ((Player.options&(o))?1:0)
#define optionset(o) (Player.options |= (o))
#define optionreset(o) (Player.options &= ~(o))

