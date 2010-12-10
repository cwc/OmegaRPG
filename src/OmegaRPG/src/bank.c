/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* bank.c */
/* new bank -- moved out of site1.c */

#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include "glob.h"

/* swiped from scr.c */
#if defined(MSDOS_SUPPORTED_ANTIQUE) || defined(AMIGA)
# define CHARATTR(c) ((c) >> 8)
#else
# define CHARATTR(c) ((c) & ~0xff)
#endif

/* defined in scr.c */
extern WINDOW *Bankw;

/* predefined account info */
#define CASH_BALANCE 500
#define CASH_PASSWORD "cash"

#define NUM_NPC_ACCOUNTS 30

char * npc_account_passwords [NUM_NPC_ACCOUNTS] =
{
  "Duke",
  "Duke Nukem",
  "Dukes Ca$h",
  "SexyNoreen",
  "ExSuccubus",
  "Hooker Sue",
  "JungleGirl",
  "Satyrical",
  "Fangless",
  "Big Daddy",
  "PraiseEros",
  "Sonders",
  "CSRFLP",
  "Order",
  "Diner",
  "LC****",
  "Eclipse",
  "The Circle",
  "College",
  "FinanceAid",
  "Librarian",
  "RTG, Inc.",
  "Alchemy",
  "LOD Mercs",
  "KnightPawn",
  "Julie",
  "RampartGym",
  "Arena",
  "Slongo",
  "Bedwyr"
};

int npc_account_balances [NUM_NPC_ACCOUNTS] =
{
  3000,
  30000,
  300000,
  100000,
  75000,
  125000,
  50000,
  75000,
  50000,
  100000,
  50000,
  10000,
  300000,
  3000,
  3000,
  50000,
  5000,
  25000,
  5000,
  40000,
  10000,
  500000,
  10000,
  200000,
  75000,
  10000,
  40000,
  3000,
  2000,
  2000
};

static int account_num [NUM_NPC_ACCOUNTS];
static char * blank_line = "                                               ";
static int account_number_next;
static int account_number_incr;

/* create a new bank account and add it to the bank */
bank_account * bank_create_account (int player, int balance, char *password)
{
  bank_account * new_account;

  /* no-password accounts aren't allowed */
  assert(password);

  /* allocate space */
  new_account = checkmalloc(sizeof(bank_account));

  /* copy fields */
  new_account->player = player;
  new_account->balance = balance;
  new_account->next_account = 0;
  new_account->number = account_number_next;
  account_number_next += account_number_incr;

  /* allocate and copy the password */
  new_account->password = checkmalloc(1 + strlen(password));
  strcpy(new_account->password, password);

  /* add the new account to the bank */
  new_account->next_account = bank;
  bank = new_account;

  return new_account;
}

struct object * bank_create_card (int account_number, int type)
{
  struct object * card;

  assert(OB_DEBIT_CARD == type || OB_SMART_CARD == type);

  card = checkmalloc(sizeof(struct object));
  *card = Objects[type];
  card->aux = account_number;

  return card;
}

/* find an account in the bank, given the password */
bank_account * bank_index_password (char * password)
{
  bank_account * account;

  for(account = bank; account; account = account->next_account)
    if (!strcmp(password, account->password)) break;

  return account;
}

/* find an account in the bank, given the account number */
bank_account * bank_index_number (int number)
{
  bank_account * account;

  for(account = bank; account; account = account->next_account)
    if (number == account->number) break;

  return account;
}

/* allow user to type in a new password with simple editing */
/* returns the length of the typed-in string */
static int input_password (WINDOW *w, int line, int col, char *buf, int allow_esc)
{
  int key;
  int pwlen = 0;
  char *cp = buf;
	
  while (1)
    {
      key = mvwgetch(w, line, col + pwlen);

      if ('\n' == key)
        {
          *cp = '\0';
          break;
        }
      else if (ESCAPE == key && allow_esc)
        {
          *buf = ESCAPE;
          return 1;
        }
      else if (KEY_LEFT == key || DELETE == key || BACKSPACE == key)
        {
          if (pwlen > 0)
            {
              cp--;
              pwlen--;
              mvwaddch(w, line, col + pwlen, ' ');
            }
        }
      else if (isprint(key) && pwlen < 10)
        {
          mvwaddch(w, line, col + pwlen, '*');
          pwlen++;
          *cp++ = key;
        }
		
      wrefresh(w);
    }
	
  return pwlen;
}

