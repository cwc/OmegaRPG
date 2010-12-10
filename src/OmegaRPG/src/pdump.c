/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* pdump.c */

#include <ctype.h>
#include "glob.h"

char *monk_rank_string(int rank);
char *legion_rank_string( int rank );
char *arena_rank_string( int rank );
char *college_rank_string( int rank );
char *nobility_rank_string( int rank );
char *circle_rank_string( int rank );
char *order_rank_string( int rank );
char *thieves_rank_string( int rank );
char *priesthood_rank_string( int rank, int patron );
char *elapsed_time_string( long minutes );
char *alignment_string( int alignment );
int alignment_level_index( int alignment );
short build_check( long *check, char *s );
short dump( FILE *dumpfile, char *s, short do_check );
short dump_basic( FILE *dumpfile );
short dump_options( FILE *dumpfile );
short dump_stati( FILE *dumpfile );
short dump_immunities( FILE *dumpfile );
short dump_in_columns( FILE *dumpfile, int col_width, char *s );
short end_dump_in_columns( FILE *dumpfile );
short dump_ranks( FILE *dumpfile );
char *fill_rank_field( char *buf );
void strcat_carry_item( char *buf, int slotnum );
short dump_carried( FILE *dumpfile );
short dump_pack( FILE *dumpfile );
short dump_condo( FILE *dumpfile );
short dump_possessions( FILE *dumpfile );
void player_dump( void );

long dumpcheck;
char *why;
char dump_buf[ 100 ];
char spaces[] = "                                             "
"                                             ";

short do_dump_stat_imm = TRUE;

char *legion_ranks[ 6 ] =
{
  "",
  "Legionaire",
  "Centurion of the Legion",
  "Force Leader of the Legion",
  "Colonel of the Legion",
  "Commandant of the Legion"
};

char *arena_ranks[ 6 ] =
{
  "Ex-gladiator",
  "Gladiator Trainee of the Arena",
  "Bestiarius of the Arena",
  "Retiarius of the Arena",
  "Gladiator of the Arena",
  "Gladiator Champion of the Arena"
};

char *college_ranks[ 6 ] =
{
  "",
  "Collegium Magii: Novice",
  "Collegium Magii: Student",
  "Collegium Magii: Preceptor",
  "Collegium Magii: Mage",
  "Archmage of the Collegium Magii"
};

char *nobility_ranks[ 6 ] =
{
  "Lowly Commoner",
  "Commoner",
  "Squire of Rampart",
  "Order of the Knights of Rampart",
  "Peer of the Realm",
  "Duke of Rampart"
};

char *circle_ranks[ 6 ] =
{
  "Former member of the Circle",
  "Member of the Circle of Initiates",
  "Member of the Circle of Enchanters",
  "Member of the Circle of Sorcerors",
  "High Sorceror of the Inner Circle",
  "Prime Sorceror of the Inner Circle"
};

char *order_ranks[ 6 ] =
{
  "Washout from the Order of Paladins",
  "Gallant of the Order",
  "Guardian of the Order",
  "Chevalier of the Order",
  "Paladin of the Order",
  "Justiciar of the Order of Paladins"
};

char *thieves_ranks[ 6 ] =
{
  "",
  "Guild of Thieves: Candidate Member",
  "Guild of Thieves: Apprentice Thief",
  "Guild of Thieves: Thief",
  "Guild of Thieves: Master Thief",
  "Guild of Thieves: Shadowlord"
};

char *priesthood_ranks[ 6 ] =
{
  "",
  "A lay devotee of ",
  "An Acolyte of ",
  "A Priest of ",
  "A Senior Priest of ",
  "The High Priest of "
};

char *patron_names[ 7 ] =
{
  "",
  "Odin",
  "Set",
  "Athena",
  "Hecate",
  "Druidism",
  "the Lords of Destiny"
};

char *monk_ranks[8] =
{
  "",
  "Monk Trainee",
  "Monk",
  "Monk Master",
  "Monk Master of Sighs",
  "Monk Master of Pain",
  "Monk Master of Tears",
  "Monk Grandmaster"
};

