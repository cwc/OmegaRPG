/* omega copyright (C) 1987,1988,1989 by Laurence Raphael Brothers */
/* itemf1.c */

/* various item functions: potions,scrolls,boots,cloaks,things,food */

#include "glob.h"

/* general item functions */

void i_no_op(Object* o)
{
}

void i_nothing(Object* o)
{
}

/*  scroll functions */

void i_knowledge(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    knowledge(o->blessing);
}

static int is_capital_letter (char ch)
{
    if (ch >= 'A' && ch <= 'Z') return true;
    return false;
}

static int is_vowel (char ch)
{
    if ('a' == ch) return true;
    if ('e' == ch) return true;
    if ('i' == ch) return true;
    if ('o' == ch) return true;
    if ('u' == ch) return true;
    return false;
}

void i_jane_t (Object* o)
{
    int volume;
    int range_idx;
    int range_beg;
    int range_end;
    char first_letter;

    Objects[o->id].known = 1;

    print1("Jane's Guide to the World's Treasures: ");

    volume = random_range(6);
    switch (volume)
    {
    case 0:
        nprint1("SCROLLS");
        range_beg = SCROLLID;
        range_end = POTIONID;
        break;

    case 1:
        nprint1("POTIONS");
        range_beg = POTIONID;
        range_end = WEAPONID;
        break;

    case 2:
        nprint1("CLOAKS");
        range_beg = CLOAKID;
        range_end = BOOTID;
        break;

    case 3:
        nprint1("BOOTS");
        range_beg = BOOTID;
        range_end = RINGID;
        break;

    case 4:
        nprint1("RINGS");
        range_beg = RINGID;
        range_end = STICKID;
        break;

    case 5:
        nprint1("STICKS");
        range_beg = STICKID;
        range_end = ARTIFACTID;
        break;

    default:
        /* bomb on error */
        range_beg = range_end = 0;
        assert(false);
    }

    menuclear();
    menuprint("You could probably now recognise:\n");

    for(range_idx = range_beg; range_idx < range_end; ++range_idx)
    {
        Objects[range_idx].known = 1;
        first_letter = Objects[range_idx].truename[0];

        if (is_capital_letter(first_letter) || volume == 3)
            sprintf(Str1, "   %s\n", Objects[range_idx].truename);
        else if (is_vowel(first_letter))
            sprintf(Str1, "   an %s\n", Objects[range_idx].truename);
        else
            sprintf(Str1, "   a %s\n", Objects[range_idx].truename);

        menuprint(Str1);
    }

    showmenu();
    morewait();
    xredraw();
}


void i_flux(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    flux(o->blessing);
}


/* enchantment */
void i_enchant(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    enchant(o->blessing < 0 ? -1-o->plus : o->plus+1);
}

/* scroll of clairvoyance */
void i_clairvoyance(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    if (o->blessing < 0)
        amnesia();
    else clairvoyance(5+o->blessing*5);
}



void i_acquire(Object* o)
{
    int blessing;

    if (o->blessing > -1)
        Objects[o->id].known = 1;
    blessing = o->blessing;
    *o = Objects[OB_BLANK_SCROLL]; /* blank out the scroll */
    acquire(blessing);
}

void i_teleport(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    p_teleport(o->blessing);
}


void i_spells(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    mprint("A scroll of spells.");
    morewait();
    learnspell(o->blessing);
}



/* scroll of blessing */
void i_bless(Object* o)
{
    Objects[o->id].known = 1;
    bless(o->blessing);
}

/* scroll of wishing */
void i_wish(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    wish(o->blessing);
    *o = Objects[OB_BLANK_SCROLL]; /* blank out the scroll */
}

/* scroll of displacement */
void i_displace(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    displace(o->blessing);
}


/* scroll of deflection */
void i_deflect(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    deflection(o->blessing);
}

/* scroll of identification */
void i_id(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    identify(o->blessing);
}

/* potion functions */

