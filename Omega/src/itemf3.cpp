/* omega copyright (C) 1987,1988,1989 by Laurence Raphael Brothers */
/* itemf3.c */

/* mostly artifact and stick functions */

#include "glob.h"

/* amulet of the planes */
void i_planes(Object* o)
{
    if (Player.mana < 1) print1("The amulet spits some multicolored sparks.");
    else {
        print1("You focus mana into the amulet....");
        Player.mana = max(0,Player.mana-100);
        dataprint();
        morewait();
        strategic_teleport(1);
    }
}

/* amulet of serenity */
void i_serenity(Object* o)
{
    if (Player.mana < 1) print1("The amulet emits a dull ring.");
    /* should the amulet drop_at a real ring? */
    else {
        print1("You focus your will with the amulet....");
        Player.mana = max(0,Player.mana-10);
        Player.maxhp -= 1;
        Player.hp = Player.maxhp;
        dataprint();
        morewait();

        s_restore();
        s_cure();
        s_breathe();
        s_alert();
        /* artifacts should have a unique function... PGM */
    }
}

/* the sceptre of high magic */
void i_sceptre(Object* o)
{
    if (HiMagicUse == Date)
        print1("The Sceptre makes a sort of dull 'thut' noise.");
    else if (Current_Environment == E_CIRCLE || Current_Environment == E_ASTRAL)
    {
        HiMagicUse = Date; /* WDT: this looks like it's a good place to use
                        * the batteries. */
        print1("The Sceptre warps strangely for a second, and then subsides.");
        morewait();
        print2("You smell ozone."); /* WDT: explain the battery use. */
    }
    else {
        HiMagicUse = Date;
        print1("With a shriek of tearing aether, a magic portal appears!");
        print2("Step through? [yn] ");
        if (ynq()=='y') change_environment(E_COURT);
        print1("The sceptre seems to subside. You hear a high whine, as of");
        print2("capacitors beginning to recharge.");
        morewait();
    }
}


/* the star gem */
void i_stargem(Object* o)
{
    if (StarGemUse == Date) {
        print1("The Star Gem glints weakly as if to say:");
        print2("'You have used me overmuch.'");
        print3("and it vanishes a puff of regret.");
        Objects[o->id].uniqueness = UNIQUE_UNMADE;
        /* it's now out there, somewhere */
        dispose_lost_objects(1,o);
    }
    else {
        StarGemUse = Date;
        if (o->blessing < 1) {
            print1("The Star Gem shines brightly and emits a musical tone.");
            print2("You see a dark cloud roil away from it.");
            morewait();
            o->blessing = 10;
        }
        print1("The star gem flares with golden light!");
        morewait();
        if (Player.status[ILLUMINATION] < 1000) {
            print1("Interesting, you seem to be permanently accompanied");
            print2("by a friendly lambent glow....");
            morewait();
            Player.status[ILLUMINATION] = 1500;
        }
        print1("You suddenly find yourself whisked away by some unknown force!");
        morewait();
        setgamestatus(COMPLETED_ASTRAL);
        change_environment(E_COUNTRYSIDE);
        Player.x = 61;
        Player.y = 3;
        screencheck(Player.x,Player.y);
        drawvision(Player.x,Player.y);
        locprint("Star Peak");
        Country[Player.x][Player.y].current_terrain_type =
            Country[Player.x][Player.y].base_terrain_type;
        c_set(Player.x, Player.y, CHANGED);
        print2("The Star Gem's brilliance seems to fade.");
    }
}


/* wand of fear */
void i_fear(Object* o)
{
    int x=Player.x,y=Player.y;
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    inflict_fear(x,y);
}


