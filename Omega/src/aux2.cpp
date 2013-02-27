/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* aux2.c */
/* some functions called by ocom.c, also see aux1.c and aux3.c*/
/* This is a real grab bag file. It contains functions used by
aux1.c and omega.c, as well as elsewhere. It is mainly here so aux1.c
and aux3.c are not huge */

#include "glob.h"

/* Player stats like str, agi, etc give modifications to various abilities
chances to do things, etc. Positive is good, negative bad. */
int statmod(int stat)
{
    return((stat-10)/2);
}

/* effects of hitting */
void p_hit (Monster *m,int dmg,int dtype)
{
    int dmult = 0;

    /* chance for critical hit..., 3/10 */
    switch (random_range(10)) {
    case 0:
        if (random_range(100) < (Player.level
                                 + Player.rank[MONKS]
                                )) {
            strcpy(Str3,"You annihilate ");
            dmult = 1000;
        }
        else {
            strcpy(Str3,"You blast ");
            dmult=5;
        }
        break;
    case 1:
    case 2:
        strcpy(Str3,"You smash ");
        dmult=2;
        break;

    default:
        dmult=1;
        if (random_range(10)) strcpy(Str3,"You hit ");
        else switch(random_range(12)) {
            case 0:
                strcpy(Str3,"You damage ");
                break;
            case 1:
                strcpy(Str3,"You inflict bodily harm upon ");
                break;
            case 2:
                strcpy(Str3,"You injure ");
                break;
            case 3:
                strcpy(Str3,"You molest ");
                break;
            case 4:
                strcpy(Str3,"You tweak ");
                break;
            case 5:
                strcpy(Str3,"You smush ");
                break;
            case 6:
                strcpy(Str3,"You smurf ");
                break;
            case 7:
                strcpy(Str3,"You grind ");
                break;
            case 8:
                strcpy(Str3,"You hurt ");
                break;
            case 9:
                strcpy(Str3,"You bring pain to ");
                break;
            case 10:
                strcpy(Str3,"You recite nasty poetry at ");
                break;
            case 11:
                strcpy(Str3,"You smack ");
                break;
            }
        break;
    }

    if (Lunarity == 1) dmult = dmult * 2;
    else if (Lunarity == -1) dmult = dmult / 2;
    if (m->uniqueness == COMMON) strcat(Str3,"the ");
    strcat(Str3,m->name);
    strcat(Str3,". ");
    if (Verbosity != TERSE) mprint(Str3);
    else mprint("You hit it.");

    if (Player.possessions[O_WEAPON_HAND] == NULL) { /*barehanded*/
        if (Player.rank[MONKS] > MONK_MASTER_SIGHS) {
            /* high level monks do unstoppable hand damage */
            dtype = UNSTOPPABLE;
        }
    }

    m->m_damage(dmult * random_range(dmg),dtype);
    if ((Verbosity != TERSE) && (random_range(10)==3) && (m->hp > 0))
        mprint("It laughs at the injury and fights on!");
}

/* and effects of missing */
void player_miss(Monster *m,int dtype)
{
    if (random_range(30)==1) /* fumble 1 in 30 */
        p_fumble(dtype);
    else {
        if (Verbosity != TERSE) {
            if (random_range(10))
                strcpy(Str3,"You miss ");
            else switch(random_range(4)) {
                case 0:
                    strcpy(Str3,"You flail lamely at ");
                    break;
                case 1:
                    strcpy(Str3,"You only amuse ");
                    break;
                case 2:
                    strcpy(Str3,"You fail to even come close to ");
                    break;
                case 3:
                    strcpy(Str3,"You totally avoid contact with ");
                    break;
                }
            if (m->uniqueness == COMMON) strcat(Str3,"the ");
            strcat(Str3,m->name);
            strcat(Str3,". ");
            mprint(Str3);
        }
        else mprint("You missed it.");
    }
}

/* oh nooooo, a fumble.... */
void p_fumble(int dtype)
{
    mprint("Ooops! You fumbled....");
    switch(random_range(10)) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        drop_weapon();
        break;
    case 6:
    case 7:
    case 8:
        break_weapon();
        break;
    case 9:
        mprint("Oh No! You hit yourself!");
        p_damage(Player.dmg,dtype,"stupidity");
        break;
    }
}

/* try to drop a weapon (from fumbling) */
void drop_weapon(void)
{
    if (Player.possessions[O_WEAPON_HAND] != NULL) {
        strcpy(Str1,"You dropped your ");
        strcat(Str1,Player.possessions[O_WEAPON_HAND]->objstr);
        mprint(Str1);
        morewait();
        p_drop_at(Player.x,Player.y,1,Player.possessions[O_WEAPON_HAND]);
        conform_lost_objects(1,Player.possessions[O_WEAPON_HAND]);
    }
    else mprint("You feel fortunate.");
}