/* potion of healing */
void i_heal(Object* o)
{
    if (o->blessing > -1) {
        Objects[o->id].known = 1;
        heal(1+o->plus);
    }
    else heal(-1-abs(o->plus));
}

/* potion of monster detection */
void i_mondet(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    mondet(o->blessing);
}


/* potion of object detection */
void i_objdet(Object* o)
{

    if (o->blessing > -1)
        Objects[o->id].known = 1;
    objdet(o->blessing);
}

/* potion of neutralize poison */
void i_neutralize_poison(Object* o)
{
    if (o->blessing > -1) {
        Objects[o->id].known = 1;
        mprint("You feel vital!");
        Player.status[POISONED] = 0;
    }
    else p_poison(random_range(20)+5);
}


/* potion of sleep */
void i_sleep_self(Object* o)
{
    sleep_player(6);
    Objects[o->id].known = 1;
}




/* potion of speed */
void i_speed(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    haste(o->blessing);
}


/* potion of restoration */
void i_restore(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    recover_stat(o->blessing);
}

void i_augment(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    augment(o->blessing);
}

void i_azoth(Object* o)
{
    if (o->plus < 0) {
        mprint("The mercury was poisonous!");
        p_poison(25);
    }
    else if (o->plus == 0) {
        mprint("The partially enchanted azoth makes you sick!");
        Player.con = ((int) (Player.con / 2));
        calc_melee();
    }
    else if (o->blessing < 1) {
        mprint("The unblessed azoth warps your soul!");
        Player.pow = Player.maxpow = ((int) (Player.maxpow / 2));
        level_drain(random_range(10),"cursed azoth");
    }
    else {
        mprint("The azoth fills you with cosmic power!");
        if (Player.str > Player.maxstr*2) {
            mprint("The power rages out of control!");
            p_death("overdose of azoth");
        }
        else {
            heal(10);
            cleanse(1);
            Player.mana = calcmana()*3;
            toggle_item_use(true);
            Player.str = (Player.maxstr++)*3;
            toggle_item_use(false);
        }
    }
}

void i_regenerate(Object* o)
{
    regenerate(o->blessing);
}





/* boots functions */
void i_perm_speed(Object* o)
{
    if (o->blessing > -1) {
        if (o->used) {
            o->known = 2;
            Objects[o->id].known = 1;
            if (Player.status[SLOWED] > 0) {
                Player.status[SLOWED] = 0;
            }
            mprint("The world slows down!");
            Player.status[HASTED] += 1500;
        }
        else {
            Player.status[HASTED] -= 1500;
            if (Player.status[HASTED] < 1)
                mprint("The world speeds up again.");
        }
    }
    else {
        if (o->used) {
            if (Player.status[HASTED] > 0) {
                Player.status[HASTED] = 0;
            }
            mprint("You feel slower.");
            Player.status[SLOWED] += 1500;
        }
        else  {
            Player.status[SLOWED] -= 1500;
            if (Player.status[SLOWED] < 1)
                mprint("You feel quicker again.");
        }
    }
}

/* cloak functions */
void i_perm_displace(Object* o)
{
    if (o->blessing > -1) {
        if (o->used) {
            mprint("You feel dislocated.");
            Player.status[DISPLACED] += 1500;
        }
        else {
            Player.status[DISPLACED] -= 1500;
            if (Player.status[DISPLACED] < 1) {
                mprint("You feel a sense of position.");
                Player.status[DISPLACED] = 0;
            }
        }
    }
    else {
        if (o->used) {
            mprint("You have a forboding of bodily harm!");
            Player.status[VULNERABLE] += 1500;
        }
        else {
            Player.status[VULNERABLE] -= 1500;
            if (Player.status[VULNERABLE] < 1) {
                mprint("You feel less endangered.");
                Player.status[VULNERABLE] = 0;
            }
        }
    }
}

