/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* wiz.cpp */
/* various wizard and omegan adept functions */

#include "glob.h"

/* window for wizard flag list in scr.cpp */
extern WINDOW *Wizlistw;
extern GameState State;

/* various kinds of wishes */
void wish(int blessing)
{
    int i;
    char wishstr[80];
    clearmsg();
    print1("What do you wish for? ");
    if (blessing < 0)
        deathprint();
    else
        strcpy(wishstr,msgscanstring());
    if (blessing < 0 || strcmp(wishstr,"Death")==0) {
        print2("As you wish, so shall it be.");
        p_death("a deathwish");
    }
    if (strcmp(wishstr,"Power")==0) {
        print2("You feel a sudden surge of energy");
        Player.mana=calcmana()*10;
    }
    else if (strcmp(wishstr,"Skill")==0) {
        print2("You feel more competent.");
        if (State.getCheater())
            gain_experience(10000);
        else
            gain_experience(min(10000,Player.xp));
    }
    else if (strcmp(wishstr,"Wealth")==0) {
        print2("You are submerged in shower of gold pieces!");
        Player.cash += 10000;
    }
    else if (strcmp(wishstr,"Balance")==0) {
        print2("You feel neutral.");
        Player.alignment = 0;
    }
    else if (strcmp(wishstr,"Chaos")==0) {
        print2("You feel chaotic.");
        Player.alignment -= 25;
    }
    else if (strcmp(wishstr,"Law")==0) {
        print2("You feel lawful.");
        Player.alignment += 25;
    }
    else if (strcmp(wishstr,"Location")==0)
        strategic_teleport(1);
    else if (strcmp(wishstr,"Knowledge")==0) {
        print2("You feel more knowledgeable.");
        i = random_range(NUMSPELLS);
        if (Spells[i].known)
            Spells[i].powerdrain =
                (max(1,Spells[i].powerdrain/2));
        else Spells[i].known = true;
    }
    else if (strcmp(wishstr,"Health")==0) {
        print2("You feel vigorous");
        Player.hp = max( Player.hp, Player.maxhp);
        Player.status[DISEASED] = 0;
        Player.status[POISONED] = 0;
        Player.food = 43;
        /* Wish for Health when starving does some good. PGM */
    }
    else if (strcmp(wishstr,"Destruction")==0)
        annihilate(State.getCheater());
    else if (strcmp(wishstr,"Acquisition")==0)
        acquire(State.getCheater());
    else if (strcmp(wishstr,"Summoning")==0)
        summon(State.getCheater(),-1);
    else if (strcmp(wishstr,"Stats") == 0 && State.getCheater())
    {
        Player.str = Player.maxstr = Player.con = Player.maxcon =
                                         Player.agi = Player.maxagi = Player.dex = Player.maxdex =
                                                 Player.iq = Player.maxiq = Player.pow = Player.maxpow = 200;
        calc_melee();
    }
    else print2("You feel stupid.");
    dataprint();
    showflags();
}

