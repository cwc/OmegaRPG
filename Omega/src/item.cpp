/* omega copyright (C) 1987,1988,1989 by Laurence Raphael Brothers */
/* item.c */

#include "glob.h"

/* make a random new object, returning pointer */
Object* create_object(int itemlevel)
{
    Object* newObject;
    int  r;
    int ok = false;

    while (! ok) {
        newObject = ((Object*) checkmalloc(sizeof(Object)));
        r= random_range(135);
        if (r < 20) make_thing(newObject,-1);
        else if (r < 40) make_food(newObject,-1);
        else if (r < 50) make_scroll(newObject,-1);
        else if (r < 60) make_potion(newObject,-1);
        else if (r < 70) make_weapon(newObject,-1);
        else if (r < 80) make_armor(newObject,-1);
        else if (r < 90) make_shield(newObject,-1);
        else if (r < 100) make_stick(newObject,-1);
        else if (r < 110) make_boots(newObject,-1);
        else if (r < 120) make_cloak(newObject,-1);
        else if (r < 130) make_ring(newObject,-1);
        else make_artifact(newObject,-1);
        /* not ok if object is too good for level, or if unique and already made */
        /* 1/100 chance of finding object if too good for level */
        ok = ((newObject->uniqueness < UNIQUE_MADE) &&
              ((newObject->level < itemlevel+random_range(3))
               || (random_range(100)==23)));
        if (!ok)
        {
            free_obj( newObject, true );
        }
    }
    if (newObject->uniqueness == UNIQUE_UNMADE)
        Objects[newObject->id].uniqueness=UNIQUE_MADE;
    return(newObject);
}

void make_cash(Object* newObject, int level)
{
    *newObject = Objects[CASHID];
    newObject->basevalue = random_range(level*level+10)+1; /* aux is AU value */
    newObject->objstr = cashstr();
    newObject->cursestr = newObject->truename = newObject->objstr;
}

void make_food(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMFOODS);
    *newObject = Objects[FOODID+id];
}


void make_corpse(Object* newObject, Monster* m)
{
    *newObject = Objects[CORPSEID];
    newObject->charge = m->id;
    newObject->weight = m->corpseWeight;
    newObject->basevalue = m->corpseValue;
    newObject->known = 2;
    newObject->objstr = m->corpseString;
    newObject->truename = newObject->cursestr = newObject->objstr;
    if ( m_statusp(m, ALLOC ) )
    {
        /* DAG we are keeping the corpseString here, can free name */
        /*     later, should track this in the object as well, as this */
        /*     is still a memory leak (though smaller than before)  */
        free ( (char *)m->name );
        m->name = Monsters[m->id].name;
        m->corpseString = Monsters[m->id].corpseString;
        m_status_reset( m, ALLOC );
        /* DAG level not otherwise used for corpses.  Use to hold ALLOC info. */
        newObject->level |= ALLOC;
    }
    /* DG I_CANNIBAL not implemented... fall through to code in I_CORPSE */
    /* WDT HACK, of course -- we need to implement I_CANNIBAL. */
#if 0
    if ((m->symbol&0xff) == '@')
        newObject->usef = I_CANNIBAL;
    else
#endif
        if (m_statusp(m,EDIBLE)) {
            newObject->usef = I_FOOD;
            newObject->aux = 6;
        }
        else if (m_statusp(m,POISONOUS))
            newObject->usef = I_POISON_FOOD;
    /* Special corpse-eating effects */
        else switch(m->id) {
            case TSETSE: /*tse tse fly */
            case TORPOR: /*torpor beast */
                newObject->usef = I_SLEEP_SELF;
                break;
            case NASTY:
                newObject->usef = I_INVISIBLE;
                break;
            case BLIPPER:
                newObject->usef = I_TELEPORT;
                break;
            case EYE: /* floating eye -- it's traditional.... */
                newObject->usef = I_CLAIRVOYANCE;
                break;
            case FUZZY: /*astral fuzzy */
                newObject->usef = I_DISPLACE;
                break;
            case SERV_LAW:
                newObject->usef = I_CHAOS;
                break;
            case SERV_CHAOS:
                newObject->usef = I_LAW;
                break;
            case ASTRAL_VAMP: /* astral vampire */
                newObject->usef = I_ENCHANT;
                break;
            case MANABURST:
                newObject->usef = I_SPELLS;
                break;
            case RAKSHASA:
                newObject->usef = I_TRUESIGHT;
                break;
                /* DG fall through to code in I_CORPSE and special case there */
#if 0 /* WDT HACK? */
            case BEHEMOTH:
                newObject->usef = I_HEAL;
                break;
            case UNICORN:
                newObject->usef = I_NEUTRALIZE_POISON;
                break;
#endif
            case COMA: /*coma beast */
                newObject->usef = I_ALERT;
                break;
                /* DG I_INEDIBLE not implemented... fall through to code in I_CORPSE */
#if 0 /* WDT HACK: yawn. */
            default:
                newObject->usef = I_INEDIBLE;
                break;
#endif
            }
}


