/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* save.c */

#include "glob.h"
#include "LyzzardBucket.h"
#include <string.h>

/*Various functions for doing game saves and restores */
/*The game remembers various player information, the city level,
the country level, and the last or current dungeon level */

/**************** SAVE FUNCTIONS */

/* Checks to see if save file already exists.
   Checks to see if save file can be opened for write.
   The player, the city level, and the current dungeon level are saved.
*/

const char* g_saveFileName = NULL;

int save_game(const char *saveFile)
{
    FILE *fd;
    int i,writeok=true;
    plv current, save;

    char *savestr = strdup(saveFile);
#if !defined(WIN32)
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
            writeok = false;
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
                writeok = false;
            }
            savestr[slashpos] = '/';
        }
    }
#endif
    change_to_user_perms();
    if (writeok) {
        fd = fopen(savestr,"wb");
        if (fd == NULL) {
            writeok = false;
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
            writeok = false;

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
        morewait();
        clearmsg();
    }
    change_to_game_perms();
    return(writeok);
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
    ok &= (fwrite((char *)&State,sizeof(GameState),1,fd) > 0);
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
    ok &= (fwrite((char *)&YogaSessions,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&LastFreeGymDay,sizeof(int),1,fd) > 0);
    ok &= (fwrite((char *)&LastFreeYogaDay,sizeof(int),1,fd) > 0);

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
                mask |= (1L<<(sizeof(long int)*8 - 1));
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


int save_monsters(FILE *fd, MonsterList* ml)
{
    MonsterList* tml;
    int nummonsters=0;
    int ok = 1;
    unsigned char type;

    /* First count monsters */
    for(tml=ml; tml!=NULL; tml=tml->next)
        if (tml->monster->hp > 0) nummonsters++;

    ok &= (fwrite((char *)&nummonsters,sizeof(int),1,fd) > 0);

    /* Now save monsters */
    for(tml=ml; tml!=NULL; tml=tml->next) {
        if (tml->monster->hp > 0) {
            ok &= (fwrite((char *)tml->monster,sizeof(Monster),1,fd) > 0);
            if (tml->monster->id != HISCORE_NPC) {
                type = 0x0;
                /* DAG use pointer compare rather than strcmp; a bit more effecient */
                if (tml->monster->name != Monsters[tml->monster->id].name)
                    type |= 0x1;
                if (tml->monster->corpseString != Monsters[tml->monster->id].corpseString)
                    type |= 0x2;
                /* DAG expect from now (version 9000) on that type will always be  */
                /*     0x3 (both) saved, but leave this way as it may not, and this  */
                /*     preserves save-file compatibility without requiring code changes */
                ok &= (fwrite((char *)&type,sizeof(unsigned char),1,fd) > 0);
                if (type&1)
                    ok &= (fprintf(fd,"%s\n",tml->monster->name) >= 0);
                if (type&2)
                    ok &= (fprintf(fd,"%s\n",tml->monster->corpseString) >= 0);
                /* WDT: line moved from here... */
            }	/* else it'll be reloaded from the hiscore file on restore */
            /* WDT: to here.  This bug fix is Sheldon Simm's suggestion
             * to fix the well-known 'Star Gem' bug; it should allow the
             * possessions of hiscore NPCs to be restored from the savefile.
             * See also the complementary change in restore_monsters. */
            ok &= save_itemlist(fd,tml->monster->possessions);
        }
    }
    return ok;
}

/* Save o unless it's null, then save a special flag byte instead */
/* Use other values of flag byte to indicate what strings are saved */
int save_item(FILE *fd, Object* o)
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
        ok &= (fwrite((char *)o,sizeof(Object),1,fd) > 0);
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
                mask |= (1L<<(sizeof(long int)*8 - 1));
            run--;
        }
    if (run < 8*sizeof(long int))
        ok &= (fwrite((char *)&mask,sizeof(long int),1,fd) > 0);
    return ok;
}


