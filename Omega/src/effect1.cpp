/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* effect1.c */

#include "glob.h"

/* enchant */
void enchant(int delta)
{
    int i,used = false;
    long change_cash;

    if (delta < 0) {
        i = random_item();
        if (i == ABORT || Player.possessions[i]->usef == I_NOTHING ||
                Player.possessions[i]->usef == I_NO_OP ||
                Player.possessions[i]->usef == I_NORMAL_ARMOR ||
                Player.possessions[i]->usef == I_NORMAL_WEAPON ||
                Player.possessions[i]->usef == I_NORMAL_SHIELD ||
                Player.possessions[i]->objchar == FOOD ||
                Player.possessions[i]->objchar == MISSILEWEAPON) {
            print1("You feel fortunate.");
            morewait();
        }
        else if (Player.possessions[i]->blessing < 0 ||
                 (Player.possessions[i]->objchar == ARTIFACT && random_range(3))) {
            if (Player.possessions[i]->uniqueness == COMMON)
                print1("Your ");
            nprint1(itemid(Player.possessions[i]));
            nprint1(" glows, but the glow flickers out...");
            morewait();
        }
        else {
            used = (Player.possessions[i]->used);
            if (used) {
                Player.possessions[i]->used = false;
                item_use(Player.possessions[i]);
            }
            if (Player.possessions[i]->uniqueness == COMMON)
                print1("Your ");
            nprint1(itemid(Player.possessions[i]));
            nprint1(" radiates an aura of mundanity!");
            morewait();
            Player.possessions[i]->plus = 0;
            Player.possessions[i]->charge = -1;
            Player.possessions[i]->usef = I_NOTHING;
            if (used) {
                Player.possessions[i]->used = true;
                item_use(Player.possessions[i]);
            }
        }
    }
    else {
        i = getitem(CASH);
        if (i == ABORT) {
            print1("You feel unlucky.");
            morewait();
        }
        else if (i == CASHVALUE) {
            print1("You enchant your money.... What a concept!");
            change_cash = Player.cash*(random_range(7) - 3)/6;
            if (change_cash > 0) print2("Seems to have been a good idea!");
            else print2("Maybe it wasn't such a good idea....");
            Player.cash += change_cash;
            morewait();
        }
        else if (Player.possessions[i]->objchar == ARTIFACT) {
            if (Player.possessions[i]->usef !=
                    Objects[Player.possessions[i]->id].usef) {
                print1("It re-acquires its magical aura!");
                Player.possessions[i]->usef = Objects[Player.possessions[i]->id].usef;
            }
            else {
                print1("The enchantment spell enfolds the ");
                nprint1(itemid(Player.possessions[i]));
                print2("and the potent enchantment of the Artifact causes a backlash!");
                morewait();
                clearmsg();
                manastorm(Player.x,Player.y,Player.possessions[i]->level*5);
            }
        }
        else {
            if (Player.possessions[i]->plus > random_range(20)+1) {
                print1("Uh-oh, the force of the enchantment was too much!");
                print2("There is a loud explosion!");
                morewait();
                manastorm(Player.x,Player.y,Player.possessions[i]->plus*5);
                dispose_lost_objects(1,Player.possessions[i]);
            }
            else {
                used = (Player.possessions[i]->used);
                if (used) {
                    State.setSuppressPrinting( true );
                    Player.possessions[i]->used = false;
                    item_use(Player.possessions[i]);
                    State.setSuppressPrinting( false );
                }
                print1("The item shines!");
                morewait();
                Player.possessions[i]->plus += delta+1;
                if (Player.possessions[i]->charge > -1)
                    Player.possessions[i]->charge +=
                        ((delta+1) * (random_range(10) + 1));
                if (used) {
                    State.setSuppressPrinting( true );
                    Player.possessions[i]->used = true;
                    item_use(Player.possessions[i]);
                    State.setSuppressPrinting( false );
                }
            }
        }
        calc_melee();
    }
}

