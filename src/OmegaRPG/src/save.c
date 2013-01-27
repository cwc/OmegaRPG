/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* save.c */

#if !defined(MSDOS) && !defined(WIN32)
#include <unistd.h>
#include <stdlib.h>
#endif

#include "glob.h"

#ifdef ALLOWING_OLD_SAVEFILES
/* holds the OLD_ definitions */
# include "oldsave.h"
#endif

/*Various functions for doing game saves and restores */
/*The game remembers various player information, the city level,
the country level, and the last or current dungeon level */

#if defined(MSDOS) || defined(WIN32) || defined(AMIGA)
void do_compression(int, char *);
#endif

/**************** SAVE FUNCTIONS */

/* Checks to see if save file already exists.
   Checks to see if save file can be opened for write.
   The player, the city level, and the current dungeon level are saved.
*/

int save_game(int compress, char *savestr)
{
    FILE *fd;
#ifdef SAVE_LEVELS
    int tmpdepth;
#endif
    int i,writeok=TRUE;
    plv current, save;
#ifdef COMPRESS_SAVE_FILES
    char temp[200];
#endif

#if !defined(MSDOS) && !defined(WIN32)
    int slashpos;

    if (access(savestr, R_OK) == 0)
        if (access(savestr, W_OK) == 0)
        {
            mprint(" Overwrite old file?");
            writeok = (ynq() == 'y');
        }
        else
        {
            mprint(" File already exists.");
            writeok = FALSE;
        }
    else
    {
        for (slashpos = strlen(savestr); slashpos > 0 && savestr[slashpos] != '/';
                slashpos--)
            ;
        if (slashpos > 0)
        {
            savestr[slashpos] = '\0';
            if (access(savestr, W_OK) == -1)
            {
                mprint(" Unable to save to that directory.");
                writeok = FALSE;
            }
            savestr[slashpos] = '/';
        }
    }
#endif
    change_to_user_perms();
    if (writeok) {
        fd = fopen(savestr,"wb");
        if (fd == NULL) {
            writeok = FALSE;
            mprint(" Error opening file.");
        }
    }
    if (! writeok)
    {
        morewait();
        print2("Save aborted.");
    }
    else {

        print1("Saving Game....");

        /* write the version number */
        i = VERSION;
        fwrite((char *)&i,sizeof(int),1,fd);
        /* write game id to save file */

        writeok &= save_player(fd);
        writeok &= save_country(fd);
#ifdef SAVE_LEVELS
        tmpdepth = Level->depth;
        City = msdos_changelevel(Level,E_CITY,0);
#endif
        writeok &= save_level(fd,City);

        if (Current_Environment == E_CITY || Current_Environment == E_COUNTRYSIDE)
            save = Dungeon;
        else if (Current_Environment == Current_Dungeon)
            save = Dungeon;
        else
            save = Level;
        for (i = 0, current = save; current; current = current->next, i++)
            ;
        if (!fwrite((char *)&i,sizeof(int),1,fd))
            writeok = FALSE;
#ifdef SAVE_LEVELS
        Level = msdos_changelevel(NULL,Current_Environment,tmpdepth);
#endif
        for (current = save; current; current = current->next)
            if (current != Level)
                writeok &= save_level(fd,current);
        if (save)
            writeok &= save_level(fd,Level);	/* put current level last */
        fclose(fd);
        if (writeok)
            print1("Game Saved.");
        else
            print1("Something didn't work... save aborted.");
#ifdef COMPRESS_SAVE_FILES
        if (writeok && compress) {
            print2("Compressing Save File....");
# if defined(MSDOS) || defined(AMIGA)
            do_compression(0, savestr);
            strcpy(temp, savestr);
            strcat(temp, "Z");
            rename(temp, savestr);
# else
            strcpy(temp,COMPRESSOR);
            strcat(temp," ");
            strcat(temp,savestr);
            system(temp);
            sprintf(temp, "%s.%s", savestr, COMPRESS_EXT);
            unlink(savestr);
            link(temp, savestr);
            unlink(temp);	/* renames, but sys-V doesn't have rename()... */
# endif
        }
#endif
        morewait();
        clearmsg();
    }
    change_to_game_perms();
    return(writeok);
}




/* saves game on SIGHUP */
/* no longer tries to compress, which hangs */
void signalsave(int ignore)
{
    change_to_user_perms();
    save_game(FALSE, "Omega.Sav");
#ifdef COMPRESS_SAVE_FILES
    print1("Signal - Saving uncompressed file 'Omega.Sav'.");
    print2("You can compress it yourself, if you like.");
#else
    print1("Signal - Saving file 'Omega.Sav'.");
#endif
    morewait();
    endgraf();
    exit(0);
}


/* also saves some globals like Level->depth... */