/* try to break a weapon (from fumbling) */
void break_weapon(void)
{
    if (Player.possessions[O_WEAPON_HAND] != NULL) {
        strcpy(Str1,"Your ");
        strcat(Str1,itemid(Player.possessions[O_WEAPON_HAND]));
        strcat(Str1," vibrates in your hand....");
        mprint(Str1);
        (void) damage_item(Player.possessions[O_WEAPON_HAND]);
        morewait();
    }
}


/* hooray */
void p_win(void)
{
    morewait();
    clearmsg();
    print1("You won!");
    morewait();
    display_win();
    endgraf();
    exit(0);
}


/* handle a h,j,k,l, etc., to change x and y by dx and dy */
/* for targeting in dungeon */
void movecursor(int *x, int *y, int dx, int dy)
{
    if (inbounds(*x+dx,*y+dy)) {
        *x += dx;
        *y += dy;
        screencheck(*x,*y);
    }
    omshowcursor(*x,*y);
}


/* is Player immune to damage type dtype */
int p_immune(int dtype)
{
    return(Player.immunity[dtype]>0);
}


/* deal with each possible stati -- values are per move */
/* this function is executed every move */
/* A value over 1000 indicates a permanent effect */
void minute_status_check(void)
{
    int i;

    if (Player.status[HASTED]>0) {
        if (Player.status[HASTED] < 1000) {
            Player.status[HASTED]--;
            if (Player.status[HASTED]==0) {
                mprint("The world speeds up.");
                calc_melee();
            }
        }
    }


    if (Player.status[POISONED]>0) {
        Player.status[POISONED]--;
        p_damage(3,POISON,"poison");
        if (Player.status[POISONED] == 0) {
            showflags();
            mprint("You feel better now.");
        }
    }


    if (Player.immunity[UNSTOPPABLE]>0) {
        for(i=0; i<NUMIMMUNITIES; i++)
            Player.immunity[i]--;
        if (Player.immunity[UNSTOPPABLE]==1)
            mprint("You feel vincible again.");
    }


    if (Player.status[IMMOBILE]>0) {
        Player.status[IMMOBILE]--;
        if (Player.status[IMMOBILE] == 0)
            mprint("You can move again.");
    }


    if (Player.status[SLEPT]>0) {
        Player.status[SLEPT]--;
        if (Player.status[SLEPT] == 0) {
            mprint("You woke up.");
        }
    }

    if (Player.status[REGENERATING]>0) {
        if ((Player.hp < Player.maxhp) && (Player.mana > 0)) {
            Player.hp++;
            Player.mana--;
            dataprint();
        }
        if (Player.status[REGENERATING] < 1000) {
            Player.status[REGENERATING]--;
            if (Player.status[REGENERATING] == 0) {
                mprint("You feel less homeostatic.");
            }
        }
    }

    if (Player.status[SLOWED]>0) {
        if (Player.status[SLOWED] < 1000) {
            Player.status[SLOWED]--;
            if (Player.status[SLOWED] == 0) {
                mprint("You feel quicker now.");
                calc_melee();
            }
        }
    }

    if (Player.status[RETURNING]>0) {
        Player.status[RETURNING]--;
        if (Player.status[RETURNING] == 10)
            mprint("Your return spell slowly hums towards activation...");
        else if (Player.status[RETURNING] == 8)
            mprint("There is an electric tension in the air!");
        else if (Player.status[RETURNING] == 5)
            mprint("A vortex of mana begins to form around you!");
        else if (Player.status[RETURNING] == 1)
            mprint("Your surroundings start to warp and fade!");
        if (Player.status[RETURNING] == 0)
            level_return();
    }

    if (Player.status[AFRAID]>0) {
        if (Player.status[AFRAID] < 1000) {
            Player.status[AFRAID]--;
            if (Player.status[AFRAID] == 0) {
                mprint("You feel bolder now.");
            }
        }
    }

}



/* effect of gamma ray radiation... */
void moon_check(void)
{
    /* 24 day lunar cycle */
    Phase = (Phase+1)%24;
    phaseprint();
    Lunarity = 0;
    if (((Player.patron == DRUID) && ((Phase/2 == 3) || (Phase/2 == 9))) ||
            ((Player.alignment > 10) && (Phase/2 == 6)) ||
            ((Player.alignment < -10) && (Phase/2 == 0))) {
        mprint("As the moon rises you feel unusually vital!");
        Lunarity = 1;
    }
    else if (((Player.patron == DRUID) && ((Phase/2 == 0) || (Phase/2 == 6))) ||
             ((Player.alignment > 10) && (Phase/2 == 0)) ||
             ((Player.alignment < -10) && (Phase/2 == 6))) {
        mprint("The rise of the moon tokens a strange enervation!");
        Lunarity = -1;
    }

}