void i_perm_negimmune(Object* o)
{
    if (o->blessing > -1) {
        if (o->used) {
            Player.immunity[NEGENERGY]++;
        }
        else Player.immunity[NEGENERGY]--;
    }
    else if (o->used)
        level_drain(abs(o->blessing),"cursed cloak of level drain");
}

/* food functions */


void i_food(Object* o)
{
    switch(random_range(6)) {
    case 0:
        mprint("That tasted horrible!");
        break;
    case 1:
        mprint("Yum!");
        break;
    case 2:
        mprint("How nauseous!");
        break;
    case 3:
        mprint("Can I have some more? Please?");
        break;
    case 4:
        mprint("Your mouth feels like it is growing hair!");
        break;
    case 5:
        mprint("Tastes like home cooking!");
        break;
    }
}

void i_stim(Object* o)
{
    mprint("You feel Hyper!");
    i_speed(o);
    Player.str +=3;
    Player.con -=1;
    calc_melee();
}

void i_pow(Object* o)
{
    mprint("You feel a surge of mystic power!");
    Player.mana = 2 * calcmana();
}

void i_poison_food(Object* o)
{
    mprint("This food was contaminated with cyanide!");
    p_poison(random_range(20)+5);
}

void i_pepper_food(Object* o)
{
    mprint("You innocently start to chew the szechuan pepper.....");
    morewait();
    mprint("hot.");
    morewait();
    mprint("Hot.");
    morewait();
    mprint("Hot!");
    morewait();
    mprint("HOT!!!!!!");
    morewait();
    p_damage(1,UNSTOPPABLE,"a szechuan pepper");
    mprint("Your sinuses melt and run out your ears.");
    mprint("Your mouth and throat seem to be permanently on fire.");
    mprint("You feel much more awake now....");
    Player.immunity[SLEEP]++;
}

void i_lembas(Object* o)
{
    heal(10);
    cleanse(0);
    Player.food = 40;
}


void i_cure(Object* o)
{
    cure(o->blessing);
}

void i_immune(Object* o)
{
    if (o->blessing > 0) {
        mprint("You feel a sense of innoculation");
        Player.immunity[INFECTION]++;
        cure(o->blessing);
    }
}

void i_breathing(Object* o)
{

    if (o->blessing > -1)
        Objects[o->id].known = 1;
    breathe(o->blessing);
}


void i_invisible(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    invisible(o->blessing);
}


void i_perm_invisible(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    if (o->used) {
        if (o->blessing > -1) {
            mprint("You feel transparent!");
            Player.status[INVISIBLE] += 1500;
        }
        else {
            mprint("You feel a forboding of bodily harm!");
            Player.status[VULNERABLE] += 1500;
        }
    }
    else {
        if (o->blessing > -1) {
            Player.status[INVISIBLE]-=1500;
            if (Player.status[INVISIBLE] < 1) {
                mprint("You feel opaque again.");
                Player.status[INVISIBLE] = 0;
            }
        }
        else {
            Player.status[VULNERABLE] -= 1500;
            if (Player.status[VULNERABLE] < 1) {
                mprint("You feel less endangered now.");
                Player.status[VULNERABLE] = 0;
            }
        }
    }
}


void i_warp(Object* o)
{
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    warp(o->blessing);
}


void i_alert(Object* o)
{
    if (o->blessing > -1) {
        Objects[o->id].known = 1;
        alert(o->blessing);
    }
}

void i_charge(Object* o)
{
    int i;
    if (o->blessing > -1)
        Objects[o->id].known = 1;
    i = getitem_prompt("A scroll of charging. Charge: ", STICK);
    if (i != ABORT) {
        if (o->blessing < 0) {
            mprint("The stick glows black!");
            Player.possessions[i]->charge = 0;
        }
        else {
            mprint("The stick glows blue!");
            Player.possessions[i]->charge += (random_range(10)+1)*(o->blessing+1);
            if (Player.possessions[i]->charge > 99)
                Player.possessions[i]->charge = 99;
        }
    }
}


