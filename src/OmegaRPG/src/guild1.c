/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* guild1.c */
/* L_ functions  */ 

/* These functions implement the various guilds. */
/* They are all l_ functions since they are basically activated*/
/* at some site or other. */

#include "glob.h"

int merc_xps [COMMANDANT] =
{
  0, 400, 1500, 4000, -1
};

void l_merc_guild(void)
{
  print1("Legion of Destiny, Mercenary Guild, Inc.");

  if (nighttime())
    {
      print2("The barracks are under curfew right now.");
      return;
    }

  print2("You enter Legion HQ, ");
  if (Player.rank[LEGION] == COMMANDANT)
    {
      nprint2("Your aide follows you to the staff room.");
      morewait();
      clearmsg();
    }
  else if (Player.rank[LEGION] > 0)
    {
      nprint2("and report to your commander.");
      morewait();
    }

  switch (Player.rank[LEGION])
    {
    case 0:
      nprint2("and see the Recruiting Centurion.");
      morewait();
      print2("Do you wish to join the Legion? [yn] ");

      if ('y' == ynq2())
        {
          clearmsg();
          if (Player.rank[ARENA] > 0)
            {
              print1("The Centurion checks your record, and gets angry:");
              print2("'The legion don't need any Arena Jocks. Git!'");
            }
          else if (Player.rank[ORDER] > 0)
            {
              print1("The Centurion checks your record, and is amused:");
              print2("'A paladin in the ranks? You must be joking.'");
            }
          else if (Player.con < 12)
            {
              print1("The Centurion looks you over, sadly.");
              print2("You are too fragile to join the legion.");
            }
          else if (Player.str < 10)
            {
              print1("The Centurion looks at you contemptuously.");
              print2("Your strength is too low to pass the physical!");
            }
          else
            {
              pob newitem;
              bank_account * account;

              print1("You are tested for strength and stamina...");
              morewait();
              nprint1(" and you pass!");

              print2("Commandant ");
              nprint2(Commandant);
              nprint2(" shakes your hand.");
              morewait();

              print1("You are also issued a shortsword and leather.");
              print2("You are now a Legionaire.");
              morewait();
              clearmsg();

              /* gain shortsword */
              newitem = checkmalloc(sizeof(struct object));
              *newitem = Objects[OB_SHORT_SWORD];
              gain_item(newitem);

              /* gain leather armor */
              newitem = checkmalloc(sizeof(struct object));
              *newitem = Objects[OB_LEATHER];
              gain_item(newitem);

              cinema_scene("Your starting salary will be 500 AU per month paid into an account the",
                           "legion has set up for you. The password is \"loyalty\". Don't lose your card!", 0);

              /* gain bank card for salary account */
              account = bank_create_account(TRUE, 0, "loyalty");
              account->balance = 500;

              Objects[OB_DEBIT_CARD].known = 1;
              newitem = bank_create_card(account->number, OB_DEBIT_CARD);
              gain_item(newitem);

              clearmsg();

              SalaryAmount = 500;
              SalaryAccount = account->number;

              Player.rank[LEGION] = LEGIONAIRE;
              Player.guildxp[LEGION] = 1;
              Player.str++;
              Player.con++;
              Player.maxstr++;
              Player.maxcon++;
            }
        }
      break;

    case COMMANDANT:
      print1("You find the disposition of your forces satisfactory.");
      break;

    case COLONEL:
      if ((Player.level > Commandantlevel)
          && find_and_remove_item(CORPSEID, DEMON_EMP))
        {  
          print1("You liberated the Demon Emperor's Regalia!");
          morewait();
          clearmsg();

          print1("The Legion is assembled in salute to you!");
          print2("The Regalia is held high for all to see and admire.");
          morewait();
          clearmsg(); 

          print1("Commandant ");
          nprint1(Commandant);
          nprint1(" promotes you to replace him,");
          print2("and announces his own overdue retirement.");
          morewait();
          clearmsg();

          print1("You are the new Commandant of the Legion!");
          print2("The Emperor's Regalia is sold for a ridiculous sum.");
          morewait();
          clearmsg();

          print1("You now know the Spell of Regeneration.");
          print2("Your training is complete. You get top salary.");

          strcpy(Commandant, Player.name);
          Commandantlevel = Player.level;
          Commandantbehavior = fixnpc(4);
          save_hiscore_npc(8);

          Spells[S_REGENERATE].known = TRUE;
          Player.rank[LEGION] = COMMANDANT;
          Player.str += 2;
          Player.con += 2;
          Player.maxstr += 2;
          Player.maxcon += 2;

          SalaryAmount = 10000;
        }
      else if (Player.level <= Commandantlevel)
        {
          clearmsg();
          print1("Your CO expresses satisfaction with your progress.");
          print2("But your service record does not yet permit promotion.");
        }
      else
        {
          clearmsg();
          print1("Why do you come empty handed?"); 
          print2("You must return with the Regalia of the Demon Emperor!");
        }
      break;

    case FORCE_LEADER:
      clearmsg();
      print1("Your CO expresses satisfaction with your progress.");

      if (Player.guildxp[LEGION] < merc_xps[Player.rank[LEGION]])
        {
          print2("But your service record does not yet permit promotion.");
        }
      else
        {
          print2("You have been promoted to Legion Colonel!");
          morewait();

          print1("Your next promotion is contingent on the return of");
          print2("the Regalia of the Demon Emperor.");
          morewait();

          print1("The Demon Emperor holds court at the base of a volcano");
          print2("to the far south, in the heart of a swamp.");
          morewait();
          clearmsg();

          print1("You have been taught the spell of heroism!");
          print2("You are given advanced training, and a raise.");

          Spells[S_HERO].known = TRUE;
          Player.rank[LEGION]=COLONEL;
          Player.str++;
          Player.con++;
          Player.maxstr++;
          Player.maxcon++;

          SalaryAmount = 5000;
        }
      break;

    case CENTURION:
      clearmsg();
      print1("Your CO expresses satisfaction with your progress.");

      if (Player.guildxp[LEGION] < merc_xps[Player.rank[LEGION]])
        {
          print2("But your service record does not yet permit promotion.");
        }
      else
        {
          print2("You are now a Legion Force-Leader!");
          morewait();
          clearmsg();

          print1("You receive more training, and another raise.");

          Player.rank[LEGION]=FORCE_LEADER;
          Player.maxstr++;
          Player.str++;

          SalaryAmount = 2000;
        }
      break;

    case LEGIONAIRE:
      clearmsg();
      print1("Your CO expresses satisfaction with your progress.");

      if (Player.guildxp[LEGION] < merc_xps[Player.rank[LEGION]])
        {
          print2("But your service record does not yet permit promotion.");
        }
      else
        {
          print2("You are promoted to Legion Centurion!");
          morewait();
          clearmsg();

          print1("You get advanced training, and a higher salary.");

          Player.rank[LEGION] = CENTURION;
          Player.maxcon++;
          Player.con++;

          SalaryAmount = 1000;
        }
      break;
    }
}