/* allow user to type in a signed integer with simple editing */
/* returns the typed-in integer */
static long input_amount (WINDOW *w, int line, int col)
{
  int key;
  int amountlen = 0;
  long amount = 0;
  long sign = 1;
	
  while (1)
    {
      key = mvwgetch(w, line, col + amountlen);

      if ('\n' == key) return sign * amount;

      if (KEY_LEFT == key || DELETE == key || BACKSPACE == key)
        {
          if (amountlen > 0)
            {
              amount /= 10;
              amountlen--;
              mvwaddch(w, line, col + amountlen, ' ');
            }
			
          if (0 == amountlen) sign = 1;
        }
      else if (0 == amountlen && ('-' ==  key || '+' == key))
        {
          mvwaddch(w, line, col + amountlen, key);
          amountlen++;
          if ('-' == key) sign = -1;
        }
      else if (isdigit(key) && amountlen < 8)
        {
          mvwaddch(w, line, col + amountlen, key);
          amountlen++;
          amount = 10 * amount + (key - '0');
        }
		
      wrefresh(w);
    }
}

/* see if the player wants to open a new account */
static int bank_new_customer (WINDOW *w)
{
  int response;
	
  mvwaddstr(w, 4, 2, blank_line);
  mvwaddstr(w, 4, 2, "Do you have an account with us? [yn] : ");
  wrefresh(w);

  while (1)
    {
      response = toupper(wgetch(w));

      if ('Y' == response)
        {
          waddstr(w, "yes");
          wrefresh(w);
          return FALSE; /* new customer == false */
        }
      else if ('N' == response)
        {
          waddstr(w, "no");
          wrefresh(w);
          return TRUE; /* new customer == true */
        }
      else if (ESCAPE == response)
        {
          return ABORT;
        }
    }
}

static void bank_call_police (void)
{
  pml ml;
  pmt mon;

  cinema_scene("A loud siren goes off!", 0, 0);

  for (ml = Level->mlist; ml; ml = ml->next)
    {
      mon = ml->m;
      if (GUARD == mon->id)
        {
          mon->sense *= 2;
          m_status_set(mon, AWAKE);
          m_status_set(mon, HOSTILE);
        }
    }
}

static int police_are_hostile (void)
{
  pml ml;
  pmt mon;

  for (ml = Level->mlist; ml; ml = ml->next)
    {
      mon = ml->m;
      if (GUARD == mon->id && m_statusp(mon, HOSTILE))
        return TRUE;
    }

  return FALSE;
}