void i_fear_resist(Object* o)
{
    if (o->blessing > -1) {
        Objects[o->id].known = 1;
        if (Player.status[AFRAID] > 0) {
            mprint("You feel stauncher now.");
            Player.status[AFRAID] = 0;
        }
    }
    else if (! p_immune(FEAR)) {
        mprint("You panic!");
        Player.status[AFRAID]+=random_range(100);
    }
}



/* use a thieves pick */
void i_pick(Object* o)
{
    int dir;
    int ox,oy;
    o->used = false;
    if ((! o->known) && (! Player.rank[THIEVES]))
        mprint("You have no idea what do with a piece of twisted metal.");
    else {
        if (o->known < 1) o->known = 1;
        Objects[o->id].known = 1;
        mprint("Pick lock:");
        dir = getdir();
        if (dir == ABORT)
            resetgamestatus(SKIP_MONSTERS);
        else {
            ox = Player.x + Dirs[0][dir];
            oy = Player.y + Dirs[1][dir];
            if ((Level->site[ox][oy].locchar != CLOSED_DOOR) ||
                    loc_statusp(ox,oy,SECRET)) {
                mprint("You can't unlock that!");
                resetgamestatus(SKIP_MONSTERS);
            }
            else if (Level->site[ox][oy].aux == LOCKED) {
                if (Level->depth == MaxDungeonLevels-1)
                    mprint("The lock is too complicated for you!!!");
                else if (Level->depth*2 + random_range(50) <
                         Player.dex+Player.level+Player.rank[THIEVES]*10) {
                    mprint("You picked the lock!");
                    Level->site[ox][oy].aux = UNLOCKED;
                    lset(ox, oy, CHANGED);
                    gain_experience(max(3,Level->depth));
                }
                else mprint("You failed to pick the lock.");
            }
            else mprint("That door is already unlocked!");
        }
    }
}

/* use a magic key*/
void i_key(Object* o)
{
    int dir;
    int ox,oy;
    o->used = false;
    mprint("Unlock door: ");
    dir = getdir();
    if (dir == ABORT)
        resetgamestatus(SKIP_MONSTERS);
    else {
        ox = Player.x + Dirs[0][dir];
        oy = Player.y + Dirs[1][dir];
        if ((Level->site[ox][oy].locchar != CLOSED_DOOR) ||
                loc_statusp(ox,oy,SECRET)) {
            mprint("You can't unlock that!");
            resetgamestatus(SKIP_MONSTERS);
        }
        else if (Level->site[ox][oy].aux == LOCKED) {
            mprint("The lock clicks open!");
            Level->site[ox][oy].aux = UNLOCKED;
            lset(ox, oy, CHANGED);
            o->blessing--;
            if ((o->blessing<0)||(Level->depth == MaxDungeonLevels-1)) {
                mprint("The key disintegrates!");
                conform_lost_objects(1,o);
            }
            else
                mprint("Your key glows faintly.");
        }
        else mprint("That door is already unlocked!");
    }
}