void l_castle(void)
{
  pob o;
  int x, y;

  if (Player.level < 3) {
    print1("You can't possibly enter the castle, you nobody!");
    print2("Come back when you are famous.");
  }
  else {
    print1("You are ushered into the castle.");
    if (Player.rank[NOBILITY]<DUKE) {
      print2("His Grace, ");
      nprint2(Duke);
      nprint2("-- Duke of Rampart! <fanfare>");
      morewait();
      clearmsg();
    }
    if (Player.rank[NOBILITY]==0) {
      print1("Well, sirrah, wouldst embark on a quest? [yn] ");
      if (ynq1() == 'y') {
	print2("Splendid. Bring me the head of the Goblin King.");
	Player.rank[NOBILITY]=COMMONER;
      }
      else {
	print1("You scoundrel! Guards! Take this blackguard away!");
	morewait();
	p_damage(25,UNSTOPPABLE,"castle guards for lese majeste");
	send_to_jail();
      }
    }
    else if (Player.rank[NOBILITY]==COMMONER) {
      if (find_and_remove_item(CORPSEID,GOBLIN_KING)) {
	print1("Good job, sirrah! I promote you to the rank of esquire.");
	Player.rank[NOBILITY]=ESQUIRE;
	gain_experience(100);
	print2("Now that you have proved yourself true, another quest!");
	morewait();
	print1("Bring to me a Holy Defender!");
	print2("One is said to be in the possession of the Great Wyrm");
	morewait();
	clearmsg();
	print1("in the depths of the sewers below the city.");
      }
      else print2("Do not return until you achieve the quest, caitiff!");
    }
    else if (Player.rank[NOBILITY]==ESQUIRE) {
      if (find_and_remove_item(OB_DEFENDER,-1)) {
	print1("My thanks, squire. In return, I dub thee knight!");
	Player.rank[NOBILITY]=KNIGHT;
	gain_experience(1000);
	print2("If thou wouldst please me further...");
	morewait();
	print1("Bring me a suit of dragonscale armor.");
	print2("You might have to kill a dragon to get one....");
      }
      else print2("Greetings, squire. My sword? What, you don't have it?");
    }
    else if (Player.rank[NOBILITY]==KNIGHT) {
      if (find_and_remove_item(OB_DRAGONSCALE,-1)) {
	print1("Thanks, good sir knight.");
	print2("Here are letters patent to a peerage!");
	Player.rank[NOBILITY]=LORD;
	gain_experience(10000);
	morewait();
	print1("If you would do me a final service...");
	print2("I require the Orb of Mastery. If you would be so kind...");
	morewait();
	print1("By the way, you might find the Orb in the possession");
	print2("Of the Elemental Master on the Astral Plane");
      }
      else print2("Your quest is not yet complete, sir knight.");
    }
    else if (Player.rank[NOBILITY]==LORD) {
      if (find_item(&o,OB_ORB_MASTERY,-1)) {
	print1("My sincerest thanks, my lord.");
	print2("You have proved yourself a true paragon of chivalry");
	morewait();
	print1("I abdicate the Duchy in your favor....");
	print2("Oh, you can keep the Orb, by the way....");
	Player.rank[NOBILITY]=DUKE;
	gain_experience(10000);
	strcpy(Duke,Player.name);
	morewait();
	Dukebehavior = fixnpc(4);
	save_hiscore_npc(12);
	for (y = 52; y < 63; y++)
	    for (x = 2; x < 52; x++) {
		if (Level->site[x][y].p_locf == L_TRAP_SIREN) {
		    Level->site[x][y].p_locf = L_NO_OP;
		    lset(x, y, CHANGED);
		}
		if (x >= 12 && loc_statusp(x, y, SECRET)) {
		    lreset(x, y, SECRET);
		    lset(x, y, CHANGED);
		}
		if (x >= 20 && x <= 23 && y == 56) {
		    Level->site[x][y].locchar = FLOOR;
		    lset(x, y, CHANGED);
		}
	    }

      }
      else print2("I didn't really think you were up to the task....");
    }
  }
}