int save_player(FILE *fd)
{
    int i;
    int ok = 1;
    bank_account *account;

    /* Save random global state information */

    Player.click = (Tick + 1)%60;
    ok &= (fwrite((char *)&Player,sizeof(Player),1,fd) > 0);
    ok &= (fprintf(fd,"%s\n",Player.name) >= 0);
    ok &= (fwrite((char *)CitySiteList,sizeof(CitySiteList),1,fd) > 0);
    ok &= (fwrite((char *)&GameStatus,sizeof(long),1,fd) > 0);
    ok &= (fwrite((char *)&Current_Environment,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Last_Environment,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Current_Dungeon,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Villagenum,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Verbosity,sizeof(char),1,fd) > 0);
    ok &= (fwrite((char *)&Time,sizeof(long),1,fd) > 0);
    ok &= (fwrite((char *)&Tick,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Searchnum,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Behavior,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Phase,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Date,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Spellsleft,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&SalaryAmount,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&SalaryAccount,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Studiesleft,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&SymbolUseHour,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&SymbolUseDay,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&ViewHour,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&ViewDay,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&HelmHour,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&HelmDay,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Constriction,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Blessing,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&LastDay,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&RitualHour,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Lawstone,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Chaostone,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Mindstone,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Arena_Opponent,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Imprisonment,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Gymcredit,sizeof(long),1,fd) > 0);

    i = 0;
    for( account = bank; account; account = account->next_account ) i++;
    ok &= ( fwrite( (char *)&i, sizeof( int ), 1, fd ) > 0 );
    for( account = bank; account; account = account->next_account )
    {
        ok &= ( fwrite( (char *)(&(account->player)), sizeof(short), 1, fd ) > 0 );
        ok &= ( fwrite( (char *)(&(account->balance)), sizeof(long), 1, fd ) > 0 );
        ok &= ( fwrite( (char *)(&(account->number)), sizeof(long), 1, fd ) > 0 );
        ok &= ( fprintf( fd, "%s\n", account->password ) >= 0 );
    }

    ok &= (fwrite((char *)&StarGemUse,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&HiMagicUse,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&HiMagic,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&FixedPoints,sizeof(long),1,fd) > 0);
    ok &= (fwrite((char *)&LastCountryLocX,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&LastCountryLocY,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&LastTownLocX,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&LastTownLocY,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&Pawndate,sizeof(int),1,fd) > 0);

    ok &= (fwrite((char *)Spells,sizeof(Spells),1,fd) > 0);

    ok &= (fwrite((char *)&Command_Duration,sizeof(Command_Duration),1,fd) > 0);
    ok &= (fwrite((char *)&Precipitation,sizeof(Precipitation),1,fd) > 0);
    ok &= (fwrite((char *)&Lunarity,sizeof(Lunarity),1,fd) > 0);
    ok &= (fwrite((char *)&ZapHour,sizeof(ZapHour),1,fd) > 0);
    ok &= (fwrite((char *)&RitualRoom,sizeof(RitualRoom),1,fd) > 0);

    /* stuff which used to be statics */
    ok &= (fwrite((char *)&twiddle,sizeof(twiddle),1,fd) > 0);
    ok &= (fwrite((char *)&saved,sizeof(saved),1,fd) > 0);
    ok &= (fwrite((char *)&onewithchaos,sizeof(onewithchaos),1,fd) > 0);
    ok &= (fwrite((char *)&club_hinthour,sizeof(club_hinthour),1,fd) > 0);
    ok &= (fwrite((char *)&winnings,sizeof(winnings),1,fd) > 0);
    ok &= (fwrite((char *)&tavern_hinthour,sizeof(tavern_hinthour),1,fd) > 0);
    ok &= (fwrite((char *)scroll_ids,sizeof(scroll_ids),1,fd) > 0);
    ok &= (fwrite((char *)potion_ids,sizeof(potion_ids),1,fd) > 0);
    ok &= (fwrite((char *)stick_ids,sizeof(stick_ids),1,fd) > 0);
    ok &= (fwrite((char *)ring_ids,sizeof(ring_ids),1,fd) > 0);
    ok &= (fwrite((char *)cloak_ids,sizeof(cloak_ids),1,fd) > 0);
    ok &= (fwrite((char *)boot_ids,sizeof(boot_ids),1,fd) > 0);
    ok &= (fwrite((char *)deepest,sizeof(int),E_MAX + 1,fd) > 0);
    ok &= (fwrite((char *)level_seed,sizeof(int),E_MAX + 1,fd) > 0);

    /* Save player possessions */

    if (Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND])
        Player.possessions[O_READY_HAND] = NULL;
    for(i=0; i<MAXITEMS; i++) ok &= save_item(fd,Player.possessions[i]);
    for(i=0; i<MAXPACK; i++) ok &= save_item(fd,Player.pack[i]);
    for(i=0; i<PAWNITEMS; i++) ok &= save_item(fd,Pawnitems[i]);

    /* Save items in condo vault */
    ok &= save_itemlist(fd,Condoitems);
    /* Save items in bag of holding */
    ok &= save_itemlist(fd,Bagitems);

    /* Save player item knowledge */
    for (i=0; i<TOTALITEMS; i++)
    {
        ok &= (fwrite((char *)&(Objects[i].known),sizeof(Objects[i].known),1,fd) > 0);
        ok &= (fwrite((char *)&(Objects[i].uniqueness),sizeof(Objects[i].uniqueness),1,fd) > 0);
    }
    return ok;
}


/* Save whatever is pointed to by level */
int save_level(FILE *fd, plv level)
{
    int i, j, run;
    unsigned long int mask;
    int ok = 1;
    int width = level->level_width;
    int length = level->level_length;

    ok &= (fwrite((char *)&level->depth,sizeof(char),1,fd) > 0);
    ok &= (fwrite((char *)&level->numrooms,sizeof(char),1,fd) > 0);
    ok &= (fwrite((char *)&level->tunnelled,sizeof(char),1,fd) > 0);
    ok &= (fwrite((char *)&level->environment,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&level->level_width,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&level->level_length,sizeof(int),1,fd) > 0);
    for (j = 0; j < length; j++)
        for (i = 0; i < width; i++)
            if (level->site[i][j].lstatus&CHANGED) {	/* this loc has been changed */
                for (run = i + 1; run < width &&	/* find how many in a row */
                        level->site[run][j].lstatus&CHANGED; run++)
                    ;
                ok &= (fwrite((char *)&i,sizeof(int),1,fd) > 0);
                ok &= (fwrite((char *)&j,sizeof(int),1,fd) > 0);
                ok &= (fwrite((char *)&run,sizeof(int),1,fd) > 0);
                for (; i < run; i++)
                    ok &= (fwrite((char *)&level->site[i][j],sizeof(struct location),1,fd) > 0);
            }
    ok &= (fwrite((char *)&i,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&j,sizeof(int),1,fd) > 0);	/* signify end */
    /* since we don't mark the 'seen' bits as CHANGED, need to save a bitmask */
    run = 8*sizeof(long int);
    mask = 0;
    for (j = 0; j < length; j++)
        for (i = 0; i < width; i++) {
            if (run == 0) {
                run = 8*sizeof(long int);
                ok &= (fwrite((char *)&mask,sizeof(long int),1,fd) > 0);
                mask = 0;
            }
            mask >>= 1;
            if (level->site[i][j].lstatus&SEEN)
                mask |= (1<<(sizeof(long int)*8 - 1));
            run--;
        }
    if (run < 8*sizeof(long int))
        ok &= (fwrite((char *)&mask,sizeof(long int),1,fd) > 0);
    ok &= save_monsters(fd,level->mlist);
    for(i=0; i<width; i++)
        for(j=0; j<length; j++)
            if (level->site[i][j].things) {
                ok &= (fwrite((char *)&i,sizeof(int),1,fd) > 0);
                ok &= (fwrite((char *)&j,sizeof(int),1,fd) > 0);
                ok &= save_itemlist(fd,level->site[i][j].things);
            }
    ok &= (fwrite((char *)&i,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&j,sizeof(int),1,fd) > 0);	/* signify end */
    return ok;
}