void i_juggernaut(Object* o)
{
    int d,x=Player.x,y=Player.y;
    int seen = 1, not_seen = 0;
    int tunneled = 0;

    print1("You activate the Juggernaut of Karnak!");
    if (! o->known) {
        print2("Uh, oh, it's coming this way!");
        p_death("the Juggernaut of Karnak");
    }
    else {
        d = getdir();
        if (d == ABORT)
            print2("You deactivate the Juggernaut before it escapes.");
        else {
            print1("Vroom! ");
            while (inbounds(x+Dirs[0][d],y+Dirs[1][d])) {
                x+=Dirs[0][d];
                y+=Dirs[1][d];
                if (!view_unblocked(x, y) || offscreen(x,y))
                    seen = 0;
                if (Level->site[x][y].locchar == WALL)
                    tunneled++;
                if (Level->site[x][y].locchar != WATER &&
                        Level->site[x][y].locchar != VOID_CHAR &&
                        Level->site[x][y].locchar != ABYSS &&
                        Level->site[x][y].locchar != SPACE &&
                        Level->site[x][y].locchar != LAVA)
                {
                    Level->site[x][y].locchar = FLOOR;
                    Level->site[x][y].p_locf = L_NO_OP;
                }
                lreset(x, y, SECRET);
                lset(x, y, CHANGED);
                if (Level->site[x][y].creature != NULL) {
                    if (seen)
                        nprint1("Splat! ");
                    else
                        not_seen++;
                    setgamestatus(SUPPRESS_PRINTING);
                    Level->site[x][y].creature->m_death();
                    resetgamestatus(SUPPRESS_PRINTING);
                }
                plotspot(x, y, false);
                omshowcursor(x, y);
            }
            if (not_seen > 6)
                print2("You hear many distant screams...");
            else if (not_seen > 3)
                print2("You hear several distant screams...");
            else if (not_seen > 1)
                print2("You hear a couple of distant screams...");
            else if (not_seen == 1)
                print2("You hear a distant scream...");
            gain_experience(1000);
            dispose_lost_objects(1,o);
            Level->tunnelled += tunneled - 1;
            tunnelcheck();
        }
    }
}


void i_symbol(Object* o)
{
    int i;
    if (! o->known)
        print1("Nothing seems to happen.");
    /* if o->charge != 17, then symbol was stolen from own high priest! */
    else if ((o->aux != Player.patron) || (o->charge != 17)) {
        print1("You invoke the deity...");
        print2("...who for some reason seems rather annoyed at you...");
        print3("You are enveloped in Godsfire!");
        morewait();
        for(; Player.hp>1; Player.hp--)
            dataprint();
        morewait();
        for(i=0; i<MAXITEMS; i++)
            if (Player.possessions[i] != NULL)
                dispose_lost_objects(Player.possessions[i]->number,
                                     Player.possessions[i]);
        Player.mana = 0;
        dataprint();
    }
    else if ( (SymbolUseDay == day() ) && (SymbolUseHour == hour())) {
        print1("Your deity frowns upon this profligate use of power...");
        print2("Shazam! A bolt of Godsfire! Your symbol shatters!");
        dispose_lost_objects(1,o);
        Player.hp = 1;
        dataprint();
    }
    else {
        print1("A mystic flow of theurgic energy courses through your body!");
        SymbolUseHour = hour();
        SymbolUseDay = day();
        cleanse(1);
        heal(10);
        Player.mana = max(Player.mana,calcmana());
        dataprint();
    }
}


void i_crystal(Object* o)
{
    if (!o->known) print1("You can't figure out how to activate this orb.");
    else {
        print1("You gaze into your crystal ball.");
        if ( (ViewDay == day()) && (ViewHour == hour())) print2("All you get is Gilligan's Island reruns.");
        else if ((o->blessing<0) || (Player.iq+Player.level < random_range(30))) {
            ViewHour = hour();
            ViewDay = day();
            print2("Weird interference patterns from the crystal fog your mind....");
            amnesia();
        }
        else {
            ViewHour = hour();
            ViewDay = day();
            print2("You sense the presence of life...");
            mondet(1);
            morewait();
            print2("You sense the presence of objects...");
            objdet(1);
            morewait();
            print2("You begin to see visions of things beyond your ken....");
            hint();
        }
    }
}