void l_arena(void)
{
  char response;
  pob newitem;
  int i,prize,monsterlevel;
  char *melee = NULL;

  print1("Rampart Coliseum");
  if (Player.rank[ARENA] == 0) {
    print2("Enter the games, or Register as a Gladiator? [e,r,ESCAPE] ");
    do response = (char) mcigetc();
    while ((response != 'e') && (response != 'r') && (response != ESCAPE));
  }
  else {
    print2("Enter the games? [yn] ");
    response = ynq2();
    if (response == 'y') response = 'e';
    else response = ESCAPE;
  }
  if (response == 'r') {
    if (Player.rank[ARENA]>0)
      print2("You're already a gladiator....");
    else if (Player.rank[ORDER]>0)
      print2("We don't let Paladins into our Guild.");
    else if (Player.rank[LEGION]>0)
      print2("We don't train no stinkin' mercs!");
    else if (Player.str < 13)
      print2("Yer too weak to train!");
    else if (Player.agi < 12)
      print2("Too clumsy to be a gladiator!");
    else {
      print1("Ok, yer now an Arena Trainee.");
      print2("Here's a wooden sword, and a shield");
      morewait();
      clearmsg();
      newitem = ((pob) checkmalloc(sizeof(objtype)));
      *newitem = Objects[OB_CLUB]; /* club */
      gain_item(newitem);
      newitem = ((pob) checkmalloc(sizeof(objtype)));
      *newitem = Objects[OB_LRG_RND_SHIELD]; /* shield */
      gain_item(newitem);
      Player.rank[ARENA] = TRAINEE;
      Arena_Opponent = 3;
      morewait();
      clearmsg();
      print1("You've got 5000Au credit at the Gym.");
      Gymcredit+=5000;
    }
  }
  else if (response == 'e') {
    print1("OK, we're arranging a match....");
    morewait();
    Arena_Monster = ((pmt) checkmalloc(sizeof(montype)));
    Arena_Victory = FALSE;
    switch(Arena_Opponent) {
    case 0: 
      *Arena_Monster = Monsters[GEEK];
      break;
    case 1:
      *Arena_Monster = Monsters[HORNET];
      break;
    case 2:
      *Arena_Monster = Monsters[HYENA];
      break;
    case 3:
      *Arena_Monster = Monsters[GOBLIN];
      break;
    case 4:
      *Arena_Monster = Monsters[GRUNT];
      break;
    case 5:
      *Arena_Monster = Monsters[TOVE];
      break;      
    case 6:
      *Arena_Monster = Monsters[APPR_NINJA];
      break;
    case 7:
      *Arena_Monster = Monsters[SALAMANDER];
      break;
    case 8:
      *Arena_Monster = Monsters[ANT];
      break;
    case 9:
      *Arena_Monster = Monsters[MANTICORE];
      break;
    case 10:
      *Arena_Monster = Monsters[SPECTRE];
      break;
    case 11:
      *Arena_Monster = Monsters[BANDERSNATCH];
      break;
    case 12:
      *Arena_Monster = Monsters[LICHE];
      break;
    case 13: 
      *Arena_Monster = Monsters[AUTO_MAJOR];
      break;
    case 14:
      *Arena_Monster = Monsters[JABBERWOCK];
      break;
    case 15:
      *Arena_Monster = Monsters[JOTUN];
      break;
    default:
      if ((Player.rank[ARENA] < 5) && (Player.rank[ARENA] > 0)) {
	strcpy(Str1,Champion);
	strcat(Str1,", the arena champion");
	*Arena_Monster = Monsters[HISCORE_NPC];
	Arena_Monster->monstring = salloc(Str1);
	strcpy(Str2,"The corpse of ");
	strcat(Str2,Str1);
	Arena_Monster->corpsestr = salloc(Str2);
	m_status_set( Arena_Monster, ALLOC );
	Arena_Monster->level = 20;
	Arena_Monster->hp = Championlevel*Championlevel*5;
	Arena_Monster->hit = Championlevel*4;
	Arena_Monster->ac = Championlevel*3;
	Arena_Monster->dmg = 100+Championlevel*2;
	Arena_Monster->xpv = Championlevel*Championlevel*5;
	Arena_Monster->speed = 3;
	melee = Arena_Monster->meleestr = (char *) checkmalloc(30*sizeof(char));
	strcpy(Arena_Monster->meleestr,"");
	for(i=0;i<Championlevel/5;i++)
	  strcat(Arena_Monster->meleestr,"L?R?");
	m_status_set(Arena_Monster, MOBILE);
	m_status_set(Arena_Monster, HOSTILE);
      }
      else {
	do 
	  i = random_range(ML9 - ML0) + ML0;
	while (i == NPC || i == HISCORE_NPC || i == ZERO_NPC ||
	       (Monsters[i].uniqueness != COMMON) || 
	       (Monsters[i].dmg == 0));
	*Arena_Monster = Monsters[i];
      }
      break;
    }
    monsterlevel = Arena_Monster->level;
    if (Arena_Monster->level != 20) {
      strcpy(Str1,nameprint());
      strcat(Str1," the ");
      strcat(Str1,Arena_Monster->monstring);
      Arena_Monster->monstring = salloc(Str1);
      strcpy(Str2,"The corpse of ");
      strcat(Str2,Str1);
      Arena_Monster->corpsestr = salloc(Str2);
      m_status_set( Arena_Monster, ALLOC );
    }
    Arena_Monster->uniqueness = UNIQUE_MADE;
    print1("You have a challenger: ");
    print2(Arena_Monster->monstring);
    Arena_Monster->attacked = TRUE;
    m_status_set(Arena_Monster,HOSTILE);
/* DAG  pump up the stats of the arena monster; from env.c */
/* DAG should we even do this for the champion? */
    Arena_Monster->hp += Arena_Monster->level*10;
    Arena_Monster->hit += Arena_Monster->hit;
    Arena_Monster->dmg += Arena_Monster->dmg/2;

    morewait();
    clearmsg();
    change_environment(E_ARENA);
    print1("Let the battle begin....");

    time_clock(TRUE);
    while (Current_Environment == E_ARENA) 
      time_clock(FALSE);

    /* DAG all this nasty mess cleaned up...  */
    /* one process with m_status_set( Arena_Monster, ALLOC) */

    /* free(name); */
    /* free(corpse); */

    if (melee)
      free(melee);
    if (! Arena_Victory) {
      print1("The crowd boos your craven behavior!!!");
      if (Player.rank[ARENA] > 0) {
	print2("You are thrown out of the Gladiator's Guild!");
	morewait();
	clearmsg();
	if (Gymcredit > 0) print1("Your credit at the gym is cut off!");
	Gymcredit = 0;
	Player.rank[ARENA] = -1;
      }
    }      
    else {
      Arena_Opponent++;
      if (monsterlevel == 20) {
	print1("The crowd roars its approval!");
	if (Player.rank[ARENA]) {
	  print2("You are the new Arena Champion!");
	  Championlevel = Player.level;
	  strcpy(Champion,Player.name);
	  Player.rank[ARENA] = 5;
	  morewait();
	  Championbehavior = fixnpc(4);
	  save_hiscore_npc(11);
	  print1("You are awarded the Champion's Spear: Victrix!");
	  morewait();
	  newitem = ((pob) checkmalloc(sizeof(objtype)));
	  *newitem = Objects[OB_VICTRIX];
	  gain_item(newitem);
	  
	}
	else {
	  print1("As you are not an official gladiator,");
	  nprint1("you are not made Champion.");
	  morewait();
	}
      }
      morewait();
      clearmsg();
      print1("Good fight! ");
      nprint1("Your prize is: ");
      prize = max(25,monsterlevel * 50);
      if (Player.rank[ARENA] > 0) prize *= 2;
      mnumprint(prize);
      nprint1("Au.");
      Player.cash+=prize;
      if ((Player.rank[ARENA]<4) &&
	  (Arena_Opponent>5) &&
	  (Arena_Opponent % 3 == 0)) {
	if (Player.rank[ARENA]>0) {
	  Player.rank[ARENA]++;
	  morewait();
	  print1("You've been promoted to a stronger class!");
	  print2("You are also entitled to additional training.");
	  Gymcredit+=Arena_Opponent*1000;
	}
      }
    }
    xredraw();
  }
  else clearmsg();
}