int save_monsters(FILE *fd, pml ml)
{
    pml tml;
    int nummonsters=0;
    int ok = 1;
    unsigned char type;

    /* First count monsters */
    for(tml=ml; tml!=NULL; tml=tml->next)
        if (tml->m->hp > 0) nummonsters++;

    ok &= (fwrite((char *)&nummonsters,sizeof(int),1,fd) > 0);

    /* Now save monsters */
    for(tml=ml; tml!=NULL; tml=tml->next) {
        if (tml->m->hp > 0) {
            ok &= (fwrite((char *)tml->m,sizeof(montype),1,fd) > 0);
            if (tml->m->id != HISCORE_NPC) {
                type = 0x0;
                /* DAG use pointer compare rather than strcmp; a bit more effecient */
                if (tml->m->monstring != Monsters[tml->m->id].monstring)
                    type |= 0x1;
                if (tml->m->corpsestr != Monsters[tml->m->id].corpsestr)
                    type |= 0x2;
                /* DAG expect from now (version 9000) on that type will always be  */
                /*     0x3 (both) saved, but leave this way as it may not, and this  */
                /*     preserves save-file compatibility without requiring code changes */
                ok &= (fwrite((char *)&type,sizeof(unsigned char),1,fd) > 0);
                if (type&1)
                    ok &= (fprintf(fd,"%s\n",tml->m->monstring) >= 0);
                if (type&2)
                    ok &= (fprintf(fd,"%s\n",tml->m->corpsestr) >= 0);
                /* WDT: line moved from here... */
            }	/* else it'll be reloaded from the hiscore file on restore */
            /* WDT: to here.  This bug fix is Sheldon Simm's suggestion
             * to fix the well-known 'Star Gem' bug; it should allow the
             * possessions of hiscore NPCs to be restored from the savefile.
             * See also the complementary change in restore_monsters. */
            ok &= save_itemlist(fd,tml->m->possessions);
        }
    }
    return ok;
}


/* Save o unless it's null, then save a special flag byte instead */
/* Use other values of flag byte to indicate what strings are saved */
int save_item(FILE *fd, pob o)
{
    int ok = 1;
    unsigned char type;

    if (o == NULL) {
        type = 0xff;
        ok &= (fwrite((char *)&type,sizeof(type),1,fd) > 0);
    }
    else {
        type = 0;
        /* DAG these strcmp could be just pointer comparisons, more efficient. */
        if (strcmp(o->objstr, Objects[o->id].objstr))
            type |= 1;
        if (strcmp(o->truename, Objects[o->id].truename))
            type |= 2;
        if (strcmp(o->cursestr, Objects[o->id].cursestr))
            type |= 4;
        /* DAG following is true for corpses */
        if ( (type && o->cursestr == o->objstr) && (o->cursestr == o->truename) )
            type |=8;
        ok &= (fwrite((char *)&type,sizeof(type),1,fd) > 0);
        ok &= (fwrite((char *)o,sizeof(objtype),1,fd) > 0);
        if (type&1)
            ok &= (fprintf(fd,"%s\n",o->objstr) >= 0);
        if (type&2)
            ok &= (fprintf(fd,"%s\n",o->truename) >= 0);
        if (type&4)
            ok &= (fprintf(fd,"%s\n",o->cursestr) >= 0);
    }
    return ok;
}

int save_itemlist(FILE *fd, pol ol)
{
    int numitems = 0;
    pol tol;
    int ok = 1;

    for(tol=ol; tol!=NULL; tol=tol->next) numitems++;
    ok &= (fwrite((char *)&numitems,sizeof(int),1,fd) > 0);
    for(tol=ol; tol!=NULL; tol=tol->next)
        ok &= save_item(fd,tol->thing);
    return ok;
}


int save_country(FILE *fd)
{
    int i, j;
    int ok = 1;
    int run;
    unsigned long int mask;

    for (i = 0; i < COUNTRY_WIDTH; i++)
        for (j = 0; j < COUNTRY_LENGTH; j++)
            if (c_statusp(i, j, CHANGED)) {
                ok &= (fwrite((char *)&i,sizeof(int),1,fd) > 0);
                ok &= (fwrite((char *)&j,sizeof(int),1,fd) > 0);
                ok &= (fwrite((char *)&Country[i][j],sizeof(struct terrain),1,fd) > 0);
            }
    ok &= (fwrite((char *)&i,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&j,sizeof(int),1,fd) > 0);
    /* since we don't mark the 'seen' bits as CHANGED, need to save a bitmask */
    run = 8*sizeof(long int);
    mask = 0;
    for (i = 0; i < COUNTRY_WIDTH; i++)
        for (j = 0; j < COUNTRY_LENGTH; j++) {
            if (run == 0) {
                run = 8*sizeof(long int);
                ok &= (fwrite((char *)&mask,sizeof(long int),1,fd) > 0);
                mask = 0;
            }
            mask >>= 1;
            if (c_statusp(i, j, SEEN))
                mask |= (1<<(sizeof(long int)*8 - 1));
            run--;
        }
    if (run < 8*sizeof(long int))
        ok &= (fwrite((char *)&mask,sizeof(long int),1,fd) > 0);
    return ok;
}


/* returns TRUE if the given version can be restored by this version */
int ok_outdated(int version)
{
    /* currently all backwards compatibility is broken, so fail ok_outdated()
       until we actually re-install some type of backwards compatibility. DAG */

    return FALSE;

#if 0
    switch (version) {
    case 80:
        print1("Converting version 0.80 savefile to current.");
        morewait();
        return TRUE;
        break;
    case 81:
        print1("Converting version 0.81 savefile to current.");
        morewait();
        return TRUE;
        break;
    case 90:
        print1("Converting version 0.90 savefile to current.");
        morewait();
        return TRUE;
        break;
    case 91:
        /* version 91 is same as version 9000 right now */
        return TRUE;
        break;
    case 9000:
        /* DAG - 9000 is the re-numbered 90, for the new version numbering system. */
        print1("Converting version 0.90 savefile to current.");
        morewait();
        return TRUE;
        break;
    default:
        return FALSE;
        break;
    }
#endif /* #if 0 */
}


/* read player data, city level, dungeon level,
   check on validity of save file, etc.
   return TRUE if game restored, FALSE otherwise */