void make_ring(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMRINGS);
    *newObject = Objects[RINGID+id];
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
    if (newObject->plus == 0) newObject->plus = itemplus()+1;
    if (newObject->blessing < 0) newObject->plus = -1 - abs(newObject->plus);
}

void make_thing (Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMTHINGS);

    *newObject = Objects[THINGID+id];

    if (0 == strcmp(newObject->objstr, "grot"))
    {
        newObject->objstr = grotname();
        newObject->truename = newObject->cursestr = newObject->objstr;
    }
    else if (newObject->id >= CARDID && newObject->id < (CARDID + NUMCARDS))
    {
        switch (newObject->id)
        {
        case OB_DEBIT_CARD:
            newObject->aux = bank_random_account_number();
            break;

        case OB_CREDIT_CARD:
            newObject->charge = random_range(250) + random_range(250) + random_range(250);
            break;

        case OB_PREPAID_CARD:
            newObject->charge = random_range(50) + random_range(50) + random_range(50);
            break;

        case OB_SMART_CARD:
            newObject->aux = bank_random_account_number();
            newObject->charge = random_range(500) + random_range(500) + random_range(500);
            break;
        }
    }
}


void make_scroll(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMSCROLLS);
    *newObject = Objects[SCROLLID+id];
    /* if a scroll of spells, aux is the spell id in Spells */
    if (newObject->id == OB_SPELLS_SCROLL) {
        newObject->aux = random_range(NUMSPELLS);
    }
}

void make_potion(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMPOTIONS);
    *newObject = Objects[POTIONID+id];
    if (newObject->plus == 0) newObject->plus = itemplus();
}

void make_weapon(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMWEAPONS);
    *newObject = Objects[WEAPONID+id];
    if ((id == 28) || (id == 29)) /* bolt or arrow */
        newObject->number = random_range(20)+1;
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
    if (newObject->plus == 0) {
        newObject->plus = itemplus();
        if (newObject->blessing < 0)
            newObject->plus = -1 - abs(newObject->plus);
        else if (newObject->blessing > 0)
            newObject->plus = 1 + abs(newObject->plus);
    }
}

void make_shield(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMSHIELDS);
    *newObject = Objects[SHIELDID+id];
    if (newObject->plus == 0)
        newObject->plus = itemplus();
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
    if (newObject->blessing < 0)
        newObject->plus = -1 - abs(newObject->plus);
    else if (newObject->blessing > 0)
        newObject->plus = 1 + abs(newObject->plus);
}

void make_armor(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMARMOR);
    *newObject = Objects[ARMORID+id];
    if (newObject->plus == 0) newObject->plus = itemplus();
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
    if (newObject->blessing < 0)
        newObject->plus = -1 - abs(newObject->plus);
    else if (newObject->blessing > 0)
        newObject->plus = 1 + abs(newObject->plus);
}

void make_cloak(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMCLOAKS);
    Objects[OB_CLOAK_PROTECT].plus = 2;
    *newObject = Objects[CLOAKID+id];
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
}

void make_boots(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMBOOTS);
    *newObject = Objects[BOOTID+id];
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
}

void make_stick(Object* newObject, int id)
{
    if (id == -1) id = random_range(NUMSTICKS);
    *newObject = Objects[STICKID+id];
    newObject->charge = itemcharge();
    if (newObject->blessing == 0) newObject->blessing = itemblessing();
}

void make_artifact(Object* newObject, int id)
{
    if (id == -1)
        do
            id = random_range(NUMARTIFACTS);
        while (Objects[id].uniqueness >= UNIQUE_MADE);
    *newObject = Objects[ARTIFACTID+id];
    if ( newObject->id == OB_HOLDING )
        newObject->blessing = itemblessing();
}

/* this function is used to shuffle the id numbers of scrolls, potions, etc */
/* taken from Knuth 2 */
void shuffle(int ids[], int number)
{
    int top, swap, with;

    for (top = 0; top < number; top++)
        ids[top] = top;
    for (top = number - 1; top >= 0; top--) {
        swap = ids[top];
        with = random_range(top + 1);	/* from  0 to top, inclusive */
        ids[top] = ids[with];
        ids[with] = swap;
    }
}