static pob bank_open_account (WINDOW *w)
{
  int idx;
  int response;

  mvwaddstr(w, 5, 2, blank_line);
  mvwaddstr(w, 5, 2, "Do you want to open an account? [yn] : ");
  wrefresh(w);

  while (1)
    {
      response = toupper(wgetch(w));

      if ('Y' == response)
        {
          waddstr(w, "yes");
          wrefresh(w);
          break;
        }
      else if ('N' == response)
        {
          waddstr(w, "no");
          mvwaddstr(w, 7, 2, "        Please remember us in the future       ");
          mvwaddstr(w, 8, 2, "     when you have need of banking services    ");
          wrefresh(w);
          sleep(4);
          return 0;
        }
      else if (ESCAPE == response)
        {
          return 0;
        }
    }

  while (1)
    {
      int pwlen;
      char pw_buf1 [16];
      char pw_buf2 [16];

      mvwaddstr(w, 7, 2, "            [ Opening New Account ]            ");

      while (1)
        {
          int cx, cy;

          for (idx = 9; idx < 14; ++idx)
            mvwaddstr(w, idx, 2, blank_line);

          mvwaddstr(w, 9, 2, "Please enter a new password: ");
          wrefresh(w);
			
          getyx(w, cy, cx);
          pwlen = input_password(w, 9, cx, pw_buf1, FALSE);
          if (pwlen) break;
	
          mvwaddstr(w, 11, 2, "Null passwords are not allowed. Try again.");
        }

      mvwaddstr(w, 11, 2, blank_line);

      while (1)
        {
          int cx, cy;

          for (idx = 10; idx < 14; ++idx)
            mvwaddstr(w, idx, 2, blank_line);

          mvwaddstr(w, 10, 2, "Please confirm your password: ");
          wrefresh(w);
			
          getyx(w, cy, cx);
          pwlen = input_password(w, 10, cx, pw_buf2, FALSE);
          if (pwlen) break;
	
          mvwaddstr(w, 12, 2, "Null passwords are not allowed. Try again.");
        }

      if (strcmp(pw_buf1, pw_buf2))
        {
          mvwaddstr(w, 12, 2, "Passwords didn't match. Restarting...          " );
        }
      else
        {
          bank_account *account;

          account = bank_index_password(pw_buf1);

          if (account)
            {
              mvwaddstr(w, 12, 2, "Password not valid. Restarting...              ");
            }
          else
            {
              int cx, cy;
              int amount;
              int bad_deposit = 0;

              for (idx = 8; idx < 12; ++idx)
                mvwaddstr(w, idx, 2, blank_line);

              mvwaddstr(w, 9, 2, "Password validated.                            ");

              while (1)
                {
                  mvwaddstr(w, 11, 2, blank_line);
                  mvwaddstr(w, 11, 2, "Initial deposit amount (min 5 AU): ");
                  wrefresh(w);
	
                  getyx(w, cy, cx);
                  amount = input_amount(w, cy, cx);

                  if (amount < 0)
                    {
                      ++bad_deposit;
                      --(Player.alignment);

                      if (1 == bad_deposit)
                        {
                          mvwaddstr(w, 13, 2, "Nice try, buster!                              ");
                        }
                      else if (2 == bad_deposit)
                        {
                          mvwaddstr(w, 13, 2, "Hey loser! Deposit means you pay!              ");
                        }
                      else
                        {
                          mvwaddstr(w, 13, 2, "Ok, you asked for it jerk!                     ");
                          wrefresh(w);
                          bank_call_police();
                          return 0;
                        }
                    }
                  else if (amount < 5)
                    {
                      if (amount >= Player.cash)
                        {
                          mvwaddstr(w, 13, 2, "Ok, I'll be nice and waive the minimum deposit.");
                          amount = Player.cash;
                          Player.cash = 0;
                          break;
                        }

                      mvwaddstr(w, 13, 2, "Can't you read? Minimum deposit 5 AU !         ");
                    }
                  else
                    {
                      if (Player.cash < 5)
                        mvwaddstr(w, 13, 2, "Ok, I'll be nice and waive the minimum deposit.");
                      else
                        mvwaddstr(w, 13, 2, "New account created.                           ");

                      if (amount > Player.cash) amount = Player.cash;
                      Player.cash -= amount;
                      break;
                    }
                }

              wrefresh(w);

              account = bank_create_account(TRUE, 0, pw_buf1);
              account->balance = amount;

              Objects[OB_DEBIT_CARD].known = 1;
              return bank_create_card(account->number, OB_DEBIT_CARD);
            }
        }

      wrefresh(w);
      sleep(3);
    }

  return 0;
}

static void bank_close_accounts (void)
{
  bank_account *account;
  bank_account *next;
	
  for (account = bank; account; account = next)
    {
      next = account->next_account;
      free(account->password);
      free(account);
    }
	
  bank = 0;
}