void i_corpse(Object* o)
{
    /* WDT HACK: there are some comments in this function which need
     * to be backed up with assert(). */
    /* DAG reply: I don't like to use assert() for minor things like this
       as the game can safely play on, yet an assert() will cause the game
       to exit if it fails.  Not friendly.  Now an error message or error
       log of some sort would make sense. */
    /* WDT discussion: there's no law which demands that an assert has to
     * kill the program -- well, okay, not the standard assert, but I seldom
     * use that one.  And anyhow, we shouldn't distribute a version with the
     * asserts compiled in; that's against the purpose. */
    /* object's charge holds the former monster id */
    switch (o->charge) {
    case MEND_PRIEST:
    case ITIN_MERCH:
    case GUARD:
    case NPC:
    case MERCHANT:
    case ZERO_NPC:
    case HISCORE_NPC:
    case APPR_NINJA:
    case SNEAK_THIEF:
    case BRIGAND:
    case GENIN:
    case MAST_THIEF:
    case CHUNIN:
    case JONIN: /* cannibalism */
        mprint("Yechh! How could you! You didn't even cook him first!");
        if (Player.alignment > 0) Player.food = 25;
        else mprint("You'd eat your own grandmother.");
        if (Player.alignment < 0) mprint("...without salt.");
        Player.food += 8;
        Player.alignment -=10;
        foodcheck();
        break;
    case FNORD: /* fnord */
        mprint("You feel illuminated!");
        Player.iq++;
        break;
    case DENEBIAN: /* denebian slime devil */
        mprint("I don't believe this. You ate Denebian Slime?");
        mprint("You deserve a horrible wasting death, uncurable by any means!");
        break;
    case DRAGONETTE:  /* can't get here... i_usef changed to I_FOOD */
        mprint("Those dragon-steaks were fantastic!");
        Player.food=24;
        foodcheck();
        break;
    case BEHEMOTH:
        mprint("You feel infinitely more virile now.");
        Player.str = max(Player.str,Player.maxstr+10);
        Player.food = 24;
        foodcheck();
        break;
    case INVIS_SLAY:
        mprint("Guess what? You're invisible.");
        if (Player.status[INVISIBLE] < 1000) Player.status[INVISIBLE] = 666;
        Player.food+=6;
        foodcheck();
        break;
    case UNICORN:
        mprint("You ATE a unicorn's horn?!?!?");
        Player.immunity[POISON]=1000;
        break;
    case HORNET: /* can't get here... i_usef changed to I_POISON_FOOD */
    case GRUNT:
    case TSETSE:  /* can't get here... i_usef changed to I_SLEEP_SELF */
    case AGGRAVATOR:
    case BLIPPER: /* can't get here... i_usef changed to I_TELEPORT */
    case GOBLIN:
    case GEEK:
    case NIGHT_GAUNT: /* can't get here... i_usef changed to I_POISON_FOOD */
    case TOVE:
    case TASMANIAN:
    case JUBJUB:  /* can't get here... i_usef changed to I_FOOD */
    case CATEAGLE:
        mprint("Well, you forced it down. Not much nutrition, though.");
        Player.food++;
        foodcheck();
        break;
    case SEWER_RAT:
    case PHANTASTICON: /* can't get here... i_usef changed to I_POISON_FOOD */
    case EYE:  /* can't get here... i_usef changed to I_CLAIRVOYANCE */
    case NASTY:  /* can't get here... i_usef changed to I_INVISIBLE */
    case CATOBLEPAS:
    case HAUNT: /* can't get here... i_usef changed to I_POISON_FOOD */
    case ROUS:
    case DEATHSTAR: /* can't get here... i_usef changed to I_POISON_FOOD */
    case EATER:
        mprint("Oh, yuck. The 'food' seems to be tainted.");
        mprint("You feel very sick. You throw up.");
        Player.food = min(Player.food, 4);
    default:
        mprint("It proved completely inedible, but you tried anyhow.");
    }
}

void i_accuracy(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    accuracy(o->blessing);
}

void i_perm_accuracy(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    if ((o->used) && (o->blessing > -1)) {
        Player.status[ACCURATE] += 1500;
        mprint("You feel skillful and see bulls' eyes everywhere.");
    }
    else {
        Player.status[ACCURATE] -= 1500;
        if (Player.status[ACCURATE] < 1) {
            Player.status[ACCURATE] = 0;
            calc_melee();
            mprint("Your vision blurs....");
        }
    }
}

void i_hero(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    hero(o->blessing);
}