void i_antioch(Object* o)
{
    int x=Player.x,y=Player.y;
    int count;
    if (!o->known) {
        print1("Ka-Boom!");
        print2("You seem to have annihilated yourself.");
        p_death("the Holy Hand-Grenade of Antioch");
    }
    else {
        print1("Bring out the Holy Hand-Grenade of Antioch!");
        setspot(&x,&y);
        print2("Ok, you pull the pin.....");
        morewait();
        print1("What do you count up to? ");
        count = (int) parsenum("");
        if ((count < 3)&&(Level->site[x][y].creature!=NULL)) {
            print1("`Three shall be the number of thy counting....");
            print2("And the number of thy counting shall be three.'");
            print3("Your target picks up the grenade and throws it back!");
            morewait();
            clearmsg();
            print1("Ka-Boom!");
            p_death("the Holy Hand-Grenade of Antioch");
        }
        else if (count>3) {
            print1("`Three shall be the number of thy counting.");
            print2("And the number of thy counting shall be three.'");
            morewait();
            clearmsg();
            print1("Ka-Boom!");
            p_death("the Holy Hand-Grenade of Antioch");
        }
        else {
            print1("Ka-Boom!");
            gain_experience(1000);
            Level->site[x][y].locchar = TRAP;
            Level->site[x][y].p_locf = L_TRAP_DOOR;
            lset(x, y, CHANGED);
            if (Level->site[x][y].creature != NULL) {
                Level->site[x][y].creature->m_death();
                print2("You are covered with gore.");
            }
            Level->site[x][y].things = NULL;
        }
    }
    dispose_lost_objects(1,o);
}

void i_kolwynia(Object* o)
{
    int i;
    if (! o->known) {
        print1("You destroy youself with a mana storm. How sad.");
        p_death("Kolwynia, The Key That Was Lost");
    }
    else {
        gain_experience(5000);
        print1("You seem to have gained complete mastery of magic.");
        Player.maxpow *= 2;
        Player.pow = max( Player.pow, Player.maxpow );
        for(i=0; i<NUMSPELLS; i++)
            Spells[i].known = true;
    }
    dispose_lost_objects(1,o);
}

void i_enchantment(Object* o)
{
    char response;
    if (ZapHour == hour())
        print1("The staff doesn't seem to have recharged yet.");
    else if (! o->known) {
        ZapHour = hour();
        print1("You blast the staff backwards....");
        dispel(-1);
    }
    else {
        ZapHour = hour();
        print1("Zap with white or black end [wb] ");
        do response = (char) mcigetc();
        while ((response != 'w') && (response != 'b'));
        print2("The staff discharges!");
        if (response == 'w') enchant(o->blessing*2+1);
        else dispel(o->blessing);
    }
}

void i_helm(Object* o)
{
    if ( (HelmDay == day()) && ((HelmHour == hour()) ))
        print1("The helm doesn't seem to have recharged yet.");
    else if (! o->known) {
        HelmHour = hour();
        HelmDay = day();
        print1("You put the helm on backwards....");
        p_teleport(-1);
    }
    else {
        HelmHour = hour();
        print1("Your environment fades.... and rematerializes.");
        p_teleport(o->blessing);
    }
}


void i_death(Object* o)
{
    clearmsg();
    print1("Bad move...");
    p_death("the Potion of Death");
}

void i_life(Object* o)
{
    clearmsg();
    print1("Good move.");
    Player.maxhp = 2 * Player.maxhp;
    Player.hp = max( Player.hp, Player.maxhp );
    dispose_lost_objects(1,o);
}


/* f = fire, w = water, e = earth, a = air, m = mastery */
int orbcheck(char element)
{
    char response;
    print1("The orb begins to glow with increasing intensity!");
    print2("You have the feeling you need to do something more....");
    morewait();
    print1("Burn it in fire [f] ");
    print2("Douse it with water [w] ");
    morewait();
    print1("Smash it against the earth [e] ");
    print2("Toss is through the air [a] ");
    morewait();
    print1("Mix the above actions, doing them in sequence [m] ");
    do {
        print2("Which one [f,w,e,a,m] ");
        response = (char) mcigetc();
    } while ((response != 'f') &&
             (response != 'w') &&
             (response != 'e') &&
             (response != 'a') &&
             (response != 'm'));
    return(response == element);
}