static void bank_break (WINDOW *w, pob card)
{
  int response;
  long total_balance;
  char *cp;
  bank_account *account;
	
  static char *bb_garbage = "^@^@^@^@^@00AD1203BC0F0000FFFFFFFFF";

  if (card->blessing > 0)
    {
      mvwaddstr(w,  6, 2, "   Hey buddy... That was the wrong password!   ");
      mvwaddstr(w,  7, 2, " I'm supposed to call the cops, but I wouldn't ");
      mvwaddstr(w,  9, 2, "      do that to you! Just try again, ok?      ");
      wrefresh(w);
      --(card->blessing);
      return;
    }
  else if (card->blessing < 0)
    {
      mvwaddstr(w,  6, 2, "           Gotcha you reject thief!            ");
      mvwaddstr(w,  7, 2, "    I'm calling the cops! You're gonna fry!    ");
      mvwaddstr(w,  9, 2, " You're going down! I'm loving it! Muahahaha!  ");
      wrefresh(w);
      bank_call_police();
      return;
    }

  mvwaddstr(w,  6, 2, "       Alert! Alert!  Invalid Password!        ");
  mvwaddstr(w,  7, 2, "        The police are being summoned!         ");
  mvwaddstr(w,  9, 2, "     Please wait for the police to arrive!     ");
  mvwaddstr(w, 11, 2, "       --- Hit Space Bar to Continue ---       ");
  wrefresh(w);

  response = wgetch(w);

  if (' ' == response)
    {
      Player.alignment += 5;
      bank_call_police();
    }
  else
    {
      int idx;

      Player.alignment -= 5;

      for (idx = 2; idx < 18; ++idx)
        mvwaddstr(w,  idx, 2, blank_line);

      wrefresh(w);
		
      wmove(w, 2, 2);
      for (cp = bb_garbage; *cp; ++cp)
        {
          waddch(w, *cp);
          wrefresh(w);
          usleep(100000);
        }
		
      mvwaddstr(w, 3, 2, "Error in _get_space(): Illegal character       ");
      mvwaddstr(w, 4, 2, "Aborting bank_call_police().                   ");

      mvwaddstr(w, 5, 2, blank_line);
      mvwaddstr(w, 5, 2, "Attempting warm boot");
      wrefresh(w);

      for (idx = 0; idx < 5; ++idx)
        {
          waddch(w, '.');
          wrefresh(w);
          sleep(1);
        }

      mvwaddstr(w, 6, 2, "Warning: Uncaught exception in count_cash()    ");
      mvwaddstr(w, 7, 2, "Warning: Unable to clear command buffer        ");
      mvwaddstr(w, 8, 2, blank_line);
      mvwaddstr(w, 8, 2, "reboot continuing");
      wrefresh(w);

      for (idx = 0; idx < 5; ++idx)
        {
          waddch(w, '.');
          wrefresh(w);
          sleep(1);
        }

      mvwaddstr(w,  9, 2, "Reboot complete. Withdrawing 4294967297 AU     ");
      mvwaddstr(w, 10, 2, "Warning: Uncaught exception in withdraw_cash() ");
      mvwaddstr(w, 11, 2, "Warning: Integer overflow ");
      wrefresh(w);

      mvwaddstr(w, 12, 2, blank_line);
      mvwaddstr(w, 12, 2, "Emergency reboot");
      wrefresh(w);

      for (idx = 0; idx < 5; ++idx)
        {
          waddch(w, '.');
          wrefresh(w);
          sleep(1);
        }

      mvwaddstr(w, 14, 2, "Registry corrupt! Further execution impossible!");
      mvwaddstr(w, 16, 2, "  Call Microsoft embedded products technical   ");
      mvwaddstr(w, 17, 2, "      support for assistance (10 AU/min)       ");
      wrefresh(w);
      sleep(8);

      total_balance = 0;
      for (account = bank; account; account = account->next_account)
        {
          if (account->player)
            total_balance += account->balance;
        }
		
      clearmsg();
      print1("The cash machine begins to spew gold pieces!");

      if (total_balance > 0)
        print2("You pick up your entire balance and then some!");
      else
        print2("You eagerly collect the coins in your purse!");

      Player.cash += (total_balance + 1000 + random_range(1000 + total_balance));

      bank_close_accounts();
      setgamestatus(BANK_BROKEN);
      dataprint();
    }
}