void player_dump( void )
{
  FILE *dumpfile;
  char dump_name[ 32 ];
	
  /* build player dump file name as "charactername.txt" */
  strncpy( dump_name, Player.name, 27 );
  strcat( dump_name, ".txt" );
	
  /* try to open dump file for writing, not using checkfopen() */
  /* (file.c) because there is no need to quit the program if */
  /* fopen fails. */
  dumpfile = fopen( dump_name, "w" );
  if ( !dumpfile )
    {
      why = "couldn't open file";
      goto dump_failed;
    }
		
  /* dump name, stats, etc. */
  if ( !dump_basic( dumpfile ) )
    goto dump_failed;
		
  /* dump options */
  if ( !dump_options( dumpfile ) )
    goto dump_failed;
		
  if ( do_dump_stat_imm )
    {
      /* dump stati */
      if ( !dump_stati( dumpfile ) )
        goto dump_failed;
			
      /* dump immunities */
      if ( !dump_immunities( dumpfile ) )
        goto dump_failed;
    }
	
  /* dump ranks */
  if ( !dump_ranks( dumpfile ) )
    goto dump_failed;
	
  /* dump possessions */
  if ( !dump_possessions( dumpfile ) )
    goto dump_failed;
		
  fclose( dumpfile );
  /*_ftype = 'odat';*/
	
  clearmsg();
  print1( "Character dump successful" );
  morewait();
  return;
	
 dump_failed:
  clearmsg();
  sprintf( Str3, "Character dump unsuccessful (%s)", why );
  print1( Str3 );
  morewait();
  return;
}

void strcat_carry_item( char *buf, int slot )
{
  if ( Player.possessions[ slot ] == NULL )
    strcat( buf, "(nothing)\n" );
  else
    {
      strcat( buf, itemid( Player.possessions[ slot ] ));
      strcat( buf, "\n" );
    }
}