int restore_game(char *savestr)
{
    int i,version;
#ifdef COMPRESS_SAVE_FILES
    char temp[200];
#endif
    FILE *fd;

#if !defined(MSDOS) && !defined(WIN32)
    if (access(savestr, F_OK|R_OK|W_OK) == -1) /* access uses real uid */
    {
        print1("Unable to access save file: ");
        nprint1(savestr);
        morewait();
        return FALSE;
    }
#endif
    change_to_user_perms();
#ifdef COMPRESS_SAVE_FILES
    fd = fopen(savestr,"rb");
    if (fd == NULL) {
        print1("Error restoring game -- aborted.");
        print2("File name was: ");
        nprint2(savestr);
        morewait();
        change_to_game_perms();
        return(FALSE);
    }
    fread((char *)&version,sizeof(int),1,fd);
    fclose(fd);
    if (VERSION != version && !ok_outdated(version)) {
        print1("Uncompressing Save File....");
#if defined(MSDOS) || defined(AMIGA)
        strcpy(temp, savestr);
        strcat(temp, "Z");
        rename(savestr, temp);
        do_compression(1, savestr);
#else
        sprintf(temp, "%s.%s", savestr, COMPRESS_EXT);
        unlink(temp);
        link(savestr, temp);
        unlink(savestr);	/* renames, but sys-V doesn't have rename()... */
        strcpy(temp,UNCOMPRESSOR);
        strcat(temp," ");
        strcat(temp,savestr);
        system(temp);
#endif
        print2("Save file uncompressed.");
        morewait();
    }
#endif

    fd = fopen(savestr,"rb");

    if (fd == NULL) {
        print1("Error restoring game -- aborted.");
        print2("File name was: ");
        nprint2(savestr);
        morewait();
        change_to_game_perms();
        return(FALSE);
    }
    else {
        print1("Restoring...");

        fread((char *)&version,sizeof(int),1,fd);

        if (VERSION != version && !ok_outdated(version)) {
            change_to_game_perms();
            fclose(fd);
            clearmsg();
            mprint(" Sorry, I can't restore an outdated save file!");
            mprint(" savefile is version ");
            mnumprint(version/10000);
            nprint2(".");
            mnumprint( (version/100)%100);
            morewait();
            return(FALSE);
        }
        restore_player(fd, version);
        restore_country(fd, version);
        restore_level(fd, version); /* the city level */
        fread((char *)&i,sizeof(int),1,fd);
        for (; i > 0; i--) {
#ifdef SAVE_LEVELS
            msdos_changelevel(Level,0,-1);
#endif
            restore_level(fd, version);
            if (Level->environment == Current_Dungeon) {
                Level->next = Dungeon;
                Dungeon = Level;
            }
            if (Current_Environment == E_CITY)
                Level = City;
        }

        fclose(fd);
        print3("Restoration complete.");
        ScreenOffset = -1000;	/* to force a redraw */
        setgamestatus(SKIP_MONSTERS);
        change_to_game_perms();
        return(TRUE);
    }
}

void restore_player(FILE *fd, int version)
{
    int i;

    // Bank information
    int num_accounts;
    bank_account *account;
    char pw_buf[ 64 ];

    fread((char *)&Player,sizeof(Player),1,fd);

    filescanstring(fd,Player.name);

#ifdef ALLOWING_OLD_SAVEFILES
    if( version < 91 ) /* DAG */
        fread((char *)CitySiteList,(3*OLD_NUMCITYSITES*sizeof(int)),1,fd);
    else
#endif

	fread((char *)CitySiteList,sizeof(CitySiteList),1,fd);
    fread((char *)&GameStatus,sizeof(long),1,fd);
    fread((char *)&Current_Environment,sizeof(int),1,fd);
    fread((char *)&Last_Environment,sizeof(int),1,fd);
    fread((char *)&Current_Dungeon,sizeof(int),1,fd);
    fread((char *)&Villagenum,sizeof(int),1,fd);
    switch(Current_Dungeon) {
    case E_ASTRAL:
        MaxDungeonLevels = ASTRALLEVELS;
        break;
    case E_SEWERS:
        MaxDungeonLevels = SEWERLEVELS;
        break;
    case E_CASTLE:
        MaxDungeonLevels = CASTLELEVELS;
        break;
    case E_CAVES:
        MaxDungeonLevels = CAVELEVELS;
        break;
    case E_VOLCANO:
        MaxDungeonLevels = VOLCANOLEVELS;
        break;
    case E_PALACE:
        MaxDungeonLevels = PALACELEVELS;
        break;
    }
    fread((char *)&Verbosity,sizeof(char),1,fd);
    fread((char *)&Time,sizeof(long),1,fd);
    fread((char *)&Tick,sizeof(int),1,fd);
    fread((char *)&Searchnum,sizeof(int),1,fd);
    fread((char *)&Behavior,sizeof(int),1,fd);
    fread((char *)&Phase,sizeof(int),1,fd);
    fread((char *)&Date,sizeof(int),1,fd);
    fread((char *)&Spellsleft,sizeof(int),1,fd);
    fread((char *)&SalaryAmount,sizeof(int),1,fd);
    fread((char *)&SalaryAccount,sizeof(int),1,fd);
    fread((char *)&Studiesleft,sizeof(int),1,fd);
    fread((char *)&SymbolUseHour,sizeof(int),1,fd);
    fread((char *)&SymbolUseDay,sizeof(int),1,fd);
    fread((char *)&ViewHour,sizeof(int),1,fd);
    fread((char *)&ViewDay,sizeof(int),1,fd);
    fread((char *)&HelmHour,sizeof(int),1,fd);
    fread((char *)&HelmDay,sizeof(int),1,fd);
    fread((char *)&Constriction,sizeof(int),1,fd);
    fread((char *)&Blessing,sizeof(int),1,fd);
    fread((char *)&LastDay,sizeof(int),1,fd);
    fread((char *)&RitualHour,sizeof(int),1,fd);
    fread((char *)&Lawstone,sizeof(int),1,fd);
    fread((char *)&Chaostone,sizeof(int),1,fd);
    fread((char *)&Mindstone,sizeof(int),1,fd);
    fread((char *)&Arena_Opponent,sizeof(int),1,fd);
    fread((char *)&Imprisonment,sizeof(int),1,fd);
    fread((char *)&Gymcredit,sizeof(long),1,fd);

    // Read bank information
    fread( (char *)&num_accounts, sizeof( int ), 1, fd );
    for( i = 0; i < num_accounts; i++ )
    {
        account = (bank_account *)checkmalloc( sizeof( bank_account ));
        fread( (char *)(&(account->player)), sizeof(short), 1, fd );
        fread( (char *)(&(account->balance)), sizeof(long), 1, fd );
        fread( (char *)(&(account->number)), sizeof(long), 1, fd );
        filescanstring( fd, pw_buf );
        account->password = salloc( pw_buf );
        account->next_account = bank;
        bank = account;
    }

    fread((char *)&StarGemUse,sizeof(int),1,fd);
    fread((char *)&HiMagicUse,sizeof(int),1,fd);
    fread((char *)&HiMagic,sizeof(int),1,fd);
    fread((char *)&FixedPoints,sizeof(long),1,fd);
    fread((char *)&LastCountryLocX,sizeof(int),1,fd);
    fread((char *)&LastCountryLocY,sizeof(int),1,fd);
    fread((char *)&LastTownLocX,sizeof(int),1,fd);
    fread((char *)&LastTownLocY,sizeof(int),1,fd);
    fread((char *)&Pawndate,sizeof(int),1,fd);

    fread((char *)Spells,sizeof(Spells),1,fd);

    fread((char *)&Command_Duration,sizeof(Command_Duration),1,fd);
    fread((char *)&Precipitation,sizeof(Precipitation),1,fd);
    fread((char *)&Lunarity,sizeof(Lunarity),1,fd);
    fread((char *)&ZapHour,sizeof(ZapHour),1,fd);
    fread((char *)&RitualRoom,sizeof(RitualRoom),1,fd);

    /* stuff which used to be statics */
    fread((char *)&twiddle,sizeof(twiddle),1,fd);
    fread((char *)&saved,sizeof(saved),1,fd);
    fread((char *)&onewithchaos,sizeof(onewithchaos),1,fd);
    fread((char *)&club_hinthour,sizeof(club_hinthour),1,fd);
    fread((char *)&winnings,sizeof(winnings),1,fd);
    fread((char *)&tavern_hinthour,sizeof(tavern_hinthour),1,fd);
    fread((char *)scroll_ids,sizeof(scroll_ids),1,fd);
    fread((char *)potion_ids,sizeof(potion_ids),1,fd);
    fread((char *)stick_ids,sizeof(stick_ids),1,fd);
    fread((char *)ring_ids,sizeof(ring_ids),1,fd);
    fread((char *)cloak_ids,sizeof(cloak_ids),1,fd);
    fread((char *)boot_ids,sizeof(boot_ids),1,fd);
    fread((char *)deepest,sizeof(int),E_MAX + 1,fd);
    fread((char *)level_seed,sizeof(int),E_MAX + 1,fd);

    /* Set up the strings for the id's */
    inititem(FALSE);

    for(i=0; i<MAXITEMS; i++)
        Player.possessions[i] = restore_item(fd, version);

    if (!Player.possessions[O_READY_HAND] && Player.possessions[O_WEAPON_HAND] &&
            twohandedp(Player.possessions[O_WEAPON_HAND]->id))
        Player.possessions[O_READY_HAND] = Player.possessions[O_WEAPON_HAND];

    for(i=0; i<MAXPACK; i++)
        Player.pack[i] = restore_item(fd, version);
    for(i=0; i<PAWNITEMS; i++)
        Pawnitems[i] = restore_item(fd, version);
    Condoitems = restore_itemlist(fd, version);
    /* Added bag of holding in 9001, so need to restore list. */
    if (version < 9001)
        Bagitems = NULL;
    else
        Bagitems = restore_itemlist(fd, version);

    for (i=0; i<TOTALITEMS; i++)
    {
        /* Add bag of holding, skip its position in the 2 arrays */
        if ( (version < 9001) && (i == 215) )
            i = 216;
        fread((char *)&(Objects[i].known),sizeof(Objects[i].known),1,fd);
        if (version != 80)
            fread((char *)&(Objects[i].uniqueness),sizeof(Objects[i].uniqueness),1,fd);
    }
}