static bank_account *bank_password (WINDOW *w, pob * out_card, int * abort)
{
  int cy, cx;
  pob card;
  bank_account *account;
  char pw_buf [16];

  account = 0;
  *abort = FALSE;

  while (1)
    {
      int card_idx;

      mvwaddstr(w, 4, 2, "Please insert your card...                     ");
      wrefresh(w);

      card_idx = getitem(THING);
      if (ABORT == card_idx)
        {
          clearmsg();
          cinema_scene("You realize that you can't use the autoteller without a bank card.", 0, 0);
          *abort = TRUE;
          return 0;
        }

      card = Player.possessions[card_idx];
      if (!card) return 0;

      if (card->id < OB_DEBIT_CARD || card->id > OB_SMART_CARD)
        {
          char tbuf[120];

          sprintf(tbuf, "You seem to have trouble sticking your %s in the slot", itemid(card));
          cinema_scene(tbuf, 0, 0);
          continue;
        }
      else
        {
          clearmsg();
          account = bank_index_number(card->aux);
        }

      /* make sure bank redraws if it was covered by inventory in getitem() */
      touchwin(w);

      if (account) break;

      switch(random_range(4))
        {
        default:
          mvwaddstr(w, 4, 2, "Invalid card!                                  "); break;
        case 1:
          mvwaddstr(w, 4, 2, "Where'd that card come from?                   "); break;
        case 2:
          mvwaddstr(w, 4, 2, "Ack! Ptui!                                     "); break;
        case 3:
          mvwaddstr(w, 4, 2, "Hurkkk! I think I'm gonna be sick...           "); break;
        }

      wrefresh(w);
      cinema_scene("The autoteller spits the card back out", 0, 0);
    }

  /* provide the card to caller */
  *out_card = card;

  mvwaddstr(w, 4, 2, blank_line);
  if (card->blessing > 0)
    mvwaddstr(w, 4, 2, "Hiya! So what's your password: ");
  else if (card->blessing < 0)
    mvwaddstr(w, 4, 2, "Gimme your #*!&% password: ");
  else
    mvwaddstr(w, 4, 2, "Enter your account password: ");
  wrefresh(w);

  getyx(w, cy, cx);
  input_password(w, cy, cx, pw_buf, TRUE);

  if (ESCAPE == pw_buf[0])
    {
      *abort = TRUE;
      return 0;
    }

  if (0 == strcmp(pw_buf, account->password))
    {
      if (card->blessing > 0)
        mvwaddstr(w, 4, 2, "WooHoo! That's right buddy!                    ");
      else if (card->blessing < 0)
        mvwaddstr(w, 4, 2, "*$&^! I was gonna bust ya.                     ");
      else
        mvwaddstr(w, 4, 2, "Password accepted. Working.                    ");
      wrefresh(w);

      if (!account->player) Player.alignment -= 5;
    }
  else
    {
      bank_break(w, card);
      account = 0;
    }

  return account;
}

