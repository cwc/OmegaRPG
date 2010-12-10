/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* char.c */
/* Player generation */

#ifndef MSDOS_SUPPORTED_ANTIQUE
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#endif

#include "glob.h"

/* set player to begin with */
int initplayer(void)
{
  int i;
  int oldchar=FALSE;
  FILE *fd;
  char *lname;
  int ret_value = FALSE;
#ifndef MSDOS
  struct passwd *dastuff;
#endif

  lname = getlogin();
#ifndef MSDOS
  if (!lname || strlen(lname) == 0)
    {
      dastuff = getpwuid(getuid());
      lname = dastuff->pw_name;
    }
#endif
  strcpy(Player.name,lname);
  if (Player.name[0] >= 'a' && Player.name[0] <= 'z')
    Player.name[0] += 'A'-'a'; /* capitalise 1st letter */
  Player.itemweight = 0;
  Player.food = 36; 
  Player.packptr = 0;
  Behavior = -1;
  Player.options = 0;
  for (i=0;i<MAXITEMS;i++)
    Player.possessions[i] = NULL;
  for (i=0;i<MAXPACK;i++)
    Player.pack[i] = NULL;
  for (i=0;i<NUMIMMUNITIES;i++) Player.immunity[i] = 0;
  for (i=0;i<NUMSTATI;i++) Player.status[i] = 0;
  for (i=0;i<NUMRANKS;i++) {
    Player.rank[i] = 0;
    Player.guildxp[i] = 0;
  }
  Player.patron = 0;
  Player.alignment = 0;
  Player.cash = 250;
  change_to_user_perms();
  if ((fd=omegarc_check())!=NULL) {
    fread((char *)&i,sizeof(int),1,fd);
    if (i != VERSION) {
#if defined(MSDOS) || defined(AMIGA)
      print1("Out of date omega.rc! Make another!");
#else
      print1("Out of date .omegarc! Make another!");
#endif
      morewait();
    }
    else {
      oldchar = TRUE;
      fread((char *)&Player,sizeof(Player),1,fd);
      fread((char *)&Searchnum,sizeof(int),1,fd);
      fread((char *)&Verbosity,sizeof(char),1,fd);
      strcpy(Player.name,lname);
      if (Player.name[0] >= 'a' && Player.name[0] <= 'z')
        Player.name[0] += 'A'-'a'; /* capitalise 1st letter */
    }
    fclose(fd);
  }
  change_to_game_perms();
  if (! oldchar) {
    optionset(RUNSTOP);
    optionset(CONFIRM);
#ifdef COMPRESS_SAVE_FILES
    optionset(COMPRESS_OPTION);
#endif
    optionset(SHOW_COLOUR);
    ret_value = initstats() ; /* RM 04-19-2000:loading patch */ /* DAG */
  }
  /* This gets executed when the player loads from .omegarc */
  /* DAG - put the code back in the same place, rather than duplicating */
  Searchnum = max(1,min(9,Searchnum));
  Player.hp = Player.maxhp = Player.maxcon;
  Player.mana = Player.maxmana = calcmana();
  Player.click = 1;
  strcpy(Player.meleestr,"CCBC");
  calc_melee();
  ScreenOffset = -1000;	/* to force a redraw */
  return ret_value; /* RM 04-19-2000: loading patch */ /* DAG */
}