/* Restore an item, the first byte tells us if it's NULL, and what strings */
/* have been saved as different from the typical */
pob restore_item(FILE *fd, int version)
{
    char tempstr[80];
    unsigned char type;
    pob obj = NULL;

    fread((char *)&type,sizeof(type),1,fd);
    if (type != 0xff) {
        obj = ((pob) checkmalloc(sizeof(objtype)));
        fread((char *)obj,sizeof(objtype),1,fd);

        /* DAG -- added object 215 (bag of holding) in 9001;  have to renumber */
        if (version < 9001 )
            if (obj->id >= 215)
                obj->id++;

        if (type&8)
        {
            /* DAG case to handle corpses */
            if (type&1)
                filescanstring(fd,tempstr);
            if (type&2)
                filescanstring(fd,tempstr);
            if (type&4)
                filescanstring(fd,tempstr);
            obj->objstr = obj->truename = obj->cursestr = salloc(tempstr);
        }
        else
        {
            if (type&1) {
                filescanstring(fd,tempstr);
                obj->objstr = salloc(tempstr);
            }
            else
                obj->objstr = Objects[obj->id].objstr;
            if (type&2) {
                filescanstring(fd,tempstr);
                obj->truename = salloc(tempstr);
            }
            else
                obj->truename = Objects[obj->id].truename;
            if (type&4) {
                filescanstring(fd,tempstr);
                obj->cursestr = salloc(tempstr);
            }
            else
                obj->cursestr = Objects[obj->id].cursestr;
        }
    }
    return obj;
}

pol restore_itemlist(FILE *fd, int version)
{
    pol ol=NULL,cur=NULL,new=NULL;
    int i,numitems,firsttime=TRUE;
    fread((char *)&numitems,sizeof(int),1,fd);
    for(i=0; i<numitems; i++) {
        new = ((pol) checkmalloc(sizeof(oltype)));
        new->thing = restore_item(fd, version);
        new->next = NULL;
        if (firsttime==TRUE) {
            ol = cur = new;
            firsttime = FALSE;
        }
        else {
            cur->next = new;
            cur = new;
        }
    }
    return(ol);
}