/* item name functions */

const char *scrollname(int id)
{
    switch(scroll_ids[id]) {
    case 0:
        return "scroll-GRISTOGRUE";
    case 1:
        return "scroll-Kho Reck Tighp";
    case 2:
        return "scroll-E Z";
    case 3:
        return "scroll-Kevitz";
    case 4:
        return "scroll-Arcanum Prime";
    case 5:
        return "scroll-NYARLATHOTEP";
    case 6:
        return "scroll-Gilthoniel";
    case 7:
        return "scroll-Zarathustra";
    case 8:
        return "scroll-Ancient Lore";
    case 9:
        return "scroll-Eyes Only";
    case 10:
        return "scroll-Ambogar Empheltz";
    case 11:
        return "scroll-Isengard";
    case 12:
        return "scroll-Deosil Widdershins";
    case 13:
        return "scroll-Magister Paracelsus";
    case 14:
        return "scroll-Qlipphotic Summons";
    case 15:
        return "scroll-Aratron Samael";
    case 16:
        return "scroll-De Wormiis Mysterius";
    case 17:
        return "scroll-Necronomicon";
    case 18:
        return "scroll-Pnakotic Manuscript";
    case 19:
        return "scroll-Codex of Xalimar";
    case 20:
        return "scroll-The Mabinogion";
    case 21:
        return "scroll-Ginseng Shiatsu";
    case 22:
        return "scroll-Tome of Tromax";
    case 23:
        return "scroll-Book of the Dead ";
    case 24:
        return "scroll-The Flame Tongue";
    case 25:
        return "scroll-Karst Khogar";
    case 26:
        return "scroll-The Yellow Sign";
    case 27:
        return "scroll-The Kevillist Manifesto";
    case 28:
        return "scroll-Goshtar Script";
    default:
    case 29:
        return "scroll-Pendragon Encryption";
    }
}

const char *grotname(void)
{
    switch(random_range(21)) {
    case 0:
        return "pot lid";
    case 1:
        return "mound of offal";
    case 2:
        return "sword that was broken";
    case 3:
        return "salted snail";
    case 4:
        return "key";
    case 5:
        return "toadstool";
    case 6:
        return "greenish spindle";
    case 7:
        return "tin soldier";
    case 8:
        return "broken yo-yo";
    case 9:
        return "NYC subway map";
    case 10:
        return "Nixon's the One! button";
    case 11:
        return "beer can (empty)";
    case 12:
        return "golden bejewelled falcon";
    case 13:
        return "hamster cage";
    case 14:
        return "wooden nickel";
    case 15:
        return "three-dollar bill";
    case 16:
        return "rosebud";
    case 17:
        return "water pistol";
    case 18:
        return "shattered skull";
    case 19:
        return "black bag";
    default:
    case 20:
        return "jawbone of an ass";
    }
}




const char *potionname(int id)
{
    switch (potion_ids[id]) {
    case 0:
        return "vial of dewy liquid";
    case 1:
        return "jug of tarry black substance";
    case 2:
        return "flask of cold smoking froth";
    case 3:
        return "phial of glowing fluid";
    case 4:
        return "bottle of sickening slime";
    case 5:
        return "sac of greenish gel";
    case 6:
        return "wineskin of odorous goo";
    case 7:
        return "canteen of sweet sap";
    case 8:
        return "urn of clear fluid";
    case 9:
        return "clotted grey ooze";
    case 10:
        return "keg of bubbly golden fluid";
    case 11:
        return "tube of minty paste";
    case 12:
        return "pitcher of aromatic liquid";
    case 13:
        return "pot of rancid grease";
    case 14:
        return "thermos of hot black liquid";
    case 15:
        return "magnum of deep red liquid";
    case 16:
        return "vase full of ichor";
    case 17:
        return "container of white cream";
    case 18:
        return "syringe of clear fluid";
    default:
    case 19:
        return "can of volatile essence";
    }
}