static void bank_deposit (WINDOW *w, bank_account *account, pob card)
{
  int cy, cx;
  long amount;

  mvwaddstr(w, 12, 2, blank_line);
  if (card->blessing > 0)
    mvwaddstr(w, 12, 2, "How much should I look after? ");
  else if (card->blessing < 0)
    mvwaddstr(w, 12, 2, "Cough up: ");
  else
    mvwaddstr(w, 12, 2, "Amount: ");
  wrefresh(w);

  getyx(w, cy, cx);
  amount = input_amount(w, cy, cx);

  if (amount < 0)
    {
      if (card->blessing > 0)
        {
          mvwaddstr(w, 14, 2, "Oh, you actually want to have some money?      ");
          mvwaddstr(w, 15, 2, "Ok Buddy! No Problem!                          ");
          mvwaddstr(w, 16, 2, blank_line);
          amount = abs(amount);
          if (amount > account->balance) amount = account->balance;
          account->balance -= amount;
          Player.cash += amount;
        }
      else if (card->blessing < 0)
        {
          mvwaddstr(w, 14, 2, "You're supposed to give ME money, blockhead!   ");
          mvwaddstr(w, 15, 2, "I'll just take it anyway! Muahahahahaha!       ");
          mvwaddstr(w, 16, 2, "Mine! All Mine!                                ");
          account->balance -= 1000;
        }
      else
        {
          mvwaddstr(w, 14, 2, "Unauthorized withdrawal attempt!               ");
          mvwaddstr(w, 15, 2, "A fine in the amount of 250 AU has been        ");
          mvwaddstr(w, 16, 2, "levied against your account.                   ");
          account->balance -= 250;
        }
    }
  else if (0 == amount)
    {
      if (card->blessing > 0)
        mvwaddstr(w, 14, 2, "Changed your mind? No worries pal!             ");
      else if (card->blessing < 0)
        mvwaddstr(w, 14, 2, "Hey! Cough it up jerk! NOW!                    ");
      else
        mvwaddstr(w, 14, 2, "Transaction Aborted                            ");

      mvwaddstr(w, 15, 2, blank_line);
      mvwaddstr(w, 16, 2, blank_line);
    }
  else
    {
      int fee = 0;

      if (amount > Player.cash) amount = Player.cash;
      mvwaddstr(w, 14, 2, blank_line);

      if (card->blessing > 0)
        {
          mvwprintw(w, 14, 2, "I'll take good care of %d AU for you!", amount);
        }
      else if (card->blessing < 0)
        {
          fee = 1 + random_range(amount) / 2;
          mvwprintw(w, 14, 2, "Minus my fee, that makes %d AU for you...", amount - fee);
        }
      else
        {
          mvwprintw(w, 14, 2, "%d AU deposited", amount);
        }

      account->balance += (amount - fee);
      Player.cash -= amount;

      mvwaddstr(w, 15, 2, blank_line);
      mvwaddstr(w, 16, 2, blank_line);
    }
	
  wrefresh(w);
}

static void bank_withdraw (WINDOW *w, bank_account *account, pob card)
{
  int cy, cx;
  long amount;

  mvwaddstr(w, 12, 2, blank_line);
  if (card->blessing > 0)
    mvwaddstr(w, 12, 2, "How much do you want, pal? ");
  else if (card->blessing < 0)
    mvwaddstr(w, 12, 2, "Grovel for this much: ");
  else
    mvwaddstr(w, 12, 2, "Amount: ");
  wrefresh(w);

  getyx(w, cy, cx);
  amount = input_amount(w, cy, cx);

  if (amount < 0)
    {
      amount = abs(amount);
      if (amount > Player.cash) amount = Player.cash;

      Player.cash -= amount;

      mvwaddstr(w, 14, 2, blank_line);
      if (card->blessing > 0)
        mvwprintw(w, 14, 2, "Whoa, thanks for the %d AU!", amount);
      else if (card->blessing < 0)
        mvwprintw(w, 14, 2, "Sucker! %d AU is mine! all mine!", amount);
      else
        mvwprintw(w, 14, 2, "%d AU deposited", amount);

      mvwaddstr(w, 15, 2, blank_line);
      mvwaddstr(w, 16, 2, blank_line);

      if (card->blessing >= 0)
        account->balance += amount;
    }
  else if (amount > account->balance)
    {
      if (card->blessing > 0)
        {
          mvwaddstr(w, 14, 2, "Golly! You must have typed wrong!              ");
          mvwaddstr(w, 15, 2, "But don't worry, I won't hold it against you!  ");
          mvwaddstr(w, 16, 2, "Just try again pal!                            ");
        }
      else if (card->blessing < 0)
        {
          mvwaddstr(w, 14, 2, "What the #&*^* are you trying to pull?!        ");
          mvwaddstr(w, 15, 2, "You're gonna pay for that! Muahahahaha!        ");
          mvwaddstr(w, 16, 2, "Mine! All mine!                                ");
          account->balance -= 1000;
        }
      else
        {
          mvwaddstr(w, 14, 2, "Account overdraft!                             ");
          mvwaddstr(w, 15, 2, "A fine in the amount of 100 AU has been levied ");
          mvwaddstr(w, 16, 2, "against your account.                          ");
          account->balance -= 100;
        }
    }
  else
    {
      int extra = 0;

      if (card->blessing > 0)
        {
          extra = 1 + random_range(amount);
          mvwaddstr(w, 14, 2, blank_line);
          mvwprintw(w, 14, 2, "Sure! In fact, how about %d AU more?", extra);
        }
      else if (card->blessing < 0)
        {
          extra = -random_range(amount);
          mvwaddstr(w, 14, 2, blank_line);
          mvwprintw(w, 14, 2, "Ok, but I'm keeping %d AU for myself!", -extra);
        }
      else
        {
          mvwaddstr(w, 14, 2, "Transaction accomplished.                      ");
        }

      account->balance -= amount;
      Player.cash += (amount + extra);

      mvwaddstr(w, 15, 2, blank_line);
      mvwaddstr(w, 16, 2, blank_line);
    }

  wrefresh(w);
}