/* orb functions */
void i_orbfire(Object* o)
{
    if (! orbcheck('f')) {
        print1("Bad choice!");
        print2("The Orb of Fire blasts you!");
        fball(Player.x,Player.y,Player.x,Player.y,250);
        o->known = 1;
    }
    else {
        print1("The Orb of Fire flares a brilliant red!");
        Spells[S_FIREBOLT].known = true;
        gain_experience(10000);
        Player.immunity[FLAME]+=100;
        print2("You feel fiery!");
        o->plus = 100;
        o->blessing = 100;
        i_firebolt(o);
    }
    *o = Objects[OB_DEAD_ORB];
}


void i_orbwater(Object* o)
{
    if (! orbcheck('w')) {
        print1("A serious mistake!");
        print2("The Orb of Water blasts you!");
        heal(-250);
        o->known = 1;
    }
    else {
        print1("The Orb of Water pulses a deep green!");
        Spells[S_DISRUPT].known = true;
        Player.immunity[POISON]+=100;
        gain_experience(10000);
        print2("You feel wet!");
        o->plus = 100;
        o->blessing = 100;
        i_disrupt(o);
    }
    *o = Objects[OB_DEAD_ORB];
}



void i_orbearth(Object* o)
{
    int i;
    if (! orbcheck('e')) {
        print1("What a maroon!");
        print2("The Orb of Earth blasts you!");
        Player.con -= 10;
        if (Player.con < 3)
            p_death("congestive heart failure");
        else {
            print3("Your possessions disintegrate!");
            for (i=0; i<MAXITEMS; i++)
                if (Player.possessions[i] != NULL)
                    dispose_lost_objects(Player.possessions[i]->number,
                                         Player.possessions[i]);
            for (i=0; i<MAXPACK; i++)
                if (Player.pack[i] != NULL) {
                    free((char *) Player.pack[i]);
                    Player.pack[i] = NULL;
                }
            Player.packptr = 0;
            o->known = 1;
        }
    }
    else {
        print1("The Orb of Earth emanates a brownish aura!");
        Spells[S_DISINTEGRATE].known = true;
        gain_experience(10000);
        Player.immunity[NEGENERGY]+=100;
        print2("You feel earthy!");
        o->plus = 100;
        o->blessing = 100;
        i_disintegrate(o);
    }
    *o = Objects[OB_DEAD_ORB];
}


void i_orbair(Object* o)
{
    if (! orbcheck('a')) {
        print1("You lose!");
        print2("The Orb of Air blasts you!");
        lball(Player.x,Player.y,Player.x,Player.y,100);
        o->known = 1;
    }
    else {
        print1("The Orb of Air flashes blue!");
        Spells[S_LBALL].known = true; /* lball */
        gain_experience(10000);
        print2("You feel airy!");
        Player.immunity[ELECTRICITY]+=100;
        o->plus = 100;
        o->blessing = 100;
        i_invisible(o);
        i_lbolt(o);
    }
    *o = Objects[OB_DEAD_ORB];
}


void i_orbmastery(Object* o)
{

    if (! orbcheck('m')) {
        print1("A fatal error!");
        print2("The Orb of Mastery blasts you to cinders!");
        p_death("playing with the Orb of Mastery");
        o->known = 1;
    }
    else if ((find_and_remove_item(OB_ORB_FIRE,-1)) &&
             (find_and_remove_item(OB_ORB_WATER,-1)) &&
             (find_and_remove_item(OB_ORB_EARTH,-1)) &&
             (find_and_remove_item(OB_ORB_AIR,-1))) {
        print1("The Orb of Mastery radiates rainbow colors!");
        print2("You feel godlike.");
        Player.iq  = max( Player.iq,  Player.maxiq  = 2 * Player.maxiq);
        Player.pow = max( Player.pow, Player.maxpow = 2 * Player.maxpow);
        Player.str = max( Player.str, Player.maxstr = 2 * Player.maxstr);
        Player.dex = max( Player.dex, Player.maxdex = 2 * Player.maxdex);
        Player.con = max( Player.con, Player.maxcon = 2 * Player.maxcon);
        Player.agi = max( Player.agi, Player.maxagi = 2 * Player.maxagi);
        dataprint();
        morewait();
        print1("You have been imbued with a cosmic power....");
        morewait();
        wish(1);
        morewait();
        clearmsg();
        print2("You feel much more experienced.");
        gain_experience(20000);
        *o = Objects[OB_DEAD_ORB];
    }
    else {
        print1("The Orb of Mastery's power is unbalanced!");
        print2("The Orb of Mastery blasts you to cinders!");
        p_death("playing with the Orb of Mastery");
    }
}