FILE *omegarc_check(void)
{
  FILE *fd;
#if defined(MSDOS) || defined(AMIGA)
  if ((fd = fopen("omega.rc","rb")) != NULL) {
    print2("Use omega.rc charcter record in current directory? [yn] ");
#else
    sprintf(Str1, "%s/.omegarc", getenv("HOME"));
    if ((fd = fopen(Str1,"r")) != NULL) {
      print2("Use .omegarc in home directory? [yn] ");
#endif
      if (ynq2()!='y') {
        fclose(fd);
        fd = NULL;
      }
    }
    clearmsg();
    return(fd);
  }

int initstats(void)
{
  char response;
  char savedg[80];
  print1("Do you want to run a character [c], load a game [l], or ");
  print2("play yourself [p]?"); /* RM 04-19-2000 loading patch */
  do 
    response = (char) mcigetc(); 
  while ((response!='c')&&(response != 'p')&&(response !='l'));
  if (response == 'c') omegan_character_stats();
  else if (response == 'l')
  {
        /* RM 04-19-2000: loading patch - a blatant hack */
        clearmsg();
        print1("Enter saved game name: ");
        strcpy(savedg,msgscanstring());

        game_restore(savedg);
        return TRUE;
  }
  else {
    clearmsg(); /* RM 04-19-2000 loading patch - fix the display */
    user_character_stats();
    user_intro();
#if defined(MSDOS) || defined(AMIGA)
    print1("Do you want to save this set-up to omega.rc in this directory? [yn] ");
#else
    print1("Do you want to save this set-up to .omegarc in your home directory? [yn] ");
#endif
    if (ynq1()=='y')
      save_omegarc();
  }
  xredraw();
  return FALSE;
}

void save_omegarc(void)
{
  int i=VERSION;
  FILE *fd;
  change_to_user_perms();
#if defined(MSDOS) || defined(AMIGA)
  fd = fopen("omega.rc","wb");
#else
  sprintf(Str1, "%s/.omegarc", getenv("HOME"));
  fd = fopen(Str1,"w");
#endif
  if (fd == NULL)
#if defined(MSDOS) || defined(AMIGA)
    print1("Sorry, couldn't save omega.rc for some reason.");
#else
    print1("Sorry, couldn't save .omegarc for some reason.");
#endif
  else {
    fwrite((char *)&i,sizeof(int),1,fd);
    print1("First, set options.");
    setoptions();
    fwrite((char *)&Player,sizeof(Player),1,fd);
    fwrite((char *)&Searchnum,sizeof(int),1,fd);
    fwrite((char *)&Verbosity,sizeof(char),1,fd);
    fclose(fd);
  }
  change_to_game_perms();
}



long calcmana(void)
{
  return(Player.pow * (long)(Player.level+1));
}


/*  npcbehavior digits 1234

4 : alignment (LAWFUL,CHAOTIC, or NEUTRAL)
3 : primary combat action (melee,missile,spell,thief,flight,1..5)
2 : competence at 4 (0..9, 0 = incompetent, 9 = masterful)
1 : conversation mode

status : 1 = dead, 2 = saved, 3 = retired, 4 = still playing
*/
int fixnpc(int status)
{
  int npcbehavior=0;
  char response;
  if (status == 1) { /* player is dead, all undead are chaotic */
    npcbehavior+=CHAOTIC;
    npcbehavior+=10; /* melee */
    npcbehavior+=100*min(9,((int) (Player.level/3)));
    npcbehavior+=1000; /* threaten */
  }
  else if (Behavior >= 0)
    npcbehavior = Behavior;
  else {
    menuclear();
    menuprint("NPC Behavior Determination Module\n\n");
    menuprint("Your overall NPC behavior is:");
    if (Player.alignment < -10) {
      npcbehavior += CHAOTIC;
      menuprint("\n\n CHAOTIC");
    }
    else if (Player.alignment > 10) {
      npcbehavior += LAWFUL;
      menuprint("\n\n LAWFUL");
    }
    else {
      npcbehavior += NEUTRAL;
      menuprint("\n\n NEUTRAL");
    }
    menuprint("\n\n1: hand-to-hand combat");
    menuprint("\n2: missile combat");
    menuprint("\n3: spellcasting");
    menuprint("\n4: thieving");
    menuprint("\n5: escape");
    menuprint("\n\nEnter NPC response to combat: ");
    showmenu();
    response = '0';
    while ((response != '1') && 
	   (response != '2') &&
	   (response != '3') &&
	   (response != '4') &&
	   (response != '5'))
      response = menugetc();
    menuaddch(response);
    npcbehavior+=10*(response - '0');
    npcbehavior+=100*competence_check(response-'0');
    response = '0';
    menuclear();
    menuprint("1: threaten");
    menuprint("\n2: greet");
    menuprint("\n3: aid");
    menuprint("\n4: beg");
    menuprint("\n5: silence");
    menuprint("\n\nEnter NPC response to conversation: ");
    showmenu();
    while ((response != '1') && 
	   (response != '2') &&
	   (response != '3') &&
	   (response != '4') &&
	   (response != '5'))
      response = menugetc();
    menuaddch(response);
    npcbehavior+=1000*(response - '0');
    xredraw();
  }
  Behavior = npcbehavior;
  return(npcbehavior);
}


/* estimates on a 0..9 scale how good a player is at something */
int competence_check(int attack)
{
  int ability = 0;
  switch(attack) {
  case 1: /* melee */
    ability += statmod(Player.str);
  case 2: /* missle */
    ability += statmod(Player.dex);
    ability += Player.rank[LEGION];
    ability += ((int) (Player.dmg / 10) - 1);
    break;
  case 3: /* spellcasting */
    ability += statmod(Player.iq);
    ability += statmod(Player.pow);
    ability += Player.rank[CIRCLE];
    ability += Player.rank[COLLEGE];
    ability += Player.rank[PRIEST];
    break;
  case 4: /* thieving */
    ability += statmod(Player.dex);
    ability += statmod(Player.agi);
    ability += Player.rank[THIEVES];
    break;
  case 5: /* escape */
    ability += 2 * statmod(Player.agi);
    break;
  }
  ability += ((int) (Player.level / 5));
  if (ability < 0) ability = 0;
  if (ability > 9) ability = 9;
  return(ability);
}

static int user_character_strength (void)
{
  int num;

  cinema_blank();
  num = (int)cinema_getnum_line(0, "How many pounds can you bench press? ");

  if (num < 30) return 3;
  if (num < 90) return num / 10;
  
  num = (num - 120) / 30 + 9;

  if (num > 18)
    {
      num = 18;
      cinema_scene("Right, make me believe THAT's true.",
                   "Even if it's true, I don't believe it.", 0);
    }

  return num;
}

static int user_character_iq_test (void)
{
  int num = 0;

  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Took an official IQ test? [yn] "))
    {
      num = (int)cinema_getnum_line(1, "So, whadja get? ");
      num /= 10;

      if (num > 18)
        {
          num = 18;
          cinema_scene("I'm not convinced...",
                       "If you're so smart, why aren't you rich?", 0);
        }
    }

  return num;
}

static int user_character_undergraduate_exam (void)
{
  int num = 0;
  char * prompt = "Took Undergraduate entrance exams? [yn] ";

  cinema_blank();
  if ('y' == cinema_ynq_line(0, prompt))
    {
      while (1)
        {
          num = (int)cinema_getnum_line(1, "So, what percentile? ");
          if (num < 100) break;

          cinema_scene("That's impossible!", 0, 0);
          cinema_print_line(0, prompt);
        }

      num = 9 * (num - 49) / 50 + 9;
    }

  return num;
}

static int user_character_graduate_exam (void)
{
  int num = 0;
  char * prompt = "Took Graduate entrance exams? [yn] ";

  cinema_blank();
  if ('y' == cinema_ynq_line(0, prompt))
    {
      while (1)
        {
          num = (int)cinema_getnum_line(1, "So, what percentile? ");
          if (num < 100) break;

          cinema_scene("That's impossible!", 0, 0);
          cinema_print_line(0, prompt);
        }

      num = 9 * (num - 49) / 50 + 9;
    }

  return num;
}

static int user_character_dumb (void)
{
  int num;

  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Pretty dumb, aren't you? [yn] "))
    {
      num = 3 + random_range(3);
      cinema_print_line(1, "I thought so...");
    }
  else
    {
      num = 6 + random_range(6);
      cinema_print_line(1, "Well, not *that* dumb.");
    }

  morewait();
  return num;
}

static int user_character_dance (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Can you dance? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "Well? [yn] ")) return 1;
  return 3;
}