const char *stickname(int id)
{
    switch (stick_ids[id]) {
    case 0:
        return "oaken staff";
    case 1:
        return "heavy metal rod";
    case 2:
        return "shaft of congealed light";
    case 3:
        return "slender ceramic wand";
    case 4:
        return "rune-inscribed bone wand";
    case 5:
        return "knurly staff";
    case 6:
        return "steel knobbed rod";
    case 7:
        return "lucite wand";
    case 8:
        return "sturdy alpenstock";
    case 9:
        return "gem-studded ebony staff";
    case 10:
        return "chromed sequinned staff";
    case 11:
        return "old peeling stick";
    case 12:
        return "jointed metal rod";
    case 13:
        return "wand with lead ferrules";
    case 14:
        return "forked wooden stick";
    case 15:
        return "cane with gold eagle handle";
    case 16:
        return "crystalline wand";
    case 17:
        return "metal stick with trigger";
    case 18:
        return "leather-handled stone rod";
    default:
    case 19:
        return "tiny mithril wand";
    }
}

const char *ringname(int id)
{
    switch (ring_ids[id]) {
    case 0:
        return "gold ring with a blue gem";
    case 1:
        return "brass ring";
    case 2:
        return "mithril ring with a red gem";
    case 3:
        return "platinum ring";
        break;
    case 4:
        return "gold dragon's head ring";
    case 5:
        return "bronze ring";
    case 6:
        return "aardvark seal ring";
    case 7:
        return "grey metal ring";
    case 8:
        return "silver skull ring";
    case 9:
        return "onyx ring";
    case 10:
        return "Collegium Magii class ring";
    case 11:
        return "worn stone ring";
    case 12:
        return "diorite ring";
    case 13:
        return "ancient scarab ring";
        break;
    case 14:
        return "plastic charm ring";
    case 15:
        return "soapy gypsum ring";
    case 16:
        return "glass ring";
    case 17:
        return "glowing bluestone ring";
    case 18:
        return "ring with eye sigil";
    default:
    case 19:
        return "zirconium ring";
    }
}


const char *cloakname(int id)
{
    switch (cloak_ids[id]) {
    case 0:
        return "tattered piece of cloth";
    case 1:
        return "fuligin cloak";
    case 2:
        return "chintz cloak";
    case 3:
        return "diaphanous cape";
        break;
    case 4:
        return "red half-cloak";
    case 5:
        return "mouse-hide cloak";
    case 6:
        return "kelly green cloak";
    case 7:
        return "cloth-of-gold cloak";
    case 8:
        return "dirty old cloak";
    case 9:
        return "weightless cloak";
    case 10:
        return "boat cloak";
    case 11:
        return "greasy tarpaulin";
    case 12:
        return "sable cloak";
    case 13:
        return "soft velvet cloak";
        break;
    case 14:
        return "opera cape";
    case 15:
        return "elegant brocade cloak";
    case 16:
        return "cloak of many colors";
    case 17:
        return "grey-green rag";
    case 18:
        return "puce and chartreuse cloak";
    default:
    case 19:
        return "smoky cloak";
    }
}

const char *bootname(int id)
{
    switch (boot_ids[id]) {
    case 0:
        return "sturdy leather boots";
    case 1:
        return "calf-length moccasins";
    case 2:
        return "dark-colored tabi";
    case 3:
        return "patent-leather shoes";
        break;
    case 4:
        return "beaten-up gumshoes";
    case 5:
        return "alligator-hide boots";
    case 6:
        return "comfortable sandals";
    case 7:
        return "roller skates";
    case 8:
        return "purple suede gaiters";
    case 9:
        return "mirror-plated wingtips";
    case 10:
        return "heavy workboots";
    case 11:
        return "polyurethane-soled sneakers";
    case 12:
        return "clodhoppers";
    case 13:
        return "wooden shoes";
        break;
    case 14:
        return "ski boots";
    case 15:
        return "hob-nailed boots";
    case 16:
        return "elven boots";
    case 17:
        return "cowboy boots";
    case 18:
        return "flipflop slippers";
    default:
    case 19:
        return "riding boots";
    }
}

int itemplus(void)
{
    int p = 0;

    while (random_range(2) == 0)
        p++;
    return(p);
}



int itemcharge(void)
{
    return(random_range(20)+1);
}



int itemblessing(void)
{
    switch(random_range(10)) {
    case 0:
    case 1:
        return(-1-random_range(10));
    case 8:
    case 9:
        return(1+random_range(10));
    default:
        return(0);
    }
}


int twohandedp(int id)
{
    switch(id) {
    case OB_GREAT_SWORD:
    case OB_GREAT_AXE:
    case OB_QUARTERSTAFF:
    case OB_HALBERD:
    case OB_LONGBOW:
    case OB_CROSSBOW:
    case OB_DESECRATOR:
    case OB_HEWER:
    case OB_GIANT_CLUB:
    case OB_SCYTHE_DEATH:
        return(true);
    default:
        return(false);
    }
}