void i_orbdead(Object* o)
{
    int i;
    print1("The burnt-out orb drains all your energy!");
    for(i=0; i<NUMSPELLS; i++)
        Spells[i].known = false;
    print2("You feel not at all like a mage.");
    for(i=0; i<MAXITEMS; i++) {
        if (Player.possessions[i] != NULL) {
            Player.possessions[i]->plus = 0;
            if (Player.possessions[i]->usef > 100)
                Player.possessions[i]->usef = I_NOTHING;
        }
    }
    print3("A storm of mundanity surounds you!");
    level_drain(Player.level-1,"a Burnt-out Orb");
    Player.mana = 0;
    Player.pow -= 10;
}



void i_dispel(Object* o)
{
    dispel((o->blessing > -1) ? o->blessing+random_range(3): o->blessing);
}

/* DAG implement bag of holding */
void i_holding(Object* o)
{
    char c, c1, c2;
    pol ol;

    /* use aux to represent # items in bag, 5 max */
    if (! o->known )
    {
        clearmsg();
        print1("You open the bag and look in.  It looks bigger on the inside");
        print2("than on the outside.  Wow, a Bag of Holding.");
        morewait();
        o->known = 1;
        Objects[o->id].known = 1;
    }
    menuclear();
    if ( ! o->aux )
        menuprint("You bag of holding is empty.\n");
    else
    {
        sprintf(Str1, "You bag of holding currently holds %d items.\n", o->aux );
        menuprint(Str1);
    }
    menuprint(" a: list contents\n");
    menuprint(" b: put an item into the bag\n");
    menuprint(" c: remove an item from the bag\n");
    menuprint(" ESCAPE:cancel\n" );
    showmenu();
    do
        c = (char) mgetc();
    while ( (c < 'a' || c > 'c') && c != ESCAPE);
    switch(c)
    {
    case 'a':
        if (!o->aux)
            print1("The bag is empty.");
        else
        {
            ol = Bagitems;
            menuclear();
            while (ol != NULL) {
                menuprint("\n");
                menuprint(itemid(ol->thing));
                ol = ol->next;
            }
            showmenu();
            morewait();
            xredraw();
        }
        break;
    case 'b':
        if (o->aux >= 5)
            print1("The bag appears to be full.");
        else
        {
            int i;
            Object* ob;
            i = getitem( NULL_ITEM );
            if (i != ABORT )
            {
                ob = Player.possessions[i];
                if ( ob->used && (ob->blessing < 0) )
                {
                    sprintf(Str1, "You can't seem to get rid of: %s", itemid(ob) );
                    print1(Str1);
                } else if (ob == o)  /* put bag into itself */
                {
                    mprint("You put the bottom of the bag into the top.");
                    mprint("The bag starts to slowly get sucked into the bag.");
                    mprint("You hear a high-pitched screaming sound.");
                    mprint("The world starts to twist around you.");
                    mprint("Suddenly the space-time continuum rips...");
                    mprint("And you are sucked into the rift.");
                    free_objlist( Bagitems );
                    Bagitems = NULL;
                    /* Bag is not destroyed, could re-appear anywhere */
                    Objects[o->id].uniqueness = UNIQUE_UNMADE;
                    free_obj( o, true );
                    Player.possessions[i] = NULL;
                    strategic_teleport(-1);
                } else
                {
                    ol = (pol) checkmalloc( sizeof(oltype) );
                    ol->thing = ob;
                    ol->next = Bagitems;
                    Bagitems = ol;
                    conform_unused_object(ob); /* if used, unuses it */
                    Player.possessions[i] = NULL;
                    o->aux++;
                }
            } /* !ABORT */
        } /* not full (else) */
        break;
    case 'c':
        if (!o->aux)
            print1("The bag is empty.");
        else
        {
            if (o->blessing < 0)
            {
                o->known = 2;
                print1("Oops, when you go to remove the objects, you find that they aren't there.");
                print2("It appears that this was actually a Bag of Devouring.");
                morewait();
                print2("You sense an aura of satisfaction around the bag.");
                o->aux = 0;
                free_objlist( Bagitems );
                Bagitems = NULL;
            }
            else
            {
                ol = Bagitems;
                menuclear();
                menuprint("Select object from bag:\n");
                c1 = 'a';
                while (ol != NULL) {
                    sprintf(Str1, "%c: %s\n", c1, itemid(ol->thing) );
                    menuprint(Str1);
                    ol = ol->next;
                    c1++;
                }
                menuprint("ESCAPE: to cancel\n");
                showmenu();
                do
                    c = (char) mgetc();
                while ( (c < 'a' || c >= c1) && c != ESCAPE);
                if (c != ESCAPE)
                {
                    pol prev = NULL;
                    ol = Bagitems;
                    for (c2='a'; c2 < c;  c2++ )
                    {
                        prev = ol;
                        ol = ol->next;
                    }
                    gain_item( ol->thing );
                    o->aux--;
                    /* free appropriately */
                    /* WDT: oops, the old assignment bug struck again.  Fixed, I hope. */
                    if (ol == Bagitems)
                    {
                        ol=ol->next;
                        free( Bagitems );
                        Bagitems = ol;
                    }
                    else
                    {
                        ol = ol->next;
                        free( prev->next );
                        prev->next = ol;
                    }

                } /* not escape */

            } /* else blessing >= 0 */
        } /* else bag is full */
        break;
    } /* switch (c) */

    calc_melee();
    xredraw();
} /* fn */