static int user_character_martial_art (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you have training in a martial art or gymnastics? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "Do you have dan rank or equivalent? [yn] ")) return 2;
  return 6;
}

static int user_character_field_sport (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you play some field sport? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "Are you good? [yn] ")) return 1;
  return 2;
}

static int user_character_cave (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you cave, mountaineer, etc.? [yn] ")) return 0;
  return 3;
}

static int user_character_skate (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you skate or ski? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "Well? [yn] ")) return 2;
  return 4;
}

static int user_character_handicapped (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Are you physically handicapped? [yn] ")) return 0;
  return -4;
}

static int user_character_accident_prone (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Are you accident prone? [yn] ")) return 0;
  return -4;
}

static int user_character_bicycle (void)
{
  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Can you ride a bicycle? [yn] ")) return 0;
  return -4;
}

static int user_character_video_games (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you play video games? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "Do you get high scores? [yn] ")) return 2;
  return 6;
}

static int user_character_archer (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Are you an archer, fencer, or marksman? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "A good one? [yn] ")) return 2;
  return 6;
}

static int user_character_picked_lock (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Have you ever picked a lock? [yn] ")) return 0;

  cinema_print_line(1, "Really? Well, the police are being notified.");
  usleep(250000);
  cinema_print_line(1, "Really? Well, the police are being notified..");
  usleep(250000);
  cinema_print_line(1, "Really? Well, the police are being notified...");
  usleep(250000);
  cinema_print_line(1, "Really? Well, the police are being notified....");
  usleep(250000);
  cinema_print_line(1, "Really? Well, the police are being notified.... done!");
  morewait();

  return 2;
}