/* returns true if the given version can be restored by this version */
int ok_outdated(int version)
{
    /* currently all backwards compatibility is broken, so fail ok_outdated()
       until we actually re-install some type of backwards compatibility. DAG */

    return false;
}

/* read player data, city level, dungeon level,
   check on validity of save file, etc.
   return true if game restored, false otherwise */

int restore_game(const char *savestr)
{
    int i,version;
    FILE *fd;

#if !defined(WIN32)
    if (access(savestr, F_OK|R_OK|W_OK) == -1) /* access uses real uid */
    {
        print1("Unable to access save file: ");
        nprint1(savestr);
        morewait();
        return false;
    }
#endif
    change_to_user_perms();

    fd = fopen(savestr,"rb");

    if (fd == NULL) {
        print1("Error restoring game -- aborted.");
        print2("File name was: ");
        nprint2(savestr);
        morewait();
        change_to_game_perms();
        return(false);
    }
    else {
        print1("Restoring...");

        fread((char *)&version,sizeof(int),1,fd);

        if (VERSION != version && !ok_outdated(version)) {
            change_to_game_perms();
            fclose(fd);
            clearmsg();
            mprint(" Sorry, I can't restore an outdated save file!");
            mprint(" Game version is ");
            mnumprint(VERSION/10000);
			nprint2(".");
			mnumprint( (version/100)%100);
            mprint(" Save file version is ");
            mnumprint(version/10000);
            nprint2(".");
            mnumprint( (version/100)%100);
            morewait();
            return(false);
        }
        restore_player(fd, version);
        restore_country(fd, version);
        restore_level(fd, version); /* the city level */
        fread((char *)&i,sizeof(int),1,fd);
        for (; i > 0; i--) {
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
        State.setSkipMonsters();
        change_to_game_perms();
        return(true);
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

	fread((char *)CitySiteList,sizeof(CitySiteList),1,fd);
    fread((char *)&State,sizeof(GameState),1,fd);
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
    fread((char *)&YogaSessions,sizeof(int),1,fd);
    fread((char *)&LastFreeGymDay,sizeof(int),1,fd);
    fread((char *)&LastFreeYogaDay,sizeof(int),1,fd);
	
    // Read bank information
    fread( (char *)&num_accounts, sizeof( int ), 1, fd );
    for( i = 0; i < num_accounts; i++ )
    {
        account = (bank_account *)checkmalloc( sizeof( bank_account ));
        fread( (char *)(&(account->player)), sizeof(short), 1, fd );
        fread( (char *)(&(account->balance)), sizeof(long), 1, fd );
        fread( (char *)(&(account->number)), sizeof(long), 1, fd );
        filescanstring( fd, pw_buf );
        account->password = (char *)salloc( pw_buf );
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
    inititem(false);

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
Object* restore_item(FILE *fd, int version)
{
    char tempstr[80];
    unsigned char type;
    Object* obj = NULL;

    fread((char *)&type,sizeof(type),1,fd);
    if (type != 0xff) {
        obj = ((Object*) checkmalloc(sizeof(Object)));
        fread((char *)obj,sizeof(Object),1,fd);

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
        if(obj->id == FOODID+0) {
            Object *old = obj;
            obj = new LyzzardBucket(obj);
            free(old);
        }
    }
    return obj;
}

pol restore_itemlist(FILE *fd, int version)
{
    pol ol=NULL,cur=NULL,newObject=NULL;
    int i,numitems,firsttime=true;
    fread((char *)&numitems,sizeof(int),1,fd);
    for(i=0; i<numitems; i++) {
        newObject = ((pol) checkmalloc(sizeof(oltype)));
        newObject->thing = restore_item(fd, version);
        newObject->next = NULL;
        if (firsttime > 0) {
            ol = cur = newObject;
            firsttime = false;
        }
        else {
            cur->next = newObject;
            cur = newObject;
        }
    }
    return(ol);
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
    Level->generated = true;
    temp_env = Current_Environment;
    Current_Environment = Level->environment;
	Village village;
    switch(Level->environment) {
    case E_COUNTRYSIDE:
        load_country();
        break;
    case E_CITY:
        load_city(false);
        break;
    case E_VILLAGE:
        village.load_village(Country[LastCountryLocX][LastCountryLocY].aux, false);
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
        load_house(Level->environment, 0);
        break;
    case E_DLAIR:
        load_dlair(State.getKilledDragonlord(), 0);
        break;
    case E_STARPEAK:
        load_speak(State.getKilledLawbringer(), 0);
        break;
    case E_MAGIC_ISLE:
        load_misle(State.getKilledEater(), 0);
        break;
    case E_TEMPLE:
        load_temple(Country[LastCountryLocX][LastCountryLocY].aux, false);
        break;
    case E_CIRCLE:
        load_circle(false);
        break;
    case E_COURT:
        load_court(false);
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


void restore_hiscore_npc(Monster* npc, int npcid)
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
        assert(false);
    }
    npc->name = salloc(Str2);
    strcpy(Str1,"The body of ");
    strcat(Str1,Str2);
    npc->corpseString = salloc(Str1);
    m_status_set( npc, ALLOC );
    if (!m_statusp(npc, HOSTILE)) {
        status = npc->status;
        npc->determine_npc_behavior(level,behavior);
        npc->status = status;
    }
}

void restore_monsters(FILE *fd, plv level, int version)
{
    MonsterList* ml=NULL;
    int i,nummonsters;
    char tempstr[80];
    int temp_x, temp_y;
    unsigned char type;

    level->mlist = NULL;

    fread((char *)&nummonsters,sizeof(int),1,fd);

    for(i=0; i<nummonsters; i++) {
        ml = ((MonsterList*) checkmalloc(sizeof(MonsterList)));
        ml->monster = ((Monster*) checkmalloc(sizeof(Monster)));
        ml->next = NULL;
        fread((char *)ml->monster,sizeof(Monster),1,fd);
        if (ml->monster->id == HISCORE_NPC)
            if (version == 80) {
                temp_x = ml->monster->x;
                temp_y = ml->monster->y;
                make_hiscore_npc(ml->monster, ml->monster->aux2);
                ml->monster->x = temp_x;
                ml->monster->y = temp_y;
            }
            else
                restore_hiscore_npc(ml->monster, ml->monster->aux2);
        else {
            fread((char *)&type,sizeof(unsigned char),1,fd);
            if ( type )
            {
                /* DAG  enforce that if either one of name or corpseString are */
                /*      alloced, both are */
                m_status_set( ml->monster, ALLOC );
                if (type&1) {
                    filescanstring(fd,tempstr);
                    ml->monster->name = salloc(tempstr);
                }
                else
                    ml->monster->name = salloc( Monsters[ml->monster->id].name );

                if (type&2) {
                    filescanstring(fd,tempstr);
                    ml->monster->corpseString = salloc(tempstr);
                }
                else
                    ml->monster->corpseString = salloc( Monsters[ml->monster->id].corpseString );
            }
            else
            {
                ml->monster->corpseString = Monsters[ml->monster->id].corpseString;
                ml->monster->name = Monsters[ml->monster->id].name;
            }
            /* WDT: As suggested by Sheldon Simms, I'm moving this line... */
            if ( version <= 80 )
                ml->monster->possessions = restore_itemlist(fd,version);
            ml->monster->combatManeuvers = Monsters[ml->monster->id].combatManeuvers;
        }
        /* WDT: ...to here, so that all creatures will have their stuff
         * restored to them.  Savefile versioning added by David Given. */
        if ( version > 80 )
            ml->monster->possessions = restore_itemlist(fd,version);
        level->site[ml->monster->x][ml->monster->y].creature = ml->monster;
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