/* stick functions */


/* wand of apportation */
void i_apport(Object* o)
{
    o->known = max(1,o->known);
    Objects[o->id].known = 1;
    apport(o->blessing);
}

/* staff of firebolts */
void i_firebolt(Object* o)
{
    int x=Player.x,y=Player.y;
    o->known = max(1,o->known);
    Objects[o->id].known = 1;
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    fbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,75);
}


void i_disintegrate(Object* o)
{
    int x=Player.x,y=Player.y;
    o->known = max(1,o->known);
    Objects[o->id].known = 1;
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    disintegrate(x,y);
}

void i_disrupt(Object* o)
{
    int x=Player.x,y=Player.y;
    o->known = max(1,o->known);
    Objects[o->id].known = 1;
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    disrupt(x,y,100);
}


/* staff of lightning bolts */
void i_lbolt(Object* o)
{
    int x=Player.x,y=Player.y;
    o->known = max(1,o->known);
    Objects[o->id].known = 1;
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    lbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,75);
}

/* wand of magic missiles */
void i_missile(Object* o)
{
    int x=Player.x,y=Player.y;
    o->known = max(1,o->known);
    Objects[o->id].known = 1;
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    nbolt(Player.x,Player.y,x,y,Player.dex*2+Player.level,20);
}

/* wand of fire balls */
void i_fireball(Object* o)
{
    int x=Player.x,y=Player.y;
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    fball(Player.x,Player.y,x,y,35);
}


/* wand of snowballs */
void i_snowball(Object* o)
{
    int x=Player.x,y=Player.y;
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    snowball(Player.x,Player.y,x,y,20);
}

/* wand of lightning balls */
void i_lball(Object* o)
{
    int x=Player.x,y=Player.y;
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    setspot(&x,&y);
    if (o->blessing < 0) {
        x = Player.x;
        y = Player.y;
    }
    lball(Player.x,Player.y,x,y,50);
}

/* staff of sleep */
void i_sleep_other(Object* o)
{
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    sleep_monster(o->blessing);
}

/* rod of summoning */
/* rod of summoning now always summons as if cursed */
void i_summon(Object* o)
{
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    summon(-1,-1);
}

void i_hide(Object* o)
{
    int x=Player.x,y=Player.y;
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    setspot(&x,&y);
    hide(x,y);
}

void i_polymorph(Object* o)
{
    Objects[o->id].known = 1;
    o->known = max(1,o->known);
    polymorph(o->blessing);
}