/* converts old location function ids to current ones.  DAG */
void fix_p_locf( unsigned char *p_locf, int version )
/* char *p_locf is pointer to single char, not a string */
{
#ifdef ALLOWING_OLD_SAVEFILES
    if( version < 91 )
    {
        switch( *p_locf )
        {
        case OLD_L_NO_OP                 :
            *p_locf =  L_NO_OP;
            break;
        case OLD_L_LIFT                  :
            *p_locf =  L_LIFT;
            break;
        case OLD_L_BALANCESTONE          :
            *p_locf =  L_BALANCESTONE;
            break;
        case OLD_L_FIRE                  :
            *p_locf =  L_FIRE;
            break;
        case OLD_L_WHIRLWIND             :
            *p_locf =  L_WHIRLWIND;
            break;
        case OLD_L_VOIDSTONE             :
            *p_locf =  L_VOIDSTONE;
            break;
        case OLD_L_WARNING               :
            *p_locf =  L_WARNING;
            break;
        case OLD_L_ARENA_EXIT            :
            *p_locf =  L_ARENA_EXIT;
            break;
        case OLD_L_HOUSE_EXIT            :
            *p_locf =  L_HOUSE_EXIT;
            break;
        case OLD_L_SAFE                  :
            *p_locf =  L_SAFE;
            break;
        case OLD_L_CHARITY               :
            *p_locf =  L_CHARITY;
            break;
        case OLD_L_ARMORER               :
            *p_locf =  L_ARMORER;
            break;
        case OLD_L_CLUB                  :
            *p_locf =  L_CLUB;
            break;
        case OLD_L_GYM                   :
            *p_locf =  L_GYM;
            break;
        case OLD_L_THIEVES_GUILD         :
            *p_locf =  L_THIEVES_GUILD;
            break;
        case OLD_L_COLLEGE               :
            *p_locf =  L_COLLEGE;
            break;
        case OLD_L_HEALER                :
            *p_locf =  L_HEALER;
            break;
        case OLD_L_CASINO                :
            *p_locf =  L_CASINO;
            break;
        case OLD_L_TAVERN                :
            *p_locf =  L_TAVERN;
            break;
        case OLD_L_MERC_GUILD            :
            *p_locf =  L_MERC_GUILD;
            break;
        case OLD_L_ALCHEMIST             :
            *p_locf =  L_ALCHEMIST;
            break;
        case OLD_L_SORCERORS             :
            *p_locf =  L_SORCERORS;
            break;
        case OLD_L_CASTLE                :
            *p_locf =  L_CASTLE;
            break;
        case OLD_L_ARENA                 :
            *p_locf =  L_ARENA;
            break;
        case OLD_L_DPW                   :
            *p_locf =  L_DPW;
            break;
        case OLD_L_LIBRARY               :
            *p_locf =  L_LIBRARY;
            break;
        case OLD_L_PAWN_SHOP             :
            *p_locf =  L_PAWN_SHOP;
            break;
        case OLD_L_BANK                  :
            *p_locf =  L_BANK;
            break;
        case OLD_L_CONDO                 :
            *p_locf =  L_CONDO;
            break;
        case OLD_L_ORACLE                :
            *p_locf =  L_ORACLE;
            break;
        case OLD_L_ORDER                 :
            *p_locf =  L_ORDER;
            break;
        case OLD_L_DINER                 :
            *p_locf =  L_DINER;
            break;
        case OLD_L_COMMANDANT            :
            *p_locf =  L_COMMANDANT;
            break;
        case OLD_L_CRAP                  :
            *p_locf =  L_CRAP;
            break;
        case OLD_L_TEMPLE                :
            *p_locf =  L_TEMPLE;
            break;
        case OLD_L_COUNTRYSIDE           :
            *p_locf =  L_COUNTRYSIDE;
            break;
        case OLD_L_BROTHEL               :
            *p_locf =  L_BROTHEL;
            break;
        case OLD_L_JAIL                  :
            *p_locf =  L_JAIL;
            break;
        case OLD_L_TEMPLE_WARNING        :
            *p_locf =  L_TEMPLE_WARNING;
            break;
        case OLD_L_LAWSTONE              :
            *p_locf =  L_LAWSTONE;
            break;
        case OLD_L_CHAOSTONE             :
            *p_locf =  L_CHAOSTONE;
            break;
        case OLD_L_EARTH_STATION         :
            *p_locf =  L_EARTH_STATION;
            break;
        case OLD_L_FIRE_STATION          :
            *p_locf =  L_FIRE_STATION;
            break;
        case OLD_L_WATER_STATION         :
            *p_locf =  L_WATER_STATION;
            break;
        case OLD_L_AIR_STATION           :
            *p_locf =  L_AIR_STATION;
            break;
        case OLD_L_VOID_STATION          :
            *p_locf =  L_VOID_STATION;
            break;
        case OLD_L_VOID                  :
            *p_locf =  L_VOID;
            break;
        case OLD_L_VOICE1                :
            *p_locf =  L_VOICE1;
            break;
        case OLD_L_VOICE2                :
            *p_locf =  L_VOICE2;
            break;
        case OLD_L_VOICE3                :
            *p_locf =  L_VOICE3;
            break;
        case OLD_L_SACRIFICESTONE        :
            *p_locf =  L_SACRIFICESTONE;
            break;
        case OLD_L_TOME1                 :
            *p_locf =  L_TOME1;
            break;
        case OLD_L_TOME2                 :
            *p_locf =  L_TOME2;
            break;
        case OLD_L_ENTER_CIRCLE          :
            *p_locf =  L_ENTER_CIRCLE;
            break;
        case OLD_L_CIRCLE_LIBRARY        :
            *p_locf =  L_CIRCLE_LIBRARY;
            break;
        case OLD_L_DRUID                 :
            *p_locf =  L_DRUID;
            break;
        case OLD_L_ALTAR                 :
            *p_locf =  L_ALTAR;
            break;
        case OLD_L_GARDEN                :
            *p_locf =  L_GARDEN;
            break;
        case OLD_L_ADEPT                 :
            *p_locf =  L_ADEPT;
            break;
        case OLD_L_SEWER                 :
            *p_locf =  L_SEWER;
            break;
        case OLD_L_OMEGA                 :
            *p_locf =  L_OMEGA;
            break;
        case OLD_L_CARTOGRAPHER          :
            *p_locf =  L_CARTOGRAPHER;
            break;
        case OLD_L_STABLES               :
            *p_locf =  L_STABLES;
            break;
        case OLD_L_COMMONS               :
            *p_locf =  L_COMMONS;
            break;
        case OLD_L_GRANARY               :
            *p_locf =  L_GRANARY;
            break;
        case OLD_L_MAZE                  :
            *p_locf =  L_MAZE;
            break;
        case OLD_L_HOVEL                 :
            *p_locf =  L_HOVEL;
            break;
        case OLD_L_HOUSE                 :
            *p_locf =  L_HOUSE;
            break;
        case OLD_L_MANSION               :
            *p_locf =  L_MANSION;
            break;
        case OLD_L_OCCUPIED_HOUSE        :
            *p_locf =  L_OCCUPIED_HOUSE;
            break;
        case OLD_L_TACTICAL_EXIT         :
            *p_locf =  L_TACTICAL_EXIT;
            break;
        case OLD_L_VAULT                 :
            *p_locf =  L_VAULT;
            break;
        case OLD_L_CEMETARY              :
            *p_locf =  L_CEMETARY;
            break;
        case OLD_L_THRONE                :
            *p_locf =  L_THRONE;
            break;
        case OLD_L_ESCALATOR             :
            *p_locf =  L_ESCALATOR;
            break;
        case OLD_L_ENTER_COURT           :
            *p_locf =  L_ENTER_COURT;
            break;
        case OLD_L_TRIFID                :
            *p_locf =  L_TRIFID;
            break;
        case OLD_L_FINAL_ABYSS           :
            *p_locf =  L_FINAL_ABYSS;
            break;
        case OLD_L_RAISE_PORTCULLIS      :
            *p_locf =  L_RAISE_PORTCULLIS;
            break;
        case OLD_L_MINDSTONE             :
            *p_locf =  L_MINDSTONE;
            break;
        case OLD_L_CHAOS                 :
            *p_locf =  L_CHAOS;
            break;
        case OLD_L_WATER                 :
            *p_locf =  L_WATER;
            break;
        case OLD_L_LAVA                  :
            *p_locf =  L_LAVA;
            break;
        case OLD_L_MAGIC_POOL            :
            *p_locf =  L_MAGIC_POOL;
            break;
        case OLD_L_PORTCULLIS_TRAP       :
            *p_locf =  L_PORTCULLIS_TRAP;
            break;
        case OLD_L_DROP_EVERY_PORTCULLIS :
            *p_locf =  L_DROP_EVERY_PORTCULLIS;
            break;
        case OLD_L_PORTCULLIS            :
            *p_locf =  L_PORTCULLIS;
            break;
        case OLD_L_TRAP_DART             :
            *p_locf =  L_TRAP_DART;
            break;
        case OLD_L_TRAP_PIT              :
            *p_locf =  L_TRAP_PIT;
            break;
        case OLD_L_TRAP_DOOR             :
            *p_locf =  L_TRAP_DOOR;
            break;
        case OLD_L_TRAP_SNARE            :
            *p_locf =  L_TRAP_SNARE;
            break;
        case OLD_L_TRAP_BLADE            :
            *p_locf =  L_TRAP_BLADE;
            break;
        case OLD_L_TRAP_FIRE             :
            *p_locf =  L_TRAP_FIRE;
            break;
        case OLD_L_TRAP_TELEPORT         :
            *p_locf =  L_TRAP_TELEPORT;
            break;
        case OLD_L_TRAP_DISINTEGRATE     :
            *p_locf =  L_TRAP_DISINTEGRATE;
            break;
        case OLD_L_TRAP_SLEEP_GAS        :
            *p_locf =  L_TRAP_SLEEP_GAS;
            break;
        case OLD_L_TRAP_ACID             :
            *p_locf =  L_TRAP_ACID;
            break;
        case OLD_L_TRAP_MANADRAIN        :
            *p_locf =  L_TRAP_MANADRAIN;
            break;
        case OLD_L_TRAP_ABYSS            :
            *p_locf =  L_TRAP_ABYSS;
            break;
        case OLD_L_TRAP_SIREN            :
            *p_locf =  L_TRAP_SIREN;
            break;
        case OLD_L_STATUE_WAKE           :
            *p_locf =  L_STATUE_WAKE;
            break;
        case OLD_L_STATUE_RANDOM         :
            *p_locf =  L_STATUE_RANDOM;
            break;
        case OLD_L_HEDGE                 :
            *p_locf =  L_HEDGE;
            break;
        case OLD_L_RUBBLE                :
            *p_locf =  L_RUBBLE;
            break;
        case OLD_L_ABYSS                 :
            *p_locf =  L_ABYSS;
            break;
        default:
            *p_locf = L_NO_OP;
            break; /* shouldn't happen */
        }
    }
#endif
}