/* bless */
void bless(int blessing)
{
    int index,used;

    if (blessing < 0) {
        index = random_item();
        if (index == ABORT) {
            print1("You feel fortunate.");
            morewait();
        }
        else {
            print1("A foul odor arises from ");
            if (Player.possessions[index]->uniqueness == COMMON)
                nprint1("your ");
            nprint1(itemid(Player.possessions[index]));
            morewait();
            used = (Player.possessions[index]->used);
            if (used) {
                State.setSuppressPrinting( true );
                Player.possessions[index]->used = false;
                item_use(Player.possessions[index]);
                State.setSuppressPrinting( false );
            }
            Player.possessions[index]->blessing -= 2;
            if (Player.possessions[index]->blessing < 0)
                Player.possessions[index]->plus =
                    abs(Player.possessions[index]->plus) - 1;
            if (used) {
                State.setSuppressPrinting( true );
                Player.possessions[index]->used = true;
                item_use(Player.possessions[index]);
                State.setSuppressPrinting( false );
            }
        }
    }
    else {
        index = getitem(NULL_ITEM);
        if (index == CASHVALUE) {
            print1("Blessing your money has no effect.");
            morewait();
        }
        else if (index != ABORT) {
            used = Player.possessions[index]->isUsed();
            if (used) {
                State.setSuppressPrinting( true );
                Player.possessions[index]->used = false;
                item_use(Player.possessions[index]);
                State.setSuppressPrinting( false );
            }
            print1("A pure white light surrounds the item... ");
            if (Player.possessions[index]->blessing < 0-(blessing+1)) {
                print2("which is evil enough to resist the effect of the blessing!");
                morewait();
            }
            else if (Player.possessions[index]->blessing < -1) {
                print2("which disintegrates under the influence of the holy aura!");
                morewait();
                Player.itemweight -=  Player.possessions[index]->weight;
                dispose_lost_objects(1,Player.possessions[index]);
            }
            else if (Player.possessions[index]->blessing < blessing+1) {
                print2("which now seems affected by afflatus!");
                morewait();
                Player.possessions[index]->blessing++;
                Player.possessions[index]->plus =
                    abs(Player.possessions[index]->plus)+1;
            }
            else {
                print2("The hierolux fades without any appreciable effect....");
                morewait();
            }
            if (used && (Player.possessions[index] != NULL)) {
                State.setSuppressPrinting( true );
                Player.possessions[index]->used = true;
                item_use(Player.possessions[index]);
                State.setSuppressPrinting( false );
            }
        }
    }
    calc_melee();
}



void heal(int amount)
{
    if (amount > -1) {
        mprint("You feel better.");
        if (Player.hp < Player.maxhp + amount)
        {
            Player.hp += random_range(10*amount)+1;
            if (Player.hp > Player.maxhp)
                Player.hp = Player.maxhp + amount;
        }
        Player.status[BLINDED] = 0;
    }
    else {
        mprint("You feel unwell.");
        Player.hp -= random_range(10*abs(amount)+1);
        if (Player.hp < 0)
            p_death("magical disruption");
    }
    dataprint();
}


void fbolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
    bolt(fx,fy,tx,ty,hit,dmg,FLAME);
}

void lbolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
    bolt(fx,fy,tx,ty,hit,dmg,ELECTRICITY);
}

/* Added 12/30/98 DG */
void icebolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
    bolt(fx,fy,tx,ty,hit,dmg,COLD);
}

void nbolt(int fx, int fy, int tx, int ty, int hit, int dmg)
{
    bolt(fx,fy,tx,ty,hit,dmg,NORMAL_DAMAGE);
}


/* from f to t */
void bolt(int fx, int fy, int tx, int ty, int hit, int dmg, int dtype)
{
    int xx,yy;
    Monster *target;
    Symbol boltchar = '?';
    xx = fx;
    yy = fy;

    switch(dtype) {
    case FLAME:
        boltchar=('*' | CLR(LIGHT_RED));
        break;
    case ELECTRICITY:
        boltchar = ('^' | CLR(LIGHT_BLUE));
        break;
    case NORMAL_DAMAGE:
        boltchar = ('!' | CLR(BROWN));
        break;
    case COLD:
        boltchar=('o' | CLR(WHITE));
        break;
    default:
        assert(false); /* this should never happen, right? WDT */
    }
    clearmsg();

    do_los(boltchar,&xx,&yy,tx,ty);

    if ((xx == Player.x) && (yy == Player.y)) {
        if (Player.status[DEFLECTION] > 0)
            mprint("The bolt just missed you!");
        else {
            switch (dtype) {
            case FLAME:
                mprint("You were blasted by a firebolt!");
                p_damage(random_range(dmg),dtype,"a firebolt");
                break;
            case ELECTRICITY:
                mprint("You were zapped by lightning!");
                p_damage(random_range(dmg),dtype,"a bolt of lightning");
                break;
            case NORMAL_DAMAGE:
                mprint("You were hit by a missile!");
                p_damage(random_range(dmg),dtype,"a missile");
                break;
            case COLD:
                mprint("You were hit by an icicle!");
                p_damage(random_range(dmg),dtype,"an icicle");
                break;
            }
        }
    }
    else if (NULL != (target = Level->site[xx][yy].creature)) {
        if (hitp(hit,target->ac)) {
            if (target->uniqueness == COMMON) {
                strcpy(Str1,"The ");
                strcat(Str1,target->name);
            }
            else strcpy(Str1,target->name);
            switch (dtype) {
                /* WDT: these sentances really ought to be livened up.  Especially
                 * in full verbose mode. */
            case FLAME:
                strcat(Str1," was blasted by a firebolt!");
                break;
            case ELECTRICITY:
                strcat(Str1," was zapped by lightning!");
                break;
            case NORMAL_DAMAGE:
                strcat(Str1," was hit by a missile!");
                break;
            case COLD:
                strcat(Str1," was hit by an icicle!");
                break;
            }
            mprint(Str1);
            m_status_set(target,HOSTILE);
            target->m_damage(random_range(dmg),dtype);
        }
        else {
            if (target->uniqueness == COMMON) {
                strcpy(Str1,"The ");
                strcat(Str1,target->name);
            }
            else strcpy(Str1,target->name);
            switch (dtype) {
            case FLAME:
                strcat(Str1," was missed by a firebolt!");
                break;
            case ELECTRICITY:
                strcat(Str1," was missed by lightning!");
                break;
            case NORMAL_DAMAGE:
                strcat(Str1," was missed by a missile!");
                break;
            case COLD:
                strcat(Str1," was missed by a flying icicle!");
                break;
            }
            mprint(Str1);
        }
    }
    else if (Level->site[xx][yy].locchar == HEDGE)
        if (Level->site[xx][yy].p_locf != L_TRIFID) {
            if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
                mprint("The hedge is blasted away!");
                Level->site[xx][yy].p_locf = L_NO_OP;
                Level->site[xx][yy].locchar = FLOOR;
                plotspot(xx, yy, true);
                lset(xx, yy, CHANGED);
            }
            else mprint("The hedge is unaffected.");
        }
        else mprint("The trifid absorbs the energy and laughs!");
    else if (Level->site[xx][yy].locchar == WATER)
        if (dtype == FLAME) {
            mprint("The water is vaporised!");
            Level->site[xx][yy].p_locf = L_NO_OP;
            Level->site[xx][yy].locchar = FLOOR;
            lset(xx, yy, CHANGED);
        }
}


