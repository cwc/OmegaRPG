/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988, 1989 */
/* stats.c */

/* this file contains one function, editstats(), that allows the player
 * (if in cheat mode) to alter their stats, HP, mana, gold, etc.
 *
 * a later version may add options to set other things like guild
 * membership, alignment, and so on
 */

#ifndef MSDOS_SUPPORTED_ANTIQUE
#include <unistd.h>
#include <ctype.h>
#endif

#include "glob.h"

void editstats(void)
{
  int slot = 1, to, done = FALSE;
  int response, num;

  clearmsg();
  menuclear();

  display_stats();

  move_slot(1,1,NUMSTATS);

  do {
    print1("Currently selected stat is preceded by highlit >>");
    print2("Move selected option with '>' and '<', ' ' to change value, ESCAPE to quit");

    response = mcigetc();
    switch(response) {
    case 'j':
    case '>':
#ifdef KEY_DOWN
    case KEY_DOWN:
#endif
      to = slot + 1;
      slot = move_slot(slot,to,NUMSTATS+1);
      break;
    case 'k':
    case '<':
#ifdef KEY_UP
    case KEY_UP:
#endif
      to = slot - 1;
      if (to > 0)
        slot = move_slot(slot,to, NUMSTATS+1);
      break;
    case ESCAPE:
      done = TRUE;
      break;
    case ' ':
      print1("enter new value:");
      num = (int) parsenum("");
      if ((slot < 7) && ((num > 32) || (num < 1)))
      {
        print1("You don't really think I'll let you get away with that, do you?");
        morewait();
        break;
      }
      switch(slot)
      {
      case 1:
        Player.str = Player.maxstr = num;
        break;
      case 2:
        Player.con = Player.maxcon = num;
        break;
      case 3:
        Player.dex = Player.maxdex = num;
        break;
      case 4:
        Player.agi = Player.maxagi = num;
        break;
      case 5:
        Player.iq  = Player.maxiq  = num;
        break;
      case 6:
        Player.pow = Player.maxpow = num;
        break;
      case 7:
        Player.hp                  = num;
        break;
      case 8:
        Player.maxhp               = num;
        break;
      case 9:
        Player.mana                = num;
        break;
      case 10:
        Player.maxmana             = num;
        break;
      case 11:
        Player.cash                = num;
        break;
      }
      calc_melee();
      break;
    default:
      print3("That response is meaningless for this option.");
      break;
    }
    print1("Currently selected stat is preceded by highlit >>");
    display_stat_slot(slot/*,slot,NUMSTATS+1 WDT HACK! */);
    move_slot(slot,slot,NUMSTATS+1);
  } while (! done);
  if (optionp(SHOW_COLOUR))
    colour_on();
  else
    colour_off();
#if !defined(MSDOS_SUPPORTED_ANTIQUE) && !defined(AMIGA)
  xredraw();
#endif
}