static int user_character_typing_speed (void)
{
  int num;

  cinema_blank();
  num = (int)cinema_getnum_line(0, "What's your typing speed (words per minute)? ");

  if (num > 124)
    {
      num = 124;
      cinema_print_line(1, "Tell me another one...");
      morewait();
    }

  return num / 25;
}

static int user_character_hand_shakes (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Hold your arm out. Tense your fist. Hand shaking? [yn] ")) return 0;
  return -3;
}

static int user_character_ambidextrous (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Ambidextrous, are you? [yn] ")) return 0;
  return 4;
}

static int user_character_can_cut_deck (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Can you cut a deck of cards with one hand? [yn] ")) return 0;
  return 2;
}

static int user_character_can_tie_shoes (void)
{
  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Can you tie your shoes blindfolded? [yn] ")) return 0;
  return -3;
}

static int user_character_colds (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you ever get colds? [yn] ")) return 4;
  if ('y' != cinema_ynq_line(1, "Frequently? [yn] ")) return 0;
  return -4;
}

static int user_character_recent_accident (void)
{
  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Had any serious accident or illness this year? [yn] ")) return -4;
  return 4;
}

static int user_character_chronic_disease (void)
{
  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Have a chronic disease? [yn] ")) return -4;
  return 0;
}

static int user_character_overweight (void)
{
  cinema_blank();
  if ('y' == cinema_ynq_line(0, "Overweight or underweight by more than 20 percent? [yn] ")) return -2;
  return 0;
}

static int user_character_high_blood_pressure (void)
{
  cinema_blank();
  if ('y' == cinema_ynq_line(0, "High blood pressure? [yn] ")) return -2;
  return 0;
}

static int user_character_smoke (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you smoke? [yn] ")) return 0;

  cinema_print_line(1, "*cough*");
  morewait();

  return -3;
}

static int user_character_aerobics (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Take aerobics classes? [yn] ")) return 0;
  return 2;
}

static int user_character_miles_run (void)
{
  int num;

  cinema_blank();
  num = (int)cinema_getnum_line(0, "How many miles can you run? ");

  if (num < 1) return -3;
  if (num < 5) return 2;
  if (num < 10) return 4;
  if (num < 26) return 8;

  cinema_print_line(1, "Right. Sure. Give me a break.");
  morewait();

  return 8;
}

static int user_character_animals (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do animals react oddly to your presence? [yn] ")) return 0;

  cinema_print_line(1, "How curious that must be.");
  morewait();

  return 2;
}

static int user_character_auras (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Can you see auras? [yn] ")) return 0;

  cinema_print_line(1, "How strange.");
  morewait();

  return 3;
}

static int user_character_out_of_body (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Ever have an out-of-body experience? [yn] ")) return 0;

  cinema_print_line(1, "Wow, man! Fly the friendly skies...");
  morewait();

  return 3;
}

static int user_character_spell (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Did you ever cast a spell? [yn] ")) return 0;
  if ('y' != cinema_ynq_line(1, "Did it work? [yn] ")) return 3;

  cinema_print_line(2, "Sure it did...");
  morewait();

  return 7;
}

static int user_character_esp (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you have ESP? [yn] ")) return 0;

  cinema_print_line(1, "Somehow, I knew you were going to say that.");
  morewait();

  return 3;
}

static int user_character_pk (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you have PK? [yn] ")) return 0;

  cinema_print_line(1, "I can't tell you how much that moves me.");
  morewait();

  return 6;
}

static int user_character_ghosts (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Do you believe in ghosts? [yn] ")) return 0;

  cinema_print_line(1, "I do! I do! I do believe in ghosts!");
  morewait();

  return 2;
}

static int user_character_irish (void)
{
  cinema_blank();
  if ('y' != cinema_ynq_line(0, "Are you Irish? [yn] ")) return 0;

  cinema_print_line(1, "Is that blarney or what?");
  morewait();

  return 2;
}