/* gain for an item */
void acquire(int blessing)
{
    char otype;
    int index,id = ABORT;
    Object* newthing;

    if (blessing < 0) {
        index = random_item();
        if (index == ABORT)
            mprint("You feel fortunate.");
        else {
            print1("Smoke drifts out of your pack.... ");
            print2("Destroyed: ");
            nprint2(itemid(Player.possessions[index]));
            morewait();
            dispose_lost_objects(1,Player.possessions[index]);
        }
    }
    else {
        newthing = ((Object*) checkmalloc(sizeof(Object)));
        /* DAG this assignment looks unneccessary */
        newthing->id = -1;
        if (State.getCheater())
            print1("Acquire which kind of item: !?][}{)/=%%\\& ");
        else
            print1("Acquire which kind of item: !?][}{)/=%%\\ ");
        otype = mgetc();
        switch (otype) {
        case (POTION&0xff):
            if (blessing > 0)
                id = itemlist(POTIONID,NUMPOTIONS);
            else
                id = random_range(NUMPOTIONS);
            if (id < 0) print2("You feel stupid.");
            else make_potion(newthing,id);
            break;
        case (SCROLL&0xff):
            if (blessing > 0)
                id = itemlist(SCROLLID,NUMSCROLLS);
            else
                id = random_range(NUMSCROLLS);
            if (id < 0) print2("You feel stupid.");
            else make_scroll(newthing,id);
            break;
        case (RING&0xff):
            if (blessing > 0)
                id = itemlist(RINGID,NUMRINGS);
            else
                id = random_range(NUMRINGS);
            if (id < 0) print2("You feel stupid.");
            else make_ring(newthing,id);
            break;
        case (STICK&0xff):
            if (blessing > 0)
                id = itemlist(STICKID,NUMSTICKS);
            else
                id = random_range(NUMSTICKS);
            if (id < 0) print2("You feel stupid.");
            else make_stick(newthing,id);
            break;
        case (ARMOR&0xff):
            if (blessing > 0)
                id = itemlist(ARMORID,NUMARMOR);
            else
                id = random_range(NUMARMOR);
            if (id < 0) print2("You feel stupid.");
            else make_armor(newthing,id);
            break;
        case (SHIELD&0xff):
            if (blessing > 0)
                id = itemlist(SHIELDID,NUMSHIELDS);
            else
                id = random_range(NUMSHIELDS);
            if (id < 0) print2("You feel stupid.");
            else make_shield(newthing,id);
            break;
        case (WEAPON&0xff):
            if (blessing > 0)
                id = itemlist(WEAPONID,NUMWEAPONS);
            else
                id = random_range(NUMWEAPONS);
            if (id < 0) print2("You feel stupid.");
            else make_weapon(newthing,id);
            break;
        case (BOOTS&0xff):
            if (blessing > 0)
                id = itemlist(BOOTID,NUMBOOTS);
            else
                id = random_range(NUMBOOTS);
            if (id < 0) print2("You feel stupid.");
            else make_boots(newthing,id);
            break;
        case (CLOAK&0xff):
            if (blessing > 0)
                id = itemlist(CLOAKID,NUMCLOAKS);
            else
                id = random_range(NUMCLOAKS);
            if (id < 0) print2("You feel stupid.");
            else make_cloak(newthing,id);
            break;
        case (FOOD&0xff):
            if (blessing > 0)
                id = itemlist(FOODID,NUMFOODS);
            else
                id = random_range(NUMFOODS);
            if (id < 0) print2("You feel stupid.");
            else make_food(newthing,id);
            break;
        case (THING&0xff):
            if (blessing > 0)
                id = itemlist(THINGID,NUMTHINGS);
            else
                id = random_range(NUMTHINGS);
            if (id < 0) print2("You feel stupid.");
            else make_thing(newthing,id);
            break;
        case (ARTIFACT&0xff):
            if (State.getCheater())
                id = itemlist(ARTIFACTID,NUMARTIFACTS);
            else
                id = -1;
            if (id < 0) print2("You feel stupid.");
            else make_artifact(newthing,id);
            break;
        default:
            print2("You feel stupid.");
        }
        xredraw();
        if (id != ABORT) {
            if (blessing > 0) {
                newthing->known = 2;
                Objects[id].known = 1;
            }
            newthing->used = false;
            gain_item(newthing);
        }
        else
        {
            /* DAG newthing allocated but was not freed... was YA memory leak */
            /* use free() rather than free_obj() since newthing not initialized */
            free( (char *) newthing );
        }
    }
}