/* check 1/hour for torch to burn out if used */
void torch_check(void)
{
    int i;
    for(i=O_READY_HAND; i<=O_WEAPON_HAND; i++) {
        if (Player.possessions[i]!=NULL)
            if ((Player.possessions[i]->id == OB_TORCH) && /*torch */
                    (Player.possessions[i]->aux > 0)) {
                Player.possessions[i]->aux--;
                if (Player.possessions[i]->aux==0) {
                    mprint("Your torch goes out!!!");
                    conform_unused_object(Player.possessions[i]);
                    if (Player.possessions[i]->number > 1) {
                        Player.possessions[i]->number--;
                        Player.possessions[i]->aux = 6;
                    }
                    else {
                        Player.possessions[i]->usef = I_NO_OP;
                        Player.possessions[i]->cursestr =
                            Player.possessions[i]->truename =
                                Player.possessions[i]->objstr = "burnt-out torch";
                    }
                }
            }
    }
}



/* values are in multiples of ten minutes */
/* values over 1000 indicate a permanent effect */
void tenminute_status_check(void)
{
    if ((Player.status[SHADOWFORM]>0) && (Player.status[SHADOWFORM]<1000)) {
        Player.status[SHADOWFORM]--;
        if (Player.status[SHADOWFORM] == 0) {
            Player.immunity[NORMAL_DAMAGE]--;
            Player.immunity[ACID]--;
            Player.immunity[THEFT]--;
            Player.immunity[INFECTION]--;
            mprint("You feel less shadowy now.");
        }
    }

    if ((Player.status[ILLUMINATION]>0) && (Player.status[ILLUMINATION]<1000)) {
        Player.status[ILLUMINATION]--;
        if (Player.status[ILLUMINATION] == 0) {
            mprint("Your light goes out!");
        }
    }


    if ((Player.status[VULNERABLE]>0) && (Player.status[VULNERABLE]<1000)) {
        Player.status[VULNERABLE]--;
        if (Player.status[VULNERABLE] == 0)
            mprint("You feel less endangered.");
    }


    if ((Player.status[DEFLECTION]>0) && (Player.status[DEFLECTION]<1000)) {
        Player.status[DEFLECTION]--;
        if (Player.status[DEFLECTION] == 0)
            mprint("You feel less well defended.");
    }

    if ((Player.status[ACCURATE]>0) && (Player.status[ACCURACY]<1000)) {
        Player.status[ACCURATE]--;
        if (Player.status[ACCURATE] == 0) {
            calc_melee();
            mprint("The bulls' eyes go away.");
        }
    }
    if ((Player.status[HERO]>0) && (Player.status[HERO]<1000)) {
        Player.status[HERO]--;
        if (Player.status[HERO] == 0) {
            calc_melee();
            mprint("You feel less than super.");
        }
    }

    if ((Player.status[LEVITATING]>0) && (Player.status[LEVITATING]<1000)) {
        Player.status[LEVITATING]--;
        if (Player.status[LEVITATING] == 0)
            mprint("You're no longer walking on air.");
    }

    if (Player.status[DISEASED]>0) {
        Player.status[DISEASED]--;
        if (Player.status[DISEASED] == 0) {
            showflags();
            mprint("You feel better now.");
        }
    }


    if ((Player.status[INVISIBLE] > 0) && (Player.status[INVISIBLE]<1000)) {
        Player.status[INVISIBLE]--;
        if (Player.status[INVISIBLE] == 0)
            mprint("You feel more opaque now.");
    }

    if ((Player.status[BLINDED]>0) && (Player.status[BLINDED]<1000)) {
        Player.status[BLINDED]--;
        if (Player.status[BLINDED] == 0)
            mprint("You can see again.");
    }

    if ((Player.status[TRUESIGHT]>0) && (Player.status[TRUESIGHT]<1000)) {
        Player.status[TRUESIGHT]--;
        if (Player.status[TRUESIGHT] == 0)
            mprint("You feel less keen now.");
    }

    if ((Player.status[BERSERK]>0) && (Player.status[BERSERK]<1000)) {
        Player.status[BERSERK]--;
        if (Player.status[BERSERK] == 0)
            mprint("You stop foaming at the mouth.");
    }

    if ((Player.status[ALERT]>0) && (Player.status[ALERT] < 1000)) {
        Player.status[ALERT]--;
        if (Player.status[ALERT] == 0)
            mprint("You feel less alert now.");
    }

    if ((Player.status[BREATHING]>0) && (Player.status[BREATHING] < 1000)) {
        Player.status[BREATHING]--;
        if (Player.status[BREATHING] == 0)
            mprint("You feel somewhat congested.");
    }

    if ((Player.status[DISPLACED]>0) && (Player.status[DISPLACED] < 1000)) {
        Player.status[DISPLACED]--;
        if (Player.status[DISPLACED]==0)
            mprint("You feel a sense of position.");
    }
    timeprint();
    dataprint();
}