void lball(int fx, int fy, int tx, int ty, int dmg)
{
    ball(fx,fy,tx,ty,dmg,ELECTRICITY);
}

void manastorm(int x, int y, int dmg)
{
    ball(x,y,x,y,dmg,UNSTOPPABLE);
}

void snowball(int fx, int fy, int tx, int ty, int dmg)
{
    ball(fx,fy,tx,ty,dmg,COLD);
}

void fball(int fx, int fy, int tx, int ty, int dmg)
{
    ball(fx,fy,tx,ty,dmg,FLAME);
}


/* from f to t */
void ball(int fx, int fy, int tx, int ty, int dmg, int dtype)
{
    int xx,yy,ex,ey,i;
    Monster *target;
    Symbol expchar=('@' | CLR(LIGHT_PURPLE));

    xx = fx;
    yy = fy;

    switch(dtype) {
    case FLAME:
        expchar=('*' | CLR(LIGHT_RED));
        break;
    case COLD:
        expchar=('o' | CLR(WHITE));
        break;
    case ELECTRICITY:
        expchar=('^' | CLR(LIGHT_BLUE));
        break;
    }

    do_los(expchar,&xx,&yy,tx,ty);
    draw_explosion(expchar,xx,yy);
    for(i=0; i<9; i++) {
        ex = xx + Dirs[0][i];
        ey = yy + Dirs[1][i];

        if ((ex == Player.x) && (ey == Player.y)) {
            switch(dtype) {
            case FLAME:
                mprint("You were blasted by a fireball!");
                p_damage(random_range(dmg),FLAME,"a fireball");
                break;
            case COLD:
                mprint("You were blasted by a snowball!");
                p_damage(random_range(dmg),COLD,"a snowball");
                break;
            case ELECTRICITY:
                mprint("You were blasted by ball lightning!");
                p_damage(random_range(dmg),ELECTRICITY,"ball lightning");
                break;
            case UNSTOPPABLE:
                mprint("Oh No! Manastorm!");
                p_damage(random_range(dmg),UNSTOPPABLE,"a manastorm!");
                break;
            }
        }
        if (NULL != (target = Level->site[ex][ey].creature)) {
            if (los_p(Player.x,Player.y,target->x,target->y)) {
                if (target->uniqueness == COMMON) {
                    strcpy(Str1,"The ");
                    strcat(Str1,target->name);
                }
                else strcpy(Str1,target->name);
                switch(dtype) {
                case FLAME:
                    strcat(Str1," was zorched by a fireball!");
                    break;
                case COLD:
                    strcat(Str1," was blasted by a snowball!");
                    break;
                case ELECTRICITY:
                    strcat(Str1," was zapped by ball lightning!");
                    break;
                case UNSTOPPABLE:
                    strcat(Str1," was nuked by a manastorm!");
                    break;
                }
                mprint(Str1);
            }
            m_status_set(target,HOSTILE);
            target->m_damage(random_range(dmg),dtype);
        }
        if (Level->site[ex][ey].locchar == HEDGE)
            if (Level->site[ex][ey].p_locf != L_TRIFID) {
                if ((dtype == FLAME)||(dtype == ELECTRICITY)) {
                    mprint("The hedge is blasted away!");
                    Level->site[ex][ey].p_locf = L_NO_OP;
                    Level->site[ex][ey].locchar = FLOOR;
                    plotspot(ex,ey,true);
                    lset(ex, ey, CHANGED);
                }
                else mprint("The hedge is unaffected.");
            }
            else mprint("The trifid absorbs the energy and laughs!");
        else if (Level->site[ex][ey].locchar == WATER)
            if (dtype == FLAME) {
                mprint("The water is vaporised!");
                Level->site[ex][ey].p_locf = L_NO_OP;
                Level->site[ex][ey].locchar = FLOOR;
                plotspot(ex,ey,true);
                lset(ex, ey, CHANGED);
            }
    }
}