/* prints a display of all the configurable flags on the main screen */
void printFlagList( WINDOW  *W )
{
    touchwin(W);
    wclear(W);
    wprintw(W, "a: %d -- Spoke to druid\n", (char*)State.getSpokeToDruid());
    wprintw(W, "b: %d -- Completed goblin caves\n", State.getCompletedCaves() );
    wprintw(W, "c: %d -- Completed sewers\n", State.getCompletedSewers() );
    wprintw(W, "d: %d -- Completed Archmage Castle\n", State.getCompletedCastle() );
    wprintw(W, "e: %d -- Completed Astral Plane\n", State.getCompletedAstral() );
    wprintw(W, "f: %d -- Completed Volcano\n", State.getCompletedVolcano() );
    wprintw(W, "g: %d -- Completed Adept's Challenge\n", State.getCompletedChallenge() );
    wprintw(W, "h: %d -- Killed the Dragonlord\n", State.getKilledDragonlord() );
    wprintw(W, "i: %d -- Killed the Eater of Magic\n", State.getKilledEater() );
    wprintw(W, "j: %d -- Killed the Lawbringer\n", State.getKilledLawbringer() );
    wprintw(W, "k: %d -- Broken the ATM\n", State.getBankBroken() );
    wprintw(W, "l: %d -- Destroyed the Order of Paladins\n", State.getDestroyedOrder() );
    wprintw(W, "m: %d -- Given Stargem to the Lawbringer\n", State.getGivenStargem() );
    wprintw(W, "n: %d -- Attacked the Oracle\n", State.getAttackedOracle() );
    wprintw(W, "o: %d -- Guards are Undead\n", State.getUndeadGuards() );
    wrefresh(W);
    
}

/* lets the user toggle a gamestate flag */
char toggleFlag( void )
{
    char response;
    mprint("Toggle which flag [a-o,ESCAPE]: ");
    do response = (char) mcigetc();
    while (( response <= 'a') && (response >= 'o') && ( response != ESCAPE ));
    switch ( response )
    {
        case 'a':
            State.toggleGameStatus(GameState::SPOKE_TO_DRUID);
            break;
        case 'b':
            State.toggleGameStatus(GameState::COMPLETED_CAVES);
            break;
        case 'c':
            State.toggleGameStatus(GameState::COMPLETED_SEWERS);
            break;
        case 'd':
            State.toggleGameStatus(GameState::COMPLETED_CASTLE);
            break;
        case 'e':
            State.toggleGameStatus(GameState::COMPLETED_ASTRAL);
            break;
        case 'f':
            State.toggleGameStatus(GameState::COMPLETED_VOLCANO);
            break;
        case 'g':
            State.toggleGameStatus(GameState::COMPLETED_CHALLENGE);
            break;
        case 'h':
            State.toggleGameStatus(GameState::KILLED_DRAGONLORD);
            break;
        case 'i':
            State.toggleGameStatus(GameState::KILLED_EATER);
            break;
        case 'j':
            State.toggleGameStatus(GameState::KILLED_LAWBRINGER);
            break;
        case 'k':
            State.toggleGameStatus(GameState::BANK_BROKEN);
            break;
        case 'l':
            State.toggleGameStatus(GameState::DESTROYED_ORDER);
            break;
        case 'm':
            State.toggleGameStatus(GameState::GAVE_STARGEM);
            break;
        case 'n':
            State.toggleGameStatus(GameState::ATTACKED_ORACLE);
            break;
        case 'o':
            State.toggleGameStatus(GameState::UNDEAD_GUARDS);
            break;        
    }
    
    return(response);

}


/* gives the wizard the ability to change GameState flags */
void frobgamestatus(void)
{
    char response;

    WINDOW *W = Wizlistw;
    mprint("List or toggle flag [l,t,ESCAPE]:");
    do response = (char) mcigetc();
    while ((response != 'l') && (response != 't') && (response != ESCAPE));
    if (response == ESCAPE) {
        clearmsg();
    } else {
        if ( response == 'l' )
        {
            while ( response != ESCAPE )
            {
                clearmsg();
                wclear(W);            
                printFlagList(W);
                response = toggleFlag();
            }
            clearmsg();    
        }
        else if ( response == 't' )
        {
            toggleFlag();
            clearmsg();
            mprint("Ok.");            
        }
    }
    
}