/* Increase in level at appropriate experience gain */
void gain_level(void)
{
    int gained=false;
    int hp_gain; /* FIXED! 12/30/98 */

    if (gamestatusp(SUPPRESS_PRINTING))
        return;
    while (expval(Player.level+1) <= Player.xp) {
        if (!gained)
            morewait();
        gained = true;
        Player.level++;
        print1("You have attained a new experience level!");
        print2("You are now ");
        nprint2(getarticle(levelname(Player.level)));
        nprint2(levelname(Player.level));
        hp_gain = random_range(Player.con)+1; /* start fix 12/30/98 */
        if (Player.hp < Player.maxhp )
            Player.hp += hp_gain*Player.hp/Player.maxhp;
        else if (Player.hp < Player.maxhp + hp_gain)
            Player.hp = Player.maxhp + hp_gain;
        /* else leave current hp alone */
        Player.maxhp += hp_gain;
        Player.maxmana = calcmana();
        /* If the character was given a bonus, let him keep it.  Otherwise
        * recharge him. */
        Player.mana = max(Player.mana, Player.maxmana); /* end fix 12/30/98 */
        morewait();
    }
    if (gained) clearmsg();
    calc_melee();
}

/* experience requirements */
long expval(int plevel)
{
    switch(plevel) {
    case 0:
        return(0L);
    case 1:
        return(20L);
    case 2:
        return(50L);
    case 3:
        return(200L);
    case 4:
        return(500L);
    case 5:
        return(1000L);
    case 6:
        return(2000L);
    case 7:
        return(3000L);
    case 8:
        return(5000L);
    case 9:
        return(7000L);
    case 10:
        return(10000L);
    default:
        if (plevel < 20)
            return((plevel-9) * 10000L);
        else
            return((plevel-9) * 10000L +
                   (plevel-19)*(plevel-19)*500);
    }
}

/* If an item is unidentified, it isn't worth much to those who would buy it */
long item_value(pob item)
{
    if (item->known == 0) {
        if (item->objchar == THING) return(1);
        else return(true_item_value(item) / 10);
    }
    else if (item->known == 1) {
        if (item->objchar == THING) return(item->basevalue);
        else return(item->basevalue / 2);
    }
    else return(true_item_value(item));
}


/* figures value based on item base-value, charge, plus, and blessing */
long true_item_value(pob item)
{
    long value = item->basevalue;

    if (item->objchar == THING) return(item->basevalue);
    else {
        if (item->objchar == STICK) value += value*item->charge/20;
        if (item->plus > -1) value += value*item->plus/4;
        else value /= -item->plus;
        if (item->blessing > 0) value *= 2;
        return((long) value);
    }
}

/* kill off player if he isn't got the "breathing" status */
void p_drown(void)
{
    int attempts = 3, i;

    if (Player.status[BREATHING] > 0)
        mprint("Your breathing is unaffected!");
    else while (Player.possessions[O_ARMOR] ||
                    Player.itemweight > ((int) (Player.maxweight / 2))) {
            menuclear();
            switch (attempts--) {
            case 3:
                print3("You try to hold your breath...");
                break;
            case 2:
                print3("You try to hold your breath... You choke...");
                break;
            case 1:
                print3("You try to hold your breath... You choke... Your lungs fill...");
                break;
            case 0:
                p_death("drowning");
            }
            morewait();
            menuprint("a: Drop an item.\n");
            menuprint("b: Bash an item.\n");
            menuprint("c: Drop your whole pack.\n");
            showmenu();
            switch(menugetc()) {
            case 'a':
                drop();
                if (Level->site[Player.x][Player.y].p_locf == L_WATER && Level->site[Player.x][Player.y].things)
                {
                    mprint("It sinks without a trace.");
                    free_objlist(Level->site[Player.x][Player.y].things);
                    Level->site[Player.x][Player.y].things = NULL;
                }
                break;
            case 'b':
                bash_item();
                break;
            case 'c':
                setgamestatus(SUPPRESS_PRINTING);
                for(i=0; i<MAXPACK; i++) {
                    if (Player.pack[i] != NULL) {
                        if (Level->site[Player.x][Player.y].p_locf != L_WATER)
                        {
                            p_drop_at(Player.x,Player.y,Player.pack[i]->number,Player.pack[i]);
                            free_obj( Player.pack[i], true );
                        } else
                            free_obj( Player.pack[i], true );
                    }
                    Player.pack[i] = NULL;
                }
                if (Level->site[Player.x][Player.y].p_locf == L_WATER)
                    mprint("It sinks without a trace.");
                Player.packptr = 0;
                resetgamestatus(SUPPRESS_PRINTING);
                calc_melee();
                break;
            }
        }
    show_screen();
    return;
}