void restore_level(FILE *fd, int version)
{
    int i, j, run;
    unsigned long int mask;
    int temp_env;
    int length, width;

    Level = (plv) checkmalloc(sizeof(levtype));
    clear_level(Level);
    fread((char *)&Level->depth,sizeof(char),1,fd);
    fread((char *)&Level->numrooms,sizeof(char),1,fd);
    fread((char *)&Level->tunnelled,sizeof(char),1,fd);
    fread((char *)&Level->environment,sizeof(int),1,fd);
    fread((char *)&Level->level_width,sizeof(int),1,fd);
    fread((char *)&Level->level_length,sizeof(int),1,fd);

    width = Level->level_width;
    length = Level->level_length;
    Level->generated = TRUE;
    temp_env = Current_Environment;
    Current_Environment = Level->environment;
    switch(Level->environment) {
    case E_COUNTRYSIDE:
        load_country();
        break;
    case E_CITY:
        load_city(FALSE);
        break;
    case E_VILLAGE:
        load_village(Country[LastCountryLocX][LastCountryLocY].aux, FALSE);
        break;
    case E_CAVES:
        initrand(Current_Environment, Level->depth);
        if ((random_range(4)==0) && (Level->depth < MaxDungeonLevels))
            room_level();
        else cavern_level();
        break;
    case E_SEWERS:
        initrand(Current_Environment, Level->depth);
        if ((random_range(4)==0) && (Level->depth < MaxDungeonLevels))
            room_level();
        else sewer_level();
        break;
    case E_CASTLE:
        initrand(Current_Environment, Level->depth);
        room_level();
        break;
    case E_PALACE:
        initrand(Current_Environment, Level->depth);
        room_level(); /* TODO PGM */
        break;
    case E_ASTRAL:
        initrand(Current_Environment, Level->depth);
        maze_level();
        break;
    case E_VOLCANO:
        initrand(Current_Environment, Level->depth);
        switch(random_range(3)) {
        case 0:
            cavern_level();
            break;
        case 1:
            room_level();
            break;
        case 2:
            maze_level();
            break;
        }
        break;
    case E_HOVEL:
    case E_MANSION:
    case E_HOUSE:
        load_house(Level->environment, FALSE);
        break;
    case E_DLAIR:
        load_dlair(gamestatusp(KILLED_DRAGONLORD), FALSE);
        break;
    case E_STARPEAK:
        load_speak(gamestatusp(KILLED_LAWBRINGER), FALSE);
        break;
    case E_MAGIC_ISLE:
        load_misle(gamestatusp(KILLED_EATER), FALSE);
        break;
    case E_TEMPLE:
        load_temple(Country[LastCountryLocX][LastCountryLocY].aux, FALSE);
        break;
    case E_CIRCLE:
        load_circle(FALSE);
        break;
    case E_COURT:
        load_court(FALSE);
        break;
    default:
        print3("This dungeon not implemented!");
        break;
    }
    if (Level->depth > 0) {	/* dungeon... */
        install_traps();
        install_specials();
        make_stairs(-1);
        make_stairs(-1);
        initrand(E_RESTORE, 0);
    }
    Current_Environment = temp_env;
    fread((char *)&i,sizeof(int),1,fd);
    fread((char *)&j,sizeof(int),1,fd);
    while ((j < length) && (i < width)) {
        fread((char *)&run,sizeof(int),1,fd);
        for (; i < run; i++) {
            fread((char *)&Level->site[i][j],sizeof(struct location),1,fd);
            fix_p_locf( &(Level->site[i][j].p_locf), version ); /* DAG */
            Level->site[i][j].creature = NULL;
            Level->site[i][j].things = NULL;
        }
        fread((char *)&i,sizeof(int),1,fd);
        fread((char *)&j,sizeof(int),1,fd);
    }
    run = 0;
    for (j = 0; j < length; j++)
        for (i = 0; i < width; i++) {
            if (run == 0) {
                run = 8*sizeof(long int);
                fread((char *)&mask,sizeof(long int),1,fd);
            }
            if (mask&1)
                lset(i, j, SEEN);
            mask >>= 1;
            run--;
        }
    restore_monsters(fd,Level, version);
    fread((char *)&i,sizeof(int),1,fd);
    fread((char *)&j,sizeof(int),1,fd);
    while (j < length && i < width) {
        Level->site[i][j].things = restore_itemlist(fd, version);
        fread((char *)&i,sizeof(int),1,fd);
        fread((char *)&j,sizeof(int),1,fd);
    }
}