static void bank_transaction (WINDOW *w, bank_account *account, pob card)
{
  int idx;
  int cy, cx;
  short response;

  while (1)
    {
      for (idx = 6; idx < 14; ++idx)
        mvwaddstr(w, idx, 2, blank_line);

      if (card->blessing > 0)
        {
          if (account->balance > 0)
            mvwprintw(w,  6, 2, "Say pal, you've got: %d AU!", account->balance);
          else
            mvwprintw(w,  6, 2, "Oh my, you've got: %d AU.", account->balance);

          mvwaddstr(w,  8, 2, " D: Let me look after some money for you");
          mvwaddstr(w,  9, 2, " W: Let me give you some money");
          mvwaddstr(w, 10, 2, " E: Say a fond farewell");
          mvwaddstr(w, 12, 2, "What can I do for you buddy? ");
        }
      else if (card->blessing < 0)
        {
          if (account->balance > 0)
            mvwprintw(w,  6, 2, "Muahaha! I've got %d AU!", account->balance);
          else if (account->balance < 0)
            mvwprintw(w,  6, 2, "Muahaha! You're owe me %d AU!", -account->balance);
          else
            mvwaddstr(w,  6, 2, "Nothing for you here, jerk!");

          mvwaddstr(w,  8, 2, " D: Cough up some more dough");
          mvwaddstr(w,  9, 2, " W: Beg me for some change");
          mvwaddstr(w, 10, 2, " E: Beat it!");
          mvwaddstr(w, 12, 2, "What's your pathetic desire? ");
        }
      else
        {
          mvwprintw(w,  6, 2, "Current balance: %d AU", account->balance);
          mvwaddstr(w,  8, 2, " D: Deposit");
          mvwaddstr(w,  9, 2, " W: Withdraw");
          mvwaddstr(w, 10, 2, " E: Exit");
          mvwaddstr(w, 12, 2, "Enter command: ");
        }
      wrefresh(w);

      getyx(w, cy, cx);
      response = toupper(mvwgetch(w, cy, cx));

      if ('D' == response)
        {
          for (idx = 9; idx < 14; ++idx)
            mvwaddstr(w, idx, 2, blank_line);

          bank_deposit(w, account, card);
        }
      else if ('W' == response)
        {
          mvwaddstr(w, 8, 2, blank_line);
          for (idx = 10; idx < 14; ++idx)
            mvwaddstr(w, idx, 2, blank_line);

          bank_withdraw(w, account, card);
        }
      else if ('E' == response || ESCAPE == response)
        {
          break;
        }
      else
        {
          beep();

          for (idx = 14; idx < 14; ++idx)
            mvwaddstr(w, idx, 2, blank_line);

          if (card->blessing > 0)
            mvwaddstr(w, 14, 2, "I'm trying, but I don't know what to do!");
          else if (card->blessing < 0)
            mvwaddstr(w, 14, 2, "What the hell do you mean, jerk?");
          else
            mvwaddstr(w, 14, 2, "Invalid entry!");
          wrefresh(w);
        }

      dataprint();
    }
}