void user_character_stats(void)
{
  int num;
  int iqpts = 0;
  int agipts = 0;
  int dexpts = 0;
  int powpts = 0;
  int conpts = 0;
  int numints = 0;

  cinema_scene("OK, now try to answer the following questions honestly:", 0, 0);

  Player.str = Player.maxstr = user_character_strength();

  num = user_character_iq_test();
  if (num) ++numints;
  iqpts += num;

  num = user_character_undergraduate_exam();
  if (num) ++numints;
  iqpts += num;

  num = user_character_graduate_exam();
  if (num) ++numints;
  iqpts += num;

  if (0 == numints)
    Player.iq = user_character_dumb();
  else
    Player.iq = iqpts / numints;

  Player.maxiq = Player.iq;

  agipts += user_character_dance();
  agipts += user_character_martial_art();
  agipts += user_character_field_sport();
  agipts += user_character_cave();
  agipts += user_character_skate();
  agipts += user_character_handicapped();
  agipts += user_character_accident_prone();
  agipts += user_character_bicycle();

  Player.agi = Player.maxagi = 9 + agipts / 2;

  dexpts += user_character_video_games();
  dexpts += user_character_archer();
  dexpts += user_character_picked_lock();
  dexpts += user_character_typing_speed();
  dexpts += user_character_hand_shakes();
  dexpts += user_character_ambidextrous();
  dexpts += user_character_can_cut_deck();
  dexpts += user_character_can_tie_shoes();

  Player.dex = Player.maxdex = 6 + dexpts / 2;

  conpts += user_character_colds();
  conpts += user_character_recent_accident();
  conpts += user_character_chronic_disease();
  conpts += user_character_overweight();
  conpts += user_character_high_blood_pressure();
  conpts += user_character_smoke();
  conpts += user_character_aerobics();
  conpts += user_character_miles_run();

  Player.con = Player.maxcon = 12 + conpts / 3;

  powpts += user_character_animals();
  powpts += user_character_auras();
  powpts += user_character_out_of_body();
  powpts += user_character_spell();
  powpts += user_character_esp();
  powpts += user_character_pk();
  powpts += user_character_ghosts();
  powpts += user_character_irish();

  Player.pow = Player.maxpow = 3 + powpts / 2;

  Player.preference =
    cinema_interact("mfyn", "Are you sexually interested in males or females? ", 0, 0);
}



void omegan_character_stats(void)
{
  int share1,share2,i=0;
  print1("To reroll hit ESCAPE; hit any other key to accept these stats.");
  do {
    i++;
#if REROLLS == -1
    sprintf(Str1, "Generated character # %d", i );
#else
    sprintf(Str1, "You have only %d chance%s to reroll... ", REROLLS - i,
            (i == (REROLLS-1) ) ? "":"s");
#endif
    print2(Str1);
    Player.iq = Player.maxiq = 4 + random_range(5)+
      (share1 = random_range(6)) + (share2 = random_range(6));
    Player.pow = Player.maxpow = 4 + random_range(5) + share1 +share2;
    Player.dex = Player.maxdex = 4 + random_range(5)+
      (share1 = random_range(6)) + (share2 = random_range(6));
    Player.agi = Player.maxagi = 4 + random_range(5) + share1 +share2;
    Player.str = Player.maxstr = 4 + random_range(5)+
      (share1 = random_range(6)) + (share2 = random_range(6));
    Player.con = Player.maxcon = 4 + random_range(5) + share1 +share2;
    Player.cash = random_range(100)+random_range(100)+
      random_range(100)+random_range(100)+random_range(100);
    Player.hp=Player.maxhp=Player.con;
    Player.mana=Player.maxmana = calcmana();
    calc_melee();
    dataprint();
#if REROLLS == -1
  } while (mgetc() == ESCAPE);
#else
  } while ((i < REROLLS) && (mgetc() == ESCAPE));
#endif
  clearmsg();
  print1("Please enter your character's name: ");
  strcpy(Player.name,msgscanstring());
  if (Player.name[0] >= 'a' && Player.name[0] <= 'z')
    Player.name[0] += 'A'-'a'; /* capitalise 1st letter */
  print1("Is your character sexually interested in males or females? [mf] ");
  do Player.preference = (char) mcigetc();
  while ((Player.preference != 'm') && (Player.preference != 'f') &&
	(Player.preference != 'y') && (Player.preference != 'n')); /* :-) */

}