void item_use(Object* o)
{
    clearmsg();
    switch(o->usef) {
    case -1:
        i_no_op(o);
        break;
    case 0:
        i_nothing(o);
        break;

        /* scrolls */
    case I_SPELLS:
        i_spells(o);
        break;
    case I_BLESS:
        i_bless(o);
        break;
    case I_ACQUIRE:
        i_acquire(o);
        break;
    case I_ENCHANT:
        i_enchant(o);
        break;
    case I_TELEPORT:
        i_teleport(o);
        break;
    case I_WISH:
        i_wish(o);
        break;
    case I_CLAIRVOYANCE:
        i_clairvoyance(o);
        break;
    case I_DISPLACE:
        i_displace(o);
        break;
    case I_ID:
        i_id(o);
        break;
    case I_JANE_T:
        i_jane_t(o);
        break;
    case I_FLUX:
        i_flux(o);
        break;
    case I_WARP:
        i_warp(o);
        break;
    case I_ALERT:
        i_alert(o);
        break;
    case I_CHARGE:
        i_charge(o);
        break;
    case I_KNOWLEDGE:
        i_knowledge(o);
        break;
    case I_LAW:
        i_law(o);
        break;
    case I_HINT:
        hint();
        break;
    case I_HERO:
        i_hero(o);
        break;
    case I_TRUESIGHT:
        i_truesight(o);
        break;
    case I_ILLUMINATE:
        i_illuminate(o);
        break;
    case I_DEFLECT:
        i_deflect(o);
        break;

        /* potion functions */
    case I_HEAL:
        i_heal(o);
        break;
    case I_OBJDET:
        i_objdet(o);
        break;
    case I_MONDET:
        i_mondet(o);
        break;
    case I_SLEEP_SELF:
        i_sleep_self(o);
        break;
    case I_NEUTRALIZE_POISON:
        i_neutralize_poison(o);
        break;
    case I_RESTORE:
        i_restore(o);
        break;
    case I_SPEED:
        i_speed(o);
        break;
    case I_AZOTH:
        i_azoth(o);
        break;
    case I_AUGMENT:
        i_augment(o);
        break;
    case I_REGENERATE:
        i_regenerate(o);
        break;
    case I_INVISIBLE:
        i_invisible(o);
        break;
    case I_BREATHING:
        i_breathing(o);
        break;
    case I_FEAR_RESIST:
        i_fear_resist(o);
        break;
    case I_CHAOS:
        i_chaos(o);
        break;
    case I_ACCURACY:
        i_accuracy(o);
        break;
    case I_LEVITATION:
        i_levitate(o);
        break;
    case I_CURE:
        i_cure(o);
        break;

        /* stick functions */
    case I_FIREBOLT:
        i_firebolt(o);
        break;
    case I_LBOLT:
        i_lbolt(o);
        break;
    case I_MISSILE:
        i_missile(o);
        break;
    case I_SLEEP_OTHER:
        i_sleep_other(o);
        break;
    case I_FIREBALL:
        i_fireball(o);
        break;
    case I_LBALL:
        i_lball(o);
        break;
    case I_SNOWBALL:
        i_snowball(o);
        break;
    case I_SUMMON:
        i_summon(o);
        break;
    case I_HIDE:
        i_hide(o);
        break;
    case I_DISRUPT:
        i_disrupt(o);
        break;
    case I_DISINTEGRATE:
        i_disintegrate(o);
        break;
    case I_APPORT:
        i_apport(o);
        break;
    case I_DISPEL:
        i_dispel(o);
        break;
    case I_POLYMORPH:
        i_polymorph(o);
        break;
    case I_FEAR:
        i_fear(o);
        break;

        /* food functions */
    case I_FOOD:
        o->use();
        break;
    case I_LEMBAS:
        i_lembas(o);
        break;
    case I_STIM:
        i_stim(o);
        break;
    case I_POW:
        i_pow(o);
        break;
    case I_IMMUNE:
        i_immune(o);
        break;
    case I_POISON_FOOD:
        i_poison_food(o);
        break;
    case I_CORPSE:
        i_corpse(o);
        break;
    case I_PEPPER_FOOD:
        i_pepper_food(o);
        break;

        /* boots functions */
    case I_PERM_SPEED:
        i_perm_speed(o);
        break;
    case I_PERM_HERO:
        i_perm_hero(o);
        break;
    case I_PERM_LEVITATE:
        i_perm_levitate(o);
        break;
    case I_PERM_AGILITY:
        i_perm_agility(o);
        break;

        /* artifact functions */
    case I_SCEPTRE:
        i_sceptre(o);
        break;
    case I_PLANES:
        i_planes(o);
        break;
    case I_SERENITY:
        i_serenity(o);
        break;
    case I_STARGEM:
        i_stargem(o);
        break;
    case I_SYMBOL:
        i_symbol(o);
        break;
    case I_ORBMASTERY:
        i_orbmastery(o);
        break;
    case I_ORBFIRE:
        i_orbfire(o);
        break;
    case I_ORBWATER:
        i_orbwater(o);
        break;
    case I_ORBEARTH:
        i_orbearth(o);
        break;
    case I_ORBAIR:
        i_orbair(o);
        break;
    case I_ORBDEAD:
        i_orbdead(o);
        break;
    case I_CRYSTAL:
        i_crystal(o);
        break;
    case I_LIFE:
        i_life(o);
        break;
    case I_DEATH:
        i_death(o);
        break;
    case I_ANTIOCH:
        i_antioch(o);
        break;
    case I_HELM:
        i_helm(o);
        break;
    case I_KOLWYNIA:
        i_kolwynia(o);
        break;
    case I_ENCHANTMENT:
        i_enchantment(o);
        break;
    case I_JUGGERNAUT:
        i_juggernaut(o);
        break;
    case I_HOLDING:
        i_holding(o);
        break;

        /* cloak functions */
    case I_PERM_DISPLACE:
        i_perm_displace(o);
        break;
    case I_PERM_NEGIMMUNE:
        i_perm_negimmune(o);
        break;
    case I_PERM_INVISIBLE:
        i_perm_invisible(o);
        break;
    case I_PERM_PROTECTION:
        i_perm_protection(o);
        break;
    case I_PERM_ACCURACY:
        i_perm_accuracy(o);
        break;
    case I_PERM_TRUESIGHT:
        i_perm_truesight(o);
        break;

        /* ring functions */
    case I_PERM_BURDEN:
        i_perm_burden(o);
        break;
    case I_PERM_STRENGTH:
        i_perm_strength(o);
        break;
    case I_PERM_GAZE_IMMUNE:
        i_perm_gaze_immune(o);
        break;
    case I_PERM_FIRE_RESIST:
        i_perm_fire_resist(o);
        break;
    case I_PERM_POISON_RESIST:
        i_perm_poison_resist(o);
        break;
    case I_PERM_REGENERATE:
        i_perm_regenerate(o);
        break;
    case I_PERM_KNOWLEDGE:
        i_perm_knowledge(o);
        break;

        /* armor functions */
    case I_NORMAL_ARMOR:
        i_normal_armor(o);
        break;
    case I_PERM_FEAR_RESIST:
        i_perm_fear_resist(o);
        break;
    case I_PERM_ENERGY_RESIST:
        i_perm_energy_resist(o);
        break;
    case I_PERM_BREATHING:
        i_perm_breathing(o);
        break;

        /* weapons functions */
    case I_NORMAL_WEAPON:
        i_normal_weapon(o);
        break;
    case I_LIGHTSABRE:
        i_lightsabre(o);
        break;
    case I_DEMONBLADE:
        i_demonblade(o);
        break;
    case I_DESECRATE:
        i_desecrate(o);
        break;
    case I_MACE_DISRUPT:
        i_mace_disrupt(o);
        break;
    case I_DEFEND:
        i_defend(o);
        break;
    case I_VICTRIX:
        i_victrix(o);
        break;

        /* thing functions */
    case I_PICK:
        i_pick(o);
        break;
    case I_KEY:
        i_key(o);
        break;
    case I_PERM_ILLUMINATE:
        i_perm_illuminate(o);
        break;
    case I_TRAP:
        i_trap(o);
        break;
    case I_RAISE_PORTCULLIS:
        i_raise_portcullis(o);
        break;

        /* shield functions */
    case I_NORMAL_SHIELD:
        i_normal_shield(o);
        break;
    case I_PERM_DEFLECT:
        i_perm_deflect(o);
        break;
#ifdef DEBUG /* WDT: good idea, DG.  I'll be using this often, I predict! */
        /* looking for objects without, or with unimplemented, functions */
    default:
        fprintf(DG_debug_log, "tried to use a %s with o->usef %d\n",
                itemid(o), o->usef);
        break;
#endif
    }
}