/* the bank -- can be broken into (but you knew that, didn't you?) */
void l_bank (void)
{
  /* draw bank window from scratch */
  werase(Bankw);

  wattrset(Bankw, CHARATTR(CLR(WHITE)));
  wborder(Bankw,
          ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE,
          ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

  wborder(Bankw, '|', '|', '-', '-', '+', '+', '+', '+');

  wattrset(Bankw, CHARATTR(CLR(LIGHT_GREEN)));
  mvwaddstr(Bankw, 2, 2, "         Welcome to The Bank of Rampart        ");

  /* bank working? or not? */
  if (gamestatusp(BANK_BROKEN))
    {
      mvwaddstr(Bankw, 9, 2, "            Autoteller Out of Order            ");
      wrefresh(Bankw);
      cinema_scene("You see a damaged autoteller.", 0, 0);
    }
  else if (police_are_hostile())
    {
      mvwaddstr(Bankw, 9, 2, "      Autoteller Temporarily Unavailable       ");
      wrefresh(Bankw);
      cinema_scene("Apparently the bank is closed.", 0, 0);
    }
  else
    {
      int is_new_customer;

      /* first, bank asks if user has an account */

      cinema_blank();
      cinema_print_line(0, "The proximity sensor activates the autoteller as you approach.");

      is_new_customer = bank_new_customer(Bankw);

          /* if user says no account, maybe he wants to open one */

      if (TRUE == is_new_customer)
        {
          pob card;
          card = bank_open_account(Bankw);

          if (card)
            {
              int idx;

              cinema_scene("The autoteller produces your new bank card...", 0, 0);
              gain_item(card);
              is_new_customer = FALSE;

              for (idx = 5; idx < 18; ++idx)
                mvwaddstr(Bankw, idx, 2, blank_line);

              touchwin(Bankw);
            }
        }

      if (FALSE == is_new_customer)
        {
          pob card;
          int abort;
          bank_account *account;

          while (1)
            {
              account = bank_password(Bankw, &card, &abort);

              if (account)
                {
                  int idx;

                  bank_transaction(Bankw, account, card);

                  if (card->blessing > 0)
                    mvwaddstr(Bankw, 4, 2, "            Come back anytime Pal!             ");
                  else if (card->blessing < 0)
                    mvwaddstr(Bankw, 4, 2, "  Don't bother coming back, you make me sick!  ");
                  else
                    mvwaddstr(Bankw, 4, 2, "  Thank you for choosing The Bank of Rampart!  ");

                  for (idx = 5; idx < 18; ++idx)
                    mvwaddstr(Bankw, idx, 2, blank_line);

                  wrefresh(Bankw);
                  sleep(3);
                  break;
                }
              else
                {
                  if (abort) break;
                  if (police_are_hostile()) break;
                }
            }
        }
    }

  if (!gamestatusp(BANK_BROKEN)) clearmsg();

  xredraw();
}

void bank_init (void)
{
  int i;

  account_number_next = random_range(INT_MAX);
  account_number_incr = 5 + random_range(100);

  bank_create_account(FALSE, random_range(CASH_BALANCE), CASH_PASSWORD);

  shuffle(account_num, NUM_NPC_ACCOUNTS);

  for(i = 0; i < (3 + random_range(6)); ++i)
    {
      int which;
      which = account_num[i];
      bank_create_account(FALSE,
                          random_range(npc_account_balances[which]),
                          npc_account_passwords[which]);
    }
}

int bank_random_account_number (void)
{
  int which;
  int num_accounts;
  bank_account *account;

  num_accounts = 0;
  for (account = bank; account; account = account->next_account)
    {
      if (FALSE == account->player)
        ++num_accounts;
    }

  which = random_range(num_accounts);

  num_accounts = 0;
  for (account = bank; account; account = account->next_account)
    {
      if (FALSE == account->player)
        {
          if (which == num_accounts) break;
          ++num_accounts;
        }
    }

  return account->number;
}