/* the effect of some weapon on monster m, with dmgmod a bonus to damage */
void weapon_use(int dmgmod, pob weapon, Monster *m)
{
    int aux = (weapon==NULL ? -2 : weapon->aux); /* bare hands */
    switch(aux) {
    case -2:
        weapon_bare_hands(dmgmod,m);
        break;
    default:
    case I_NO_OP:
        weapon_normal_hit(dmgmod,weapon,m);
        break;
    case I_ACIDWHIP:
        weapon_acidwhip(dmgmod,weapon,m);
        break;
    case I_TANGLE:
        weapon_tangle(dmgmod,weapon,m);
        break;
    case I_ARROW:
        weapon_arrow(dmgmod,weapon,m);
        break;
    case I_BOLT:
        weapon_bolt(dmgmod,weapon,m);
        break;
    case I_DEMONBLADE:
        weapon_demonblade(dmgmod,weapon,m);
        break;
    case I_LIGHTSABRE:
        weapon_lightsabre(dmgmod,weapon,m);
        break;
    case I_MACE_DISRUPT:
        weapon_mace_disrupt(dmgmod,weapon,m);
        break;
    case I_VORPAL:
        weapon_vorpal(dmgmod,weapon,m);
        break;
    case I_DESECRATE:
        weapon_desecrate(dmgmod,weapon,m);
        break;
    case I_FIRESTAR:
        weapon_firestar(dmgmod,weapon,m);
        break;
    case I_DEFEND:
        weapon_defend(dmgmod,weapon,m);
        break;
    case I_VICTRIX:
        weapon_victrix(dmgmod,weapon,m);
        break;
    case I_SCYTHE:
        weapon_scythe(dmgmod,weapon,m);
        break;
    }
}


/* for printing actions in printactions above */
char *actionlocstr(char dir)
{
    switch(dir) {
    case 'L':
        strcpy(Str3,"low.");
        break;
    case 'C':
        strcpy(Str3,"center.");
        break;
    case 'H':
        strcpy(Str3,"high.");
        break;
    default:
        strcpy(Str3,"wildly.");
        break;
    }
    return(Str3);
}


/* execute player combat actions versus monster m */
void tacplayer(Monster *m)
{
    int i=0;

    while (i < strlen(Player.combatManeuvers)) {
        if (m->hp > 0) {
            switch(Player.combatManeuvers[i]) {
            case 't':
            case 'T':
                if (Player.possessions[O_WEAPON_HAND] == NULL)
                    strcpy(Str1,"You punch ");
                else strcpy(Str1,"You thrust ");
                strcat(Str1,actionlocstr(Player.combatManeuvers[i+1]));
                if (Verbosity == VERBOSE) mprint(Str1);
                if (m->player_hit(2*statmod(Player.dex),Player.combatManeuvers[i+1]))
                    weapon_use(0,Player.possessions[O_WEAPON_HAND],m);
                else player_miss(m,NORMAL_DAMAGE);
                break;
            case 'c':
            case 'C':
                if (Player.possessions[O_WEAPON_HAND] == NULL)
                    strcpy(Str1,"You punch ");
                else if (Player.possessions[O_WEAPON_HAND]->type == CUTTING)
                    strcpy(Str1,"You cut ");
                else if (Player.possessions[O_WEAPON_HAND]->type == STRIKING)
                    strcpy(Str1,"You strike ");
                else strcpy(Str1,"You attack ");
                strcat(Str1,actionlocstr(Player.combatManeuvers[i+1]));
                if (Verbosity == VERBOSE) mprint(Str1);
                if (m->player_hit(0,Player.combatManeuvers[i+1]))
                    weapon_use(2*statmod(Player.str),
                               Player.possessions[O_WEAPON_HAND],
                               m);
                else player_miss(m,NORMAL_DAMAGE);
                break;
            case 'l':
            case 'L':
                strcpy(Str1,"You lunge ");
                strcat(Str1,actionlocstr(Player.combatManeuvers[i+1]));
                if (Verbosity == VERBOSE) mprint(Str1);
                if (m->player_hit(Player.level+Player.dex,Player.combatManeuvers[i+1]))
                    weapon_use(Player.level,Player.possessions[O_WEAPON_HAND],m);
                else player_miss(m,NORMAL_DAMAGE);
                break;
            }
        }
        i+=2;
    }
}