void i_perm_hero(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    if (o->used) {
        if (o->blessing > -1) {
            Player.status[HERO] += 1500;
            calc_melee();
            mprint("You feel super!");
        }
        else {
            Player.status[HERO] = 0;
            calc_melee();
            if (! Player.immunity[FEAR]) {
                Player.status[AFRAID]+=1500;
                mprint("You feel cowardly....");
            }
        }
    }
    else {
        if (o->blessing > -1) {
            Player.status[HERO] -= 1500;
            if (Player.status[HERO] < 1) {
                calc_melee();
                mprint("You feel less super now.");
                Player.status[HERO] = 0;
            }
        }
        else {
            Player.status[AFRAID] -= 1500;
            if (Player.status[AFRAID] < 1) {
                mprint("You finally conquer your fear.");
                Player.status[AFRAID] = 0;
            }
        }
    }
}

void i_levitate(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    levitate(o->blessing);
}

void i_perm_levitate(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    if (o->blessing > -1) {
        if (o->used) {
            Player.status[LEVITATING] += 1400;
            mprint("You start to float a few inches above the floor");
            mprint("You find you can easily control your altitude");
        }
        else {
            Player.status[LEVITATING] -= 1500;
            if (Player.status[LEVITATING] < 1) {
                Player.status[LEVITATING] = 0;
                mprint("You sink to the floor.");
            }
        }
    }
    else i_perm_burden(o);
}

void i_perm_protection(Object* o)
{
    if (o->used) {
        if (o->blessing > -1)
            Player.status[PROTECTION] += abs(o->plus)+1;
        else
            Player.status[PROTECTION] -= abs(o->plus)+1;
    }
    else {
        if (o->blessing > -1)
            Player.status[PROTECTION] -= abs(o->plus)+1;
        else
            Player.status[PROTECTION] += abs(o->plus)+1;
    }
    calc_melee();
}

void i_perm_agility(Object* o)
{
    if (o->known < 1) o->known = 2;
    Objects[o->id].known = 1;
    if (o->used) {
        if (o->blessing > -1)
            Player.agi += abs(o->plus)+1;
        else
            Player.agi -= abs(o->plus)+1;
    }
    else {
        if (o->blessing > -1)
            Player.agi -= abs(o->plus)+1;
        else
            Player.agi += abs(o->plus)+1;
    }
    calc_melee();
}

void i_truesight(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    truesight(o->blessing);
}

void i_perm_truesight(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    if (o->used) {
        if (o->blessing > -1) {
            Player.status[TRUESIGHT] += 1500;
            mprint("You feel sharp!");
        }
        else {
            Player.status[BLINDED] += 1500;
            mprint("You've been blinded!");
        }
    }
    else {
        if (o->blessing > -1) {
            Player.status[TRUESIGHT] -= 1500;
            if (Player.status[TRUESIGHT] < 1) {
                mprint("You feel less keen now.");
                Player.status[TRUESIGHT] = 0;
            }
        }
        else {
            Player.status[BLINDED] -= 1500;
            if (Player.status[BLINDED] < 1) {
                mprint("You can see again!");
                Player.status[BLINDED] = 0;
            }
        }
    }
}

void i_illuminate(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    illuminate(o->blessing);
}

void i_perm_illuminate(Object* o)
{
    if (o->known < 1) o->known = 1;
    Objects[o->id].known = 1;
    if (o->used)
        Player.status[ILLUMINATION]+=1500;
    else
        Player.status[ILLUMINATION]=max(0,Player.status[ILLUMINATION]-1500);
}




void i_trap(Object* o)
{
    Objects[o->id].known = 1;

    if ((Level->site[Player.x][Player.y].locchar != FLOOR) ||
            (Level->site[Player.x][Player.y].p_locf != L_NO_OP))
        mprint("Your attempt fails.");
    else  if (! o->known) {
        mprint("Fiddling with the thing, you have a small accident....");
        p_movefunction(o->aux);
    }
    else {
        mprint("You successfully set a trap at your location.");
        Level->site[Player.x][Player.y].p_locf = o->aux;
        lset(Player.x, Player.y, CHANGED);
    }
    dispose_lost_objects(1,o);
}


void i_raise_portcullis(Object* o)
{
    l_raise_portcullis();
    mprint("The box beeps once and explodes in your hands!");
    conform_lost_objects(1,o);
}