void restore_hiscore_npc(pmt npc, int npcid)
{
    int level, behavior;
    long status;

    switch(npcid) {
    case 0:
        strcpy(Str2,Hiscorer);
        level = Hilevel;
        behavior = Hibehavior;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        strcpy(Str2,Priest[npcid]);
        level = Priestlevel[npcid];
        behavior = Priestbehavior[npcid];
        break;
    case 7:
        strcpy(Str2,Shadowlord);
        level = Shadowlordlevel;
        behavior = Shadowlordbehavior;
        break;
    case 8:
        strcpy(Str2,Commandant);
        level = Commandantlevel;
        behavior = Commandantbehavior;
        break;
    case 9:
        strcpy(Str2,Archmage);
        level = Archmagelevel;
        behavior = Archmagebehavior;
        break;
    case 10:
        strcpy(Str2,Prime);
        level = Primelevel;
        behavior = Primebehavior;
        break;
    case 11:
        strcpy(Str2,Champion);
        level = Championlevel;
        behavior = Championbehavior;
        break;
    case 12:
        strcpy(Str2,Duke);
        level = Dukelevel;
        behavior = Dukebehavior;
        break;
    case 13:
        strcpy(Str2,Chaoslord);
        level = Chaoslordlevel;
        behavior = Chaoslordbehavior;
        break;
    case 14:
        strcpy(Str2,Lawlord);
        level = Lawlordlevel;
        behavior = Lawlordbehavior;
        break;
    case 15:
        strcpy(Str2,Justiciar);
        level = Justiciarlevel;
        behavior = Justiciarbehavior;
        break;
    case 16:
        strcpy(Str2,Grandmaster);
        level = Grandmasterlevel;
        behavior = Grandmasterbehavior;
        break;
    default:
        /* bomb on error */
        level = behavior = 0;
        assert(FALSE);
    }
    npc->monstring = salloc(Str2);
    strcpy(Str1,"The body of ");
    strcat(Str1,Str2);
    npc->corpsestr = salloc(Str1);
    m_status_set( npc, ALLOC );
    if (!m_statusp(npc, HOSTILE)) {
        status = npc->status;
        determine_npc_behavior(npc,level,behavior);
        npc->status = status;
    }
}


void restore_monsters(FILE *fd, plv level, int version)
{
    pml ml=NULL;
    int i,nummonsters;
    char tempstr[80];
    int temp_x, temp_y;
    unsigned char type;

    level->mlist = NULL;

    fread((char *)&nummonsters,sizeof(int),1,fd);

    for(i=0; i<nummonsters; i++) {
        ml = ((pml) checkmalloc(sizeof(mltype)));
        ml->m = ((pmt) checkmalloc(sizeof(montype)));
        ml->next = NULL;
        fread((char *)ml->m,sizeof(montype),1,fd);
        if (ml->m->id == HISCORE_NPC)
            if (version == 80) {
                temp_x = ml->m->x;
                temp_y = ml->m->y;
                make_hiscore_npc(ml->m, ml->m->aux2);
                ml->m->x = temp_x;
                ml->m->y = temp_y;
            }
            else
                restore_hiscore_npc(ml->m, ml->m->aux2);
        else {
            fread((char *)&type,sizeof(unsigned char),1,fd);
            if ( type )
            {
                /* DAG  enforce that if either one of monstring or corpsestr are */
                /*      alloced, both are */
                m_status_set( ml->m, ALLOC );
                if (type&1) {
                    filescanstring(fd,tempstr);
                    ml->m->monstring = salloc(tempstr);
                }
                else
                    ml->m->monstring = salloc( Monsters[ml->m->id].monstring );

                if (type&2) {
                    filescanstring(fd,tempstr);
                    ml->m->corpsestr = salloc(tempstr);
                }
                else
                    ml->m->corpsestr = salloc( Monsters[ml->m->id].corpsestr );
            }
            else
            {
                ml->m->corpsestr = Monsters[ml->m->id].corpsestr;
                ml->m->monstring = Monsters[ml->m->id].monstring;
            }
            /* WDT: As suggested by Sheldon Simms, I'm moving this line... */
            if ( version <= 80 )
                ml->m->possessions = restore_itemlist(fd,version);
            ml->m->meleestr = Monsters[ml->m->id].meleestr;
        }
        /* WDT: ...to here, so that all creatures will have their stuff
         * restored to them.  Savefile versioning added by David Given. */
        if ( version > 80 )
            ml->m->possessions = restore_itemlist(fd,version);
        level->site[ml->m->x][ml->m->y].creature = ml->m;
        ml->next = level->mlist;
        level->mlist = ml;
    }
}


void restore_country(FILE *fd, int version)
{
    int i, j;
    int run;
    unsigned long int mask;

    load_country();
    fread((char *)&i,sizeof(int),1,fd);
    fread((char *)&j,sizeof(int),1,fd);
    while (i < COUNTRY_WIDTH && j < COUNTRY_LENGTH) {
        fread((char *)&Country[i][j],sizeof(struct terrain),1,fd);
        fread((char *)&i,sizeof(int),1,fd);
        fread((char *)&j,sizeof(int),1,fd);
    }
    run = 0;
    for (i = 0; i < COUNTRY_WIDTH; i++)
        for (j = 0; j < COUNTRY_LENGTH; j++) {
            if (run == 0) {
                run = 8*sizeof(long int);
                fread((char *)&mask,sizeof(long int),1,fd);
            }
            if (mask&1)
                c_set(i, j, SEEN);
            mask >>= 1;
            run--;
        }
}