/* This function is used to undo all items temporarily, should
always be used in pairs with on being true and false, and may cause
anomalous stats and item-usage if used indiscriminately */

void toggle_item_use(int on)
{
    static int used[MAXITEMS];
    int i;
    setgamestatus(SUPPRESS_PRINTING);
    if (on)
        for(i=0; i<MAXITEMS; i++) {
            used[i] = false;
            if (Player.possessions[i] != NULL) {
                if ((used[i] = Player.possessions[i]->used) == true) {
                    Player.possessions[i]->used = false;
                    item_use(Player.possessions[i]);
                }
            }
        }
    else {
        for(i=1; i<MAXITEMS; i++)
            if (used[i]) {
                Player.possessions[i]->used = true;
                item_use(Player.possessions[i]);
            }
        calc_melee();
        showflags();
        dataprint();
        timeprint();
    }
    resetgamestatus(SUPPRESS_PRINTING);
}


void enter_site(Symbol site)
{
    switch(site) {
    case CITY:
        change_environment(E_CITY);
        break;
    case VILLAGE:
        change_environment(E_VILLAGE);
        break;
    case CAVES:
        change_environment(E_CAVES);
        break;
    case CASTLE:
        change_environment(E_CASTLE);
        break;
    case VOLCANO:
        change_environment(E_VOLCANO);
        break;
    case TEMPLE:
        change_environment(E_TEMPLE);
        break;
    case DRAGONLAIR:
        change_environment(E_DLAIR);
        break;
    case STARPEAK:
        change_environment(E_STARPEAK);
        break;
    case MAGIC_ISLE:
        change_environment(E_MAGIC_ISLE);
        break;
    case PALACE:
        change_environment(E_PALACE);
        break;
    default:
        print3("There's nothing to enter here!");
        break;
    }
}