short dump_carried( FILE *dumpfile )
{
  /* reset "checksum" */
  dumpcheck = 0;

  if ( !dump( dumpfile, "-- Possessions (Carried) --\n\n", FALSE ) )
    return FALSE;
		
  strcpy( dump_buf, "a) Ready hand     : " );
  strcat_carry_item( dump_buf, O_READY_HAND );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  strcpy( dump_buf, "b) Weapon hand    : " );
  strcat_carry_item( dump_buf, O_WEAPON_HAND );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "c) Left shoulder  : " );
  strcat_carry_item( dump_buf, O_LEFT_SHOULDER );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "d) Right shoulder : " );
  strcat_carry_item( dump_buf, O_RIGHT_SHOULDER );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "e) Belt           : " );
  strcat_carry_item( dump_buf, O_BELT1 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "f) Belt           : " );
  strcat_carry_item( dump_buf, O_BELT2 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "g) Belt           : " );
  strcat_carry_item( dump_buf, O_BELT3 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "h) Shield         : " );
  strcat_carry_item( dump_buf, O_SHIELD );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "i) Armor          : " );
  strcat_carry_item( dump_buf, O_ARMOR );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "j) Boots          : " );
  strcat_carry_item( dump_buf, O_BOOTS );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "k) Cloak          : " );
  strcat_carry_item( dump_buf, O_CLOAK );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "l) Finger         : " );
  strcat_carry_item( dump_buf, O_RING1 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "m) Finger         : " );
  strcat_carry_item( dump_buf, O_RING2 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "n) Finger         : " );
  strcat_carry_item( dump_buf, O_RING3 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  strcpy( dump_buf, "o) Finger         : " );
  strcat_carry_item( dump_buf, O_RING4 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  sprintf( dump_buf, "\n[Verification: %8.8lx]\n\n", dumpcheck );
  if ( !dump( dumpfile, dump_buf, FALSE ) )
    return FALSE;
			
  return TRUE;
}

short dump_pack( FILE *dumpfile )
{
  int i;
	
  if ( Player.packptr > 0 )
    {
      /* reset "checksum" */
      dumpcheck = 0;
	
      if ( !dump( dumpfile, "-- Possessions (In Pack) --\n\n", FALSE ) )
        return FALSE;
		
      for( i = 0; i < Player.packptr; i++ )
        {
          sprintf( dump_buf, "%c) %s\n", i + 'A', itemid( Player.pack[ i ] ) );
          if ( !dump( dumpfile, dump_buf, TRUE ) )
            return FALSE;
        }
			
      sprintf( dump_buf, "\n[Verification: %8.8lx]\n\n", dumpcheck );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
			
  return TRUE;
}

short dump_condo( FILE *dumpfile )
{
  pol ol;
	
  if ( Condoitems )
    {
      /* reset "checksum" */
      dumpcheck = 0;
	
      if ( !dump( dumpfile, "-- Possessions (In Condo) --\n\n", FALSE ) )
        return FALSE;
			
      for( ol = Condoitems; ol; ol = ol->next )
        {
          sprintf( dump_buf, "%s\n", itemid( ol->thing ) );
          if ( !dump( dumpfile, dump_buf, TRUE ) )
            return FALSE;
        }
			
      sprintf( dump_buf, "\n[Verification: %8.8lx]\n\n", dumpcheck );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
			
  return TRUE;
}

short dump_possessions( FILE *dumpfile )
{
  if ( !dump_carried( dumpfile ) )
    return FALSE;
		
  if ( !dump_pack( dumpfile ) )
    return FALSE;
	
  if ( gamestatusp( SOLD_CONDO ) )
    {
      if ( !dump_condo( dumpfile ) )
        return FALSE;
    }
		
  return TRUE;
}

#define RANK_FIELD_WIDTH 41
char *fill_rank_field( char *buf )
{
  int slen;
  char *cp;
	
  slen = strlen( buf );
  cp = buf + slen;
  while( slen < RANK_FIELD_WIDTH )
    {
      *cp++ = ' ';
      slen++;
    }
	
  return cp;
}

short dump_ranks( FILE *dumpfile )
{
  char *cp;
	
  /* reset "checksum" */
  dumpcheck = 0;

  if ( !dump( dumpfile, "-- Current Ranks --\n\n", FALSE ) )
    return FALSE;
	
  if ( Player.rank[ LEGION ] > 0 )
    {
      strcpy( dump_buf, legion_rank_string( Player.rank[ LEGION ] ) );
      cp = fill_rank_field( dump_buf );
      sprintf( cp, "Exp: %ld\n", Player.guildxp[ LEGION ] );
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  if ( Player.rank[ ARENA ] != 0 )
    {
      strcpy( dump_buf, arena_rank_string( Player.rank[ ARENA ] ) );
      if ( Player.rank[ ARENA ] > 0 )
        {
          cp = fill_rank_field( dump_buf );
          sprintf( cp, "%d opponents defeated\n", Arena_Opponent - 3 );
        }
      else
        {
          strcat( dump_buf, "\n" );
        }
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  if ( Player.rank[ COLLEGE ] > 0 )
    {
      strcpy( dump_buf, college_rank_string( Player.rank[ COLLEGE ] ) );
      cp = fill_rank_field( dump_buf );
      sprintf( cp, "Exp: %ld\n", Player.guildxp[ COLLEGE ] );
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  strcpy( dump_buf, nobility_rank_string( Player.rank[ NOBILITY ] ) );
  cp = fill_rank_field( dump_buf );
  if ( Player.rank[ NOBILITY ] == 0 )
    {
      *cp++ = '\n';
      *cp = '\0';
    }
  else if ( Player.rank[ NOBILITY ] == 1 )
    {
      sprintf( cp, "1st quest undertaken\n" );
    }
  else if ( Player.rank[ NOBILITY ] > 1 )
    {
      sprintf( cp, "%d%s quest completed\n", Player.rank[ NOBILITY ] - 1,
               ordinal( Player.rank[ NOBILITY ] - 1 ) );
    }
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  if ( Player.rank[ CIRCLE ] != 0 )
    {
      strcpy( dump_buf, circle_rank_string( Player.rank[ CIRCLE ] ) );
      if ( Player.rank[ CIRCLE ] > 0 )
        {
          cp = fill_rank_field( dump_buf );
          sprintf( cp, "Exp: %ld\n", Player.guildxp[ CIRCLE ] );
        }
      else
        {
          strcat( dump_buf, "\n" );
        }
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  if ( Player.rank[ ORDER ] != 0 )
    {
      strcpy( dump_buf, order_rank_string( Player.rank[ ORDER ] ) );
      if ( Player.rank[ ORDER ] > 0 )
        {
          cp = fill_rank_field( dump_buf );
          sprintf( cp, "Exp: %ld\n", Player.guildxp[ ORDER ] );
        }
      else
        {
          strcat( dump_buf, "\n" );
        }
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  if ( Player.rank[ THIEVES ] > 0 )
    {
      strcpy( dump_buf, thieves_rank_string( Player.rank[ THIEVES ] ) );
      cp = fill_rank_field( dump_buf );
      sprintf( cp, "Exp: %ld\n", Player.guildxp[ THIEVES ] );
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  if ( Player.rank[ PRIESTHOOD ] > 0 )
    {
      strcpy( dump_buf, priesthood_rank_string( Player.rank[ PRIESTHOOD ], Player.patron ) );
      cp = fill_rank_field( dump_buf );
      sprintf( cp, "Exp: %ld\n", Player.guildxp[ PRIESTHOOD ] );
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
		
  if ( Player.rank[ MONKS ] > 0 )
    {
      strcpy(dump_buf, monk_rank_string(Player.rank[MONKS]));
      cp = fill_rank_field(dump_buf);
      sprintf(cp, "Exp: %ld\n", Player.guildxp[MONKS]);
      if (!dump(dumpfile, dump_buf, TRUE)) return FALSE;
    }
		
  if ( Player.rank[ ADEPT ] > 0 ) 
    {
      if ( !dump( dumpfile, "********** An Adept of Omega **********\n", TRUE ) )
        return FALSE;
    }
		
  sprintf( dump_buf, "\n[Verification: %8.8lx]\n\n", dumpcheck );
  if ( !dump( dumpfile, dump_buf, FALSE ) )
    return FALSE;
			
  return TRUE;
}

int dump_col;

short dump_in_columns( FILE *dumpfile, int col_width, char *s )
{
  int i, slen;
	
  slen = strlen( s );
  if (( dump_col + slen ) > ( 80 - col_width ))
    {
      strcat( dump_buf, s );
      strcat( dump_buf, "\n" );
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
			
      dump_col = 0;
    }
  else if ( dump_col == 0 )
    {
      strcpy( dump_buf, s );
      for( dump_col = slen; dump_col < col_width; dump_col++ )
        dump_buf[ dump_col ] = ' ';
      dump_buf[ dump_col ] = '\0';
    }
  else
    {
      strcat( dump_buf, s );
      for( i = dump_col + slen; i < dump_col + col_width; i++ )
        dump_buf[ i ] = ' ';
      dump_col = i;
      dump_buf[ dump_col ] = '\0';
    }
	
  return TRUE;
}

short end_dump_in_columns( FILE *dumpfile )
{
  if ( dump_col > 0 )
    {
      strcat( dump_buf, "\n" );
      if ( !dump( dumpfile, dump_buf, TRUE ) )
        return FALSE;
    }
	
  if ( !dump( dumpfile, "\n", FALSE ) )
    return FALSE;
		
  return TRUE;
}

short dump_stati( FILE *dumpfile )
{
  int numstati = 0;
	
  /* reset "checksum" */
  dumpcheck = 0;
  dump_col = 0;

  if ( !dump( dumpfile, "-- Current Stati --\n\n", FALSE ) )
    return FALSE;
	
  if ( Player.status[ BLINDED ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Blinded" ) )
        return FALSE;
    }
		
  if ( Player.status[ SLOWED ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Slowed" ) )
        return FALSE;
    }
		
  if ( Player.status[ HASTED ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Hasted" ) )
        return FALSE;
    }
		
  if ( Player.status[ DISPLACED ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Displaced" ) )
        return FALSE;
    }
		
  if ( Player.status[ SLEPT ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Slept" ) )
        return FALSE;
    }
		
  if ( Player.status[ DISEASED ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Diseased" ) )
        return FALSE;
    }
		
  if ( Player.status[ POISONED ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Poisoned" ) )
        return FALSE;
    }
		
  if ( Player.status[ BREATHING ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Breathing" ) )
        return FALSE;
    }
		
  if ( Player.status[ INVISIBLE ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Invisible" ) )
        return FALSE;
    }
		
  if ( Player.status[ REGENERATING ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Regenerating" ) )
        return FALSE;
    }
		
  if ( Player.status[ VULNERABLE ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Vulnerable" ) )
        return FALSE;
    }
		
  if ( Player.status[ BERSERK ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Berserk" ) )
        return FALSE;
    }
		
  if ( Player.status[ IMMOBILE ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Immobile" ) )
        return FALSE;
    }
		
  if ( Player.status[ ALERT ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Alert" ) )
        return FALSE;
    }
		
  if ( Player.status[ AFRAID ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Afraid" ) )
        return FALSE;
    }
		
  if ( Player.status[ ACCURATE ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Accurate" ) )
        return FALSE;
    }
		
  if ( Player.status[ HERO ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Heroic" ) )
        return FALSE;
    }
		
  if ( Player.status[ LEVITATING ] )
    {
      numstati++;
      if ( !dump_in_columns( dumpfile, 20, "Levitating" ) )
        return FALSE;
    }
	
  if ( numstati )
    {
      /* dump last row of stati */
      if ( !end_dump_in_columns( dumpfile ) )
        return FALSE;
			
      sprintf( dump_buf, "[Verification: %8.8lx]\n\n", dumpcheck );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
  else
    {
      sprintf( dump_buf, "(None)\n\n" );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
		
  return TRUE;
}

short dump_immunities( FILE *dumpfile )
{
  int numimmunities = 0;
	
  /* reset "checksum" */
  dumpcheck = 0;
  dump_col = 0;

  if ( !dump( dumpfile, "-- Current Immunities --\n\n", FALSE ) )
    return FALSE;
		
  if ( p_immune( NORMAL_DAMAGE ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Normal Damage" ) )
        return FALSE;
    }
		
  if ( p_immune( FLAME ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Flame" ) )
        return FALSE;
    }
		
  if ( p_immune( ELECTRICITY ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Electricity" ) )
        return FALSE;
    }
		
  if ( p_immune( COLD ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Cold" ) )
        return FALSE;
    }
		
  if ( p_immune( POISON ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Poison" ) )
        return FALSE;
    }
		
  if ( p_immune( ACID ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Acid" ) )
        return FALSE;
    }
		
  if ( p_immune( FEAR ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Fear" ) )
        return FALSE;
    }
		
  if ( p_immune( SLEEP ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Sleep" ) )
        return FALSE;
    }
		
  if ( p_immune( NEGENERGY ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Negative Energies" ) )
        return FALSE;
    }
		
  if ( p_immune( THEFT ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Theft" ) )
        return FALSE;
    }
		
  if ( p_immune( GAZE ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Gaze" ) )
        return FALSE;
    }
		
  if ( p_immune( INFECTION ) )
    {
      numimmunities++;
      if ( !dump_in_columns( dumpfile, 26, "Infection" ) )
        return FALSE;
    }
	
  if ( numimmunities )
    {
      /* dump last row of immunities */
      if ( !end_dump_in_columns( dumpfile ) )
        return FALSE;
			
      sprintf( dump_buf, "[Verification: %8.8lx]\n\n", dumpcheck );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
  else
    {
      sprintf( dump_buf, "(None)\n\n" );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
		
  return TRUE;
}

short cheated_death;
short cheated_savegame;

short dump_options( FILE *dumpfile )
{
  if ( cheated_death || cheated_savegame )
    {
      /* reset "checksum" */
      dumpcheck = 0;
	
      if ( !dump( dumpfile, "\n-- Cheats --\n\n", FALSE ) )
        return FALSE;
			
      if ( gamestatusp( CHEATED ) )
        {
          if ( !dump( dumpfile, "Entered Wizard Mode\n", TRUE ) )
            return FALSE;
        }
				
      if ( cheated_death )
        {
          if ( !dump( dumpfile, "Refused to die\n", TRUE ) )
            return FALSE;
        }
				
      if ( cheated_savegame )
        {
          if ( !dump( dumpfile, "Restored from save file after death\n", TRUE ) )
            return FALSE;
        }
			
      sprintf( dump_buf, "[Verification: %8.8lx]\n\n", dumpcheck );
      if ( !dump( dumpfile, dump_buf, FALSE ) )
        return FALSE;
    }
			
  return TRUE;
}

short dump_basic( FILE *dumpfile )
{
  int curcol;
  long total_balance;
  char *cp;
  bank_account *account;
	
  /* reset "checksum" */
  dumpcheck = 0;

  strcpy( dump_buf, "[*] " );
  strcat( dump_buf, VERSIONSTRING );
  strcat( dump_buf, " character dump [*]\n\n" );
  if ( !dump( dumpfile, dump_buf, FALSE ) )
    return FALSE;
		
  curcol = sprintf( dump_buf, "Name      : %s\n", Player.name );
  if ( gamestatusp( CHEATED ) )
    {
      if ( 72 > strlen( dump_buf ) )
        cp = strrchr( dump_buf, '\n' );
      else
        cp = &(dump_buf[ 70 ]);
      strcpy( cp, " (WIZARD)\n" );
    }
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  sprintf( dump_buf, "Level     : %s [%d]\n", levelname( Player.level ), Player.level );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  sprintf( dump_buf, "Alignment : %s\n\n", alignment_string( Player.alignment ) );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
	
  /*
    sprintf( dump_buf, "Str : %5d [%d]%n%*sHP       : %5d [%d]%n%*sHit     : %d\n",
    Player.str, Player.maxstr, &curcol, 26 - curcol, spaces,
    Player.hp,  Player.maxhp,  &curcol, 57 - curcol, spaces,
    Player.hit );
    if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
  */
		
  curcol = sprintf( dump_buf, "Str : %5d [%d]", Player.str, Player.maxstr );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 26; curcol++ )
    *cp++ = ' ';
  curcol += sprintf( cp, "HP       : %5d [%d]", Player.hp,  Player.maxhp );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 57; curcol++ )
    *cp++ = ' ';
  sprintf( cp, "Hit     : %d\n", Player.hit );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  curcol = sprintf( dump_buf, "Con : %5d [%d]", Player.con, Player.maxcon );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 26; curcol++ )
    *cp++ = ' ';
  curcol += sprintf( cp, "Mana     : %5ld [%ld]", Player.mana,  Player.maxmana );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 57; curcol++ )
    *cp++ = ' ';
  sprintf( cp, "Damage  : %d\n", Player.dmg );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  curcol = sprintf( dump_buf, "Dex : %5d [%d]", Player.dex, Player.maxdex );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 57; curcol++ )
    *cp++ = ' ';
  sprintf( cp, "Defense : %d\n", Player.defense );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  curcol = sprintf( dump_buf, "Agi : %5d [%d]", Player.agi, Player.maxagi );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 26; curcol++ )
    *cp++ = ' ';
  sprintf( cp, "Exp      : %-10ld          Armor   : %d\n", Player.xp,  Player.absorption );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  curcol = sprintf( dump_buf, "Int : %5d [%d]", Player.iq, Player.maxiq );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 26; curcol++ )
    *cp++ = ' ';
  sprintf( cp, "Carry    : %-10d          Speed   : %d.%d\n",
           Player.itemweight, 5 / Player.speed, 500 / Player.speed % 100 );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  curcol = sprintf( dump_buf, "Pow : %5d [%d]", Player.pow, Player.maxpow );
  cp = strrchr( dump_buf, '\0' );
  for ( ; curcol < 26; curcol++ )
    *cp++ = ' ';
  sprintf( cp, "Capacity : %d\n\n", Player.maxweight );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  total_balance = 0;
  for( account = bank; account; account = account->next_account )
    {
      if ( account->player )
        total_balance += account->balance;
    }
		
  sprintf( dump_buf, "Cash (carried/bank) : %ld / %ld\n", Player.cash, total_balance );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  sprintf( dump_buf, "Current Point Total : %ld\n", calc_points() );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  sprintf( dump_buf, "Elapsed Game Time   : %s\n\n", elapsed_time_string( Time ) );
  if ( !dump( dumpfile, dump_buf, TRUE ) )
    return FALSE;
		
  sprintf( dump_buf, "[Verification: %8.8lx]\n\n", dumpcheck );
  if ( !dump( dumpfile, dump_buf, FALSE ) )
    return FALSE;
		
  return TRUE;
}

short dump( FILE *dumpfile, char *s, short do_check )
{
  if ( do_check )
    {
      if( !build_check( &dumpcheck, s ) )
        return FALSE;
    }
		
  if ( EOF == fputs( s, dumpfile ) )
    {
      fclose( dumpfile );
      why = "failed write";
      return FALSE;
    }
	
  return TRUE;
}

short build_check( long *check, char *s )
{
  int count = 0;
  char *cp = s;
	
  /* build checksum of all printing characters in s, while rotating */
  /* the partial checksum left one bit for each character added */
  while ( *cp )
    {
      if ( isgraph( *cp ) )
        {
          if ( *check < 0 )
            {
              (*check) <<= 1;
              (*check)++;
            }
          else
            {
              (*check) <<= 1;
            }
          (*check) += ( (long)(*cp) & 0xff );
        }
		
      cp++;
      if ( ++count > 80 )
        {
          why = "checksum unterminated string";
          return FALSE;
        }
    }
	
  return TRUE;
}

int alignment_level_index( int alignment )
{
  alignment = abs( alignment );
	
  if ( !alignment )
    return 0;
  else if ( alignment < 10 )
    return 1;
  else if ( alignment < 50 )
    return 2;
  else if ( alignment < 100 )
    return 3;
  else if ( alignment < 200 )
    return 4;
  else if ( alignment < 400 )
    return 5;
  else if ( alignment < 800 )
    return 6;
  else
    return 7;
}

char *alignment_level[ 8 ] =
{
  "Neutral, embodying the Cosmic Balance",
  "Neutral, tending toward ",
  "Neutral-",
  "",
  "Servant of ",
  "Master of ",
  "The Essence of ",
  "The Ultimate Avatar of "
};

char *alignment_string( int alignment )
{
  strcpy( Str3, alignment_level[ alignment_level_index( alignment ) ] );
		
  if ( alignment < 0 )
    strcat( Str3, "Chaos" );
  else if ( alignment > 0 )
    strcat( Str3, "Law" );
		
  return Str3;
}

char *elapsed_time_string( long minutes )
{
  long hours, days, months, years;
	
  hours = minutes / 60;
  days = hours / 24;
  months = days / 30;
  years = months / 12;
	
  minutes %= 60;
  hours %= 24;
  days %= 30;
  months %= 12;
	
  Str3[ 0 ] = '\0';
	
  if ( years )
    {
      sprintf( Str2, "%ld years, ", years );
      strcat( Str3, Str2 );
    }
	
  if ( years || months )
    {
      sprintf( Str2, "%ld months, ", months );
      strcat( Str3, Str2 );
    }
	
  if ( years || months || days )
    {
      sprintf( Str2, "%ld days, ", days );
      strcat( Str3, Str2 );
    }
	
  if ( years || months || days || hours )
    {
      sprintf( Str2, "%ld hours, ", hours );
      strcat( Str3, Str2 );
    }
	
  if ( years || months || days || hours || minutes )
    {
      sprintf( Str2, "%ld minutes", minutes );
      strcat( Str3, Str2 );
    }
	
  return Str3;
}

char *legion_rank_string( int rank )
{
  return legion_ranks[ rank ];
}

char *arena_rank_string( int rank )
{
  if ( rank < 0 )
    return arena_ranks[ 0 ];
  else
    return arena_ranks[ rank ];
}

char *college_rank_string( int rank )
{
  return college_ranks[ rank ];
}

char *nobility_rank_string( int rank )
{
  return nobility_ranks[ rank ];
}

char *circle_rank_string( int rank )
{
  if ( rank < 0 )
    return circle_ranks[ 0 ];
  else
    return circle_ranks[ rank ];
}

char *order_rank_string( int rank )
{
  if ( rank < 0 )
    return order_ranks[ 0 ];
  else
    return order_ranks[ rank ];
}

char *thieves_rank_string( int rank )
{
  return thieves_ranks[ rank ];
}

char *priesthood_rank_string( int rank, int patron )
{
  strcpy( Str3, priesthood_ranks[ rank ] );
  strcat( Str3, patron_names[ patron ] );
  return Str3;
}

char *monk_rank_string(int rank)
{
  return monk_ranks[rank];
}