void mondet(int blessing)
{
    MonsterList* ml;
    for (ml=Level->mlist; ml!=NULL; ml=ml->next)
        if (ml->monster->hp > 0) /* FIXED 12/30/98 DG */
        {
            if (blessing > -1)
                plotmon(ml->monster);
            else
                putspot(random_range(Level->level_width), random_range(Level->level_length),
                        Monsters[random_range(NUMMONSTERS)].symbol);
        }
    levelrefresh();
    morewait();
    show_screen();
}


void objdet(int blessing)
{
    int i,j;
    for (i=0; i<Level->level_width; i++)
        for (j=0; j<Level->level_length; j++)
            if (Level->site[i][j].things != NULL) {
                if (blessing < 0)
                    putspot(random_range(Level->level_width),
                            random_range(Level->level_length),
                            Level->site[i][j].things->thing->objchar);
                else putspot(i,j,Level->site[i][j].things->thing->objchar);
            }
    levelrefresh();
    morewait();
    show_screen();
}

void identify(int blessing)
{
    int index;

    clearmsg();

    if (blessing == 0) {
        index = getitem_prompt("Identify: ", NULL_ITEM);
        if (index == CASHVALUE) print3("Your money is really money.");
        else if (index == ABORT)
            State.setSkipMonsters();
        else {
            if (Player.possessions[index]->objchar == FOOD)
                Player.possessions[index]->known = 1;
            else {
                Player.possessions[index]->known = 2;
                Objects[Player.possessions[index]->id].known = 1;
            }
            print1("Identified: ");
            mprint(itemid(Player.possessions[index]));
        }
    }
    else if (blessing < 0) {
        print2("You feel forgetful.");
        for (index=0; index<MAXITEMS; index++)
            if (Player.possessions[index] != NULL) {
                Player.possessions[index]->known = 0;
                Objects[Player.possessions[index]->id].known = 0;
            }
    }
    else {
        print2("You feel encyclopaedic.");
        for (index=0; index<MAXITEMS; index++)
            if (Player.possessions[index] != NULL) {
                if (Player.possessions[index]->objchar == FOOD)
                    Player.possessions[index]->known = 1;
                else {
                    Player.possessions[index]->known = 2;
                    Objects[Player.possessions[index]->id].known = 1;
                }
            }
        for (index=0; index<Player.packptr; index++)
            if (Player.pack[index] != NULL) {
                if (Player.pack[index]->objchar == FOOD)
                    Player.pack[index]->known = 1;
                else {
                    Player.pack[index]->known = 2;
                    Objects[Player.pack[index]->id].known = 1;
                }
            }
    }
    calc_melee();
}


/* returns index of random item, ABORT if player carrying none */
int random_item(void)
{
    int item = ABORT,tries=0;
    int number = 0;

    for(tries=0; tries<MAXITEMS; tries++)
        if (Player.possessions[tries] != NULL) {
            number++;
            if (random_range(number) == 0)
                item = tries;
        }
    return(item);
}

/* returns index of an item to steal, ABORT if player carrying none */
int stolen_item(void)
{
    int idx;
    int nextitem;

    int cards[MAXITEMS];

    nextitem = 0;

    /* look for bank cards */
    for (idx = 0; idx < MAXITEMS; ++idx)
    {
        if (0 == Player.possessions[idx]) continue;
        if (Player.possessions[idx]->id < OB_DEBIT_CARD) continue;
        if (Player.possessions[idx]->id > OB_SMART_CARD) continue;

        cards[nextitem++] = idx;
    }

    if (0 == nextitem) return random_item();

    if (nextitem > 1)
        shuffle(cards, nextitem);

    if (random_range(100) < 75)
    {
        /* oh that sucks... */
        return cards[0];
    }

    return random_item();
}