/* Switches context dungeon/countryside/city, etc */
void change_environment(char new_environment)
{
    int i, emerging = false;

    Player.sx = -1;
    Player.sy = -1; /* reset sanctuary if there was one */

    /* missing message if gets lost on site... */
    if (gamestatusp(LOST))
    {
        resetgamestatus(LOST);	/* in case the player gets lost _on_ a site */
        mprint("You know where you are now."); /* but didn't inform player... DAG */
    }

    resetgamestatus(FAST_MOVE);

    Last_Environment = Current_Environment;
    if (Last_Environment == E_COUNTRYSIDE)
    {
        LastCountryLocX = Player.x;
        LastCountryLocY = Player.y;
    }

    if (((Last_Environment == E_CITY) ||
            (Last_Environment == E_VILLAGE)) &&
            ((new_environment == E_MANSION) ||
             (new_environment == E_HOUSE) ||
             (new_environment == E_HOVEL) ||
             (new_environment == E_SEWERS) ||
             (new_environment == E_ARENA)))
    {
        LastTownLocX = Player.x;
        LastTownLocY = Player.y;
    }

    else if (((Last_Environment == E_MANSION) ||
              (Last_Environment == E_HOUSE) ||
              (Last_Environment == E_HOVEL) ||
              (Last_Environment == E_SEWERS) ||
              (Last_Environment == E_ARENA)) &&
             ((new_environment == E_CITY) ||
              (new_environment == E_VILLAGE)))
    {
        setPlayerXY( LastTownLocX, LastTownLocY);
        emerging = true;
    }

    Current_Environment = new_environment;
    switch (new_environment)
    {
    case E_ARENA:
        setPlayerXY(5, 7);
        setgamestatus(ARENA_MODE);
        load_arena();
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_ABYSS:
        setPlayerXY(32, 15);
        load_abyss();
        abyss_file();
        lose_all_items();
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_CIRCLE:
        setPlayerXY(32, 14);
        load_circle(true);
        if (Objects[OB_STARGEM].uniqueness == UNIQUE_TAKEN)
        {
            print1("A bemused voice says:");
            print2("'Why are you here? You already have the Star Gem!'");
            morewait();
        }
        else if (Player.rank[CIRCLE] > 0)
        {
            print1("You hear the voice of the Prime Sorceror:");
            print2("'Congratulations on your attainment of the Circle's Demesne.'");
            morewait();
            print1("For the honor of the Circle, you may take the Star Gem");
            print2("and destroy it on the acme of Star Peak.");
            morewait();
            print1("Beware the foul LawBringer who resides there...");
            print2("By the way, some of the members of the Circle seem to");
            morewait();
            print1("have become a bit jealous of your success --");
            print2("I'd watch out for them too if I were you.");
            morewait();
        }
        else if (Player.alignment > 0)
        {
            print1("A mysterious ghostly image materializes in front of you.");
            print2("It speaks: 'Greetings, fellow abider in Law. I am called");
            morewait();
            print1("The LawBringer. If you wish to advance our cause, obtain");
            print2("the mystic Star Gem and return it to me on Star Peak.");
            morewait();
            print1("Beware the power of the evil Circle of Sorcerors and the");
            print2("forces of Chaos which guard the gem.'");
            morewait();
            print1("The strange form fades slowly.");
            morewait();
        }
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_COURT:
        setPlayerXY(32, 2);
        LastCountryLocX = 6;
        LastCountryLocY = 1;
        load_court(true);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_MANSION:
        load_house(E_MANSION, true);
        setPlayerXY(2, 8);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_HOUSE:
        load_house(E_HOUSE, true);
        setPlayerXY(2, 13);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_HOVEL:
        load_house(E_HOVEL, true);
        setPlayerXY(2, 9);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_DLAIR:
        setPlayerXY(0, 8);
        load_dlair(gamestatusp(KILLED_DRAGONLORD), true);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_STARPEAK:
        setPlayerXY(2, 9);
        load_speak(gamestatusp(KILLED_LAWBRINGER), true);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_MAGIC_ISLE:
        setPlayerXY(62, 14);
        load_misle(gamestatusp(KILLED_EATER), true);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_TEMPLE:
        load_temple(Country[Player.x][Player.y].aux, true);
        setPlayerXY(32, 15);
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;

    case E_CITY:
        if (emerging) {
            print1("You emerge onto the street.");
            emerging = false;
        }
        else {
            print1("You pass through the massive gates of Rampart, the city.");
            setPlayerXY(62, 20);
        }
        if (City == NULL) load_city(true);

        Level = City;
        ScreenOffset = Player.y - (ScreenLength/2);
        ScreenXOffset =Player.x - (ScreenWidth/2);
        screencheck(Player.x, Player.y);
        show_screen();
		Village village;
        break;
    case E_VILLAGE:
        if (!emerging) {
            /* different villages per different locations */
            switch(Country[Player.x][Player.y].aux) {
			case Village::VIL_STARVIEW:
                setPlayerXY( 0, 6 );
                Villagenum = Village::VIL_STARVIEW;
                break;
            default:
                print3("Very strange, a nonexistent village.");
            case Village::VIL_WOODMERE:
                setPlayerXY( 39, 15 );
                Villagenum = Village::VIL_WOODMERE;
                break;
            case Village::VIL_STORMWAT:
                setPlayerXY( 63, 8 );
                Villagenum = Village::VIL_STORMWAT;
                break;
            case Village::VIL_THAUMARI:
                setPlayerXY( 32, 15 );
                Villagenum = Village::VIL_THAUMARI;
                break;
            case Village::VIL_SKORCH:
                setPlayerXY( 2, 8 );
                Villagenum = Village::VIL_SKORCH;
                break;
            case Village::VIL_WHORFEN:
                setPlayerXY( 2, 2 );
                Villagenum = Village::VIL_WHORFEN;
                break;
            }
        }
        if ((! emerging) || (TempLevel == NULL)) village.load_village(Villagenum, true);
        else if (TempLevel->environment != E_VILLAGE) village.load_village(Villagenum, true);
        else Level = TempLevel;

        if (emerging) {
            print1("You emerge onto the street.");
            emerging = false;
        }
        else
            print1("You enter a small rural village.");
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;
    case E_CAVES:
        print1("You enter a dark cleft in a hillside;");
        print2("You note signs of recent passage in the dirt nearby.");
        if (gamestatusp(MOUNTED)) {
            morewait();
            print1("Seeing as you might not be coming back, you feel compelled");
            print2("to let your horse go, rather than keep him hobbled outside.");
            resetgamestatus(MOUNTED);
            calc_melee();
        }
        MaxDungeonLevels = CAVELEVELS;
        if (Current_Dungeon != E_CAVES) {
            free_dungeon();
            Dungeon = NULL;
            Level = NULL;
            Current_Dungeon = E_CAVES;
        }
        change_level(0,1,false);
        break;
    case E_VOLCANO:
        print1("You pass down through the glowing crater.");
        if (gamestatusp(MOUNTED)) {
            morewait();
            print1("Seeing as you might not be coming back, you feel compelled");
            print2("to let your horse go, rather than keep him hobbled outside.");
            resetgamestatus(MOUNTED);
            calc_melee();
        }
        MaxDungeonLevels = VOLCANOLEVELS;
        if (Current_Dungeon != E_VOLCANO) {
            free_dungeon();
            Dungeon = NULL;
            Level = NULL;
            Current_Dungeon = E_VOLCANO;
        }
        change_level(0,1,false);
        break;
    case E_ASTRAL:
        print1("You are in a weird flickery maze.");
        if (gamestatusp(MOUNTED)) {
            print2("Your horse doesn't seem to have made it....");
            resetgamestatus(MOUNTED);
            calc_melee();
        }
        MaxDungeonLevels = ASTRALLEVELS;
        if (Current_Dungeon != E_ASTRAL) {
            free_dungeon();
            Dungeon = NULL;
            Level = NULL;
            Current_Dungeon = E_ASTRAL;
        }
        change_level(0,1,false);
        break;
    case E_CASTLE:
        print1("You cross the drawbridge. Strange forms move beneath the water.");
        if (gamestatusp(MOUNTED)) {
            morewait();
            print1("Seeing as you might not be coming back, you feel compelled");
            print2("to let your horse go, rather than keep him hobbled outside.");
            resetgamestatus(MOUNTED);
        }
        MaxDungeonLevels = CASTLELEVELS;
        if (Current_Dungeon != E_CASTLE) {
            free_dungeon();
            Dungeon = NULL;
            Level = NULL;
            Current_Dungeon = E_CASTLE;
        }
        change_level(0,1,false);
        break;
    case E_SEWERS:
        print1("You pry open a manhole and descend into the sewers below.");
        if (gamestatusp(MOUNTED)) {
            print2("You horse waits patiently outside the sewer entrance....");
            dismount_steed();
        }
        MaxDungeonLevels = SEWERLEVELS;
        if (Current_Dungeon != E_SEWERS) {
            free_dungeon();
            Dungeon = NULL;
            Level = NULL;
            Current_Dungeon = E_SEWERS;
        }
        change_level(0,1,false);
        break;
    case E_COUNTRYSIDE:
        print1("You return to the fresh air of the open countryside.");
        if (Last_Environment == E_CITY) {
            setPlayerXY(27,19);
        }
        else {
            setPlayerXY(LastCountryLocX,LastCountryLocY);
        }
        for(i=0; i<9; i++)
            c_set(Player.x+Dirs[0][i], Player.y+Dirs[1][i], SEEN);
        ScreenOffset = Player.y - (ScreenLength/2);
        /*     ScreenXOffset = Player.x - (ScreenWidth/2); PGM*/
        show_screen();
        break;
    case E_TACTICAL_MAP:
        print1("You are now on the tactical screen; exit off any side to leave");
        make_country_screen(Country[Player.x][Player.y].current_terrain_type);
        make_country_monsters(Country[Player.x][Player.y].current_terrain_type);
        setPlayerXY( Level->level_width/2, Level->level_length/2 );

        /* This should be altered to use setPlayerXY(x,y) PGM */

        while (Level->site[Player.x][Player.y].locchar == WATER) {
            if (Player.y < Level->level_length/2 + 5)
                Player.y++;
            else if (Player.x > Level->level_width/2 - 10) {
                Player.x--;
                Player.y = Level->level_length/2 - 5;
            }
            else {
                Level->site[Player.x][Player.y].locchar =
                    Level->site[Player.x][Player.y].showchar = FLOOR;
                Level->site[Player.x][Player.y].p_locf = L_NO_OP;
            }
        }
        ScreenOffset = 0;
        ScreenXOffset = 0;
        show_screen();
        break;
    case E_PALACE:
        print1("You enter the dungeons of the ruined palace.");
        if (gamestatusp(MOUNTED)) {
            morewait();
            print1("Seeing as you might not be coming back, you feel compelled");
            print2("to let your horse go, rather than keep him hobbled outside.");
            resetgamestatus(MOUNTED);
        }
        MaxDungeonLevels = PALACELEVELS;
        if (Current_Dungeon != E_PALACE) {
            free_dungeon();
            Dungeon = NULL;
            Level = NULL;
            Current_Dungeon = E_PALACE;
        }
        change_level(0,1,false);
        break;
    case E_NEVER_NEVER_LAND:
    default:
        print1("There must be some mistake. You don't look like Peter Pan.");
        print2("(But here you are in Never-Never Land)");
        ScreenOffset = Player.y - (ScreenLength/2);
        ScreenXOffset = Player.x - (ScreenWidth/2);
        show_screen();
        break;
    }
    setlastxy(Player.x, Player.y);
    if (Current_Environment != E_COUNTRYSIDE)
        showroom(Level->site[Player.x][Player.y].roomnumber);
    else
        terrain_check(false);
}
