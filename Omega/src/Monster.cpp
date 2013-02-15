#include "glob.h"

/* like m_normal_move, but can open doors */
void Monster::m_smart_move()
{
    m_simple_move();
}

/* not very smart, but not altogether stupid movement */
void Monster::m_normal_move()
{
    m_simple_move();
}

/* used by both m_normal_move and m_smart_move */
void Monster::m_simple_move()
{
    int dx = sign(Player.x - this->x);
    int dy = sign(Player.y - this->y);

    erase_monster(this);
    if (this->hp < Monsters[this->id].hp/4) {
        dx = - dx;
        dy = - dy;
        this->movef = M_MOVE_SCAREDY;
        if (this->uniqueness == COMMON) {
            strcpy(Str2,"The ");
            strcat(Str2,this->name);
        }
        else strcpy(Str2,this->name);
        if (this->possessions != NULL) {
            strcat(Str2," drops its treasure and flees!");
            this->m_dropstuff();
        }
        else strcat(Str2," flees!");
        mprint(Str2);
        this->speed = min(2,this->speed-1);
    }
    if ((! m_statusp(this,HOSTILE) && !m_statusp(this, NEEDY)) ||
            (Player.status[INVISIBLE] > 0)) m_random_move();
    else {
        if (m_unblocked(this,this->x+dx,this->y+dy))
            this->movemonster(this->x+dx,this->y+dy);
        else if (dx == 0) {
            if (m_unblocked(this,this->x+1,this->y+dy))
                this->movemonster(this->x+1,this->y+dy);
            else if (m_unblocked(this,this->x-1,this->y+dy))
                this->movemonster(this->x-1,this->y+dy);
        }

        else if (dy == 0) {
            if (m_unblocked(this,this->x+dx,this->y+1))
                this->movemonster(this->x+dx,this->y+1);
            else if (m_unblocked(this,this->x+dx,this->y-1))
                this->movemonster(this->x+dx,this->y-1);
        }

        else {
            if (m_unblocked(this,this->x+dx,this->y))
                this->movemonster(this->x+dx,this->y);
            else if (m_unblocked(this,this->x,this->y+dy))
                this->movemonster(this->x,this->y+dy);
        }
    }
}

void Monster::m_move_animal()
{
    if (m_statusp(this,HOSTILE))
        m_normal_move();
    else m_scaredy_move();
}

/* same as simple move except run in opposite direction */
void Monster::m_scaredy_move()
{
    int dx = -sign(Player.x - this->x);
    int dy = -sign(Player.y - this->y);
    erase_monster(this);
    if (Player.status[INVISIBLE]) m_random_move();
    else {
        if (m_unblocked(this,this->x+dx,this->y+dy))
            this->movemonster(this->x+dx,this->y+dy);
        else if (dx == 0) {
            if (m_unblocked(this,this->x+1,this->y+dy))
                this->movemonster(this->x+1,this->y+dy);
            else if (m_unblocked(this,this->x-1,this->y+dy))
                this->movemonster(this->x-1,this->y+dy);
        }

        else if (dy == 0) {
            if (m_unblocked(this,this->x+dx,this->y+1))
                this->movemonster(this->x+dx,this->y+1);
            else if (m_unblocked(this,this->x+dx,this->y-1))
                this->movemonster(this->x+dx,this->y-1);
        }

        else {
            if (m_unblocked(this,this->x+dx,this->y))
                this->movemonster(this->x+dx,this->y);
            else if (m_unblocked(this,this->x,this->y+dy))
                this->movemonster(this->x,this->y+dy);
        }
    }
}

/* for spirits (and earth creatures) who can ignore blockages because
   either they are noncorporeal or they can move through stone */
void Monster::m_spirit_move()
{
    int dx = sign(Player.x - this->x);
    int dy = sign(Player.y - this->y);
    erase_monster(this);
    if (this->hp < Monsters[this->id].hp/6) {
        dx = -dx;
        dy = -dy;
    }

    if (Player.status[INVISIBLE] > 0 || !m_unblocked(this, this->x+dx, this->y+dy))
        m_random_move();
    else
        this->movemonster(this->x+dx,this->y+dy);
}

/* fluttery dumb movement */
void Monster::m_flutter_move()
{
    int trange,range = distance(this->x,this->y,Player.x,Player.y);
    int i,tx,ty,nx=this->x,ny=this->y;
    erase_monster(this);
    if (Player.status[INVISIBLE] > 0) m_random_move();
    else {
        for (i=0; i<8; i++) {
            tx = this->x+Dirs[0][i];
            ty = this->y+Dirs[1][i];
            trange = distance(tx,ty,Player.x,Player.y);
            if (this->hp < Monsters[this->id].hp/6) {
                if ((trange > range) && m_unblocked(this,tx,ty)) {
                    range = trange;
                    nx = tx;
                    ny = ty;
                }
            }
            else if ((trange <= range) && m_unblocked(this,tx,ty)) {
                range = trange;
                nx = tx;
                ny = ty;
            }
        }
        this->movemonster(nx,ny);
    }
}

void Monster::m_follow_move()
{
    if (! m_statusp(this,HOSTILE))
        m_normal_move();
    else m_scaredy_move();
}

/* allows monsters to fall into pools, revealed traps, etc */
void Monster::m_confused_move()
{
    int i,nx,ny,done=false;
    erase_monster(this);
    for (i=0; ((i<8)&&(! done)); i++) {
        nx = this->x+random_range(3)-1;
        ny = this->y+random_range(3)-1;
        if (unblocked(nx,ny) &&
                ((nx != Player.x) ||
                 (ny != Player.y))) {
            done = true;
            this->movemonster(nx,ny);
        }
    }
}

void Monster::m_random_move()
{
    int i,nx,ny,done=false;
    erase_monster(this);
    for (i=0; ((i<8)&&(! done)); i++) {
        nx = this->x+random_range(3)-1;
        ny = this->y+random_range(3)-1;
        if (m_unblocked(this,nx,ny) &&
                ((nx != Player.x) ||
                 (ny != Player.y))) {
            done = true;
            this->movemonster(nx,ny);
        }
    }
}

/* monster removed from play */
void Monster::m_vanish()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    strcat(Str2," vanishes in the twinkling of an eye!");
    mprint(Str2);
    this->m_remove();/* signals "death" -- no credit to player, though */
}

/* monster still in play */
void Monster::m_teleport()
{
    erase_monster(this);
    if (m_statusp(this,AWAKE)) {
        Level->site[this->x][this->y].creature = NULL;
        putspot(this->x,this->y,getspot(this->x,this->y,false));
        findspace(&(this->x),&(this->y),-1);
        Level->site[this->x][this->y].creature = this;
    }
}

void Monster::m_move_leash()
{
    m_simple_move();
    if (this->aux1 == 0) {
        this->aux1 = this->x;
        this->aux2 = this->y;
    }
    else if (distance(this->x,this->y,this->aux1,this->aux2) > 5) {
        if (Level->site[this->aux1][this->aux2].creature != NULL) {
            if (los_p(Player.x,Player.y,this->aux1,this->aux2)) {
                /* some other monster is where the chain starts */
                if (Level->site[this->aux1][this->aux2].creature->uniqueness == COMMON) {
                    strcpy(Str1, "The ");
                    strcat(Str1, Level->site[this->aux1][this->aux2].creature->name);
                }
                else
                    strcpy(Str1, Level->site[this->aux1][this->aux2].creature->name);
                strcat(Str1, " releases the dog's chain!");
                mprint(Str1);
            }
            this->movef = M_MOVE_NORMAL;
            /* otherwise, we'd lose either the dog or the other monster. */
        }
        else if (los_p(Player.x,Player.y,this->x,this->y)) {
            mprint("You see the dog jerked back by its chain!");
            plotspot(this->x, this->y, false);
        }
        else mprint("You hear a strangled sort of yelp!");
        Level->site[this->x][this->y].creature = NULL;
        this->x = this->aux1;
        this->y = this->aux2;
        Level->site[this->x][this->y].creature = this;
    }
}

/*               Revised function                   */
/* WDT: code contributed by David J. Robertson */
/* consider one monster's action */
void Monster::m_pulse()
{
    int range = distance(this->x, this->y, Player.x,Player.y);
    int STRIKE=false;
    pol prev;

    if (Time % 10 == 0)
        if (this->hp < Monsters[this->id].hp)
            this->hp++;

    if ((! m_statusp(this,AWAKE)) && (range <= this->wakeup)) {
        m_status_set(this,AWAKE);
        resetgamestatus(FAST_MOVE);
    }

    if (m_statusp(this,AWAKE)) {
        if (m_statusp(this,WANDERING)) {
            if (m_statusp(this,MOBILE)) m_random_move();
            if (range <= this->sense && (m_statusp(this, HOSTILE) ||
                                      m_statusp(this, NEEDY)))
                m_status_reset(this,WANDERING);
        }
        else { /* not wandering */
            if (m_statusp(this,HOSTILE))
                if ((range > 2) && (range < this->sense) && (random_range(2) == 1))
                    if (los_p(this->x,this->y,Player.x,Player.y) &&
                            (Player.status[INVISIBLE] == 0)) {
                        STRIKE=true;
                        monster_strike();
                    }

            if ((m_statusp(this,HOSTILE) || m_statusp(this,NEEDY))
                    && (range > 1) && m_statusp(this,MOBILE) &&
                    (!STRIKE || (random_range(2) == 1)))
                monster_move();
            else if (m_statusp(this,HOSTILE) && (range ==1)) {
                resetgamestatus(FAST_MOVE);
                tacmonster(this);
            }
        }
        /* if monster is greedy, picks up treasure it finds */
        if (m_statusp(this,GREEDY) && (this->hp >0) )
            while (Level->site[this->x][this->y].things != NULL) {
                m_pickup(Level->site[this->x][this->y].things->thing);
                prev = Level->site[this->x][this->y].things;
                Level->site[this->x][this->y].things =
                    Level->site[this->x][this->y].things->next;
                free((char *) prev);
            }
        /* prevents monsters from casting spells from other side of dungeon */
        if ((range < max(5,this->level)) && (this->hp > 0) &&
                (random_range(2) == 1))
            monster_special();
    }
}

/* actually make a move */
void Monster::movemonster(int newx, int newy)
{
    if (Level->site[newx][newy].creature != NULL)
        return;
    if (Level->site[this->x][this->y].creature == this)
        Level->site[this->x][this->y].creature = NULL;
    this->x = newx;
    this->y = newy;
    Level->site[this->x][this->y].creature = this;
    m_movefunction(this,Level->site[this->x][this->y].p_locf);
}

/* give object o to monster m */
void Monster::m_pickup(pob o)
{
    pol tmp = ((pol) checkmalloc(sizeof(oltype)));
    tmp->thing = o;
    tmp->next = this->possessions;
    this->possessions = tmp;
}

// Drop all possessions
void Monster::m_dropstuff()
{
    pol tmp = this->possessions;
    if (tmp != NULL) {
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = Level->site[this->x][this->y].things;
        Level->site[this->x][this->y].things = this->possessions;
        this->possessions = NULL;
    }
}

// Attempt to deal damage to monster
void Monster::m_damage(int dmg, int dtype)
{
    m_status_set(this,AWAKE);
    m_status_set(this,HOSTILE);
    if (m_immunityp(this,dtype)) {
        if (los_p(Player.x,Player.y,this->x,this->y)) {
            if (this->uniqueness != COMMON) strcpy(Str1,this->name);
            else {
                strcpy(Str1,"The ");
                strcat(Str1,this->name);
            }
            strcat(Str1," ignores the attack!");
            mprint(Str1);
        }
    }
    else if ((this->hp -= dmg) < 1) m_death();
}

/* remove a monster -- death without crediting player */
void Monster::m_remove()
{
    Level->site[this->x][this->y].creature = NULL;
    erase_monster(this);
    this->hp = -1; /* signals "death" -- no credit to player, though */
    /* DAG -- monster structure will be "cleaned up" in time_clock() which */
    /*     walks through all the monsters each "tick".  (Or on level free.) */
}

// Perform death actions
void Monster::m_death()
{
    pob corpse;
    MonsterList* ml;
    int x,y,found=false;
    pol curr, prev = NULL;

    this->hp = -1;
    if (los_p(Player.x,Player.y,this->x,this->y)) {
        gain_experience(this->xpv);
        calc_melee();
        if (this->uniqueness != COMMON) strcpy(Str1,this->name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,this->name);
        }
        strcat(Str1," is dead! ");
        mprint(Str1);
    }
    this->m_dropstuff();
    if (this->id == DEATH) { /* Death */
        mprint("Death lies sprawled out on the ground......");
        mprint("Death laughs ironically and gets back to its feet.");
        mprint("It gestures and another scythe appears in its hands.");
        switch(random_range(10)) {
        case 0:
            mprint("Death performs a little bow and goes back on guard.");
            break;
        case 1:
            mprint("'A hit! A palpable hit!' Death goes back on the attack.");
            break;
        case 2:
            mprint("'Ah, if only it could be so simple!' snickers Death.");
            break;
        case 3:
            mprint("'You think Death can be slain?  What a jest!' says Death.");
            break;
        case 4:
            mprint("'Your point is well taken.' says Death, attacking again.");
            break;
        case 5:
            mprint("'Oh, come now, stop delaying the inevitable.' says Death.");
            break;
        case 6:
            mprint("'Your destiny ends here with me.' says Death, scythe raised.");
            break;
        case 7:
            mprint("'I almost felt that.' says Death, smiling.");
            break;
        case 8:
            mprint("'Timeo Mortis?' asks Death quizzically, 'Not me!'");
            break;
        case 9:
            mprint("Death sighs theatrically. 'They never learn.'");
            break;
        }
        strengthen_death();
    }
    else {
        Level->site[this->x][this->y].creature = NULL;
        if (this == Arena_Monster)
            Arena_Victory = true;	/* won this round of arena combat */
        if (random_range(2) || (this->uniqueness != COMMON)) {
            corpse=((pob) checkmalloc(sizeof(objtype)));
            make_corpse(corpse,this);
            drop_at(this->x,this->y,corpse);
        }
        plotspot(this->x,this->y,false);
        switch(this->id) {
        case HISCORE_NPC:
            switch(this->aux2) {
            case 0:
                mprint("You hear a faroff dirge. You feel a sense of triumph.");
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                mprint("You hear a faroff sound like angels crying....");
                strcpy(Priest[this->aux2],nameprint());
                Priestbehavior[this->aux2] = 2933;
                break;
            case 7:
                mprint("A furtive figure dashes out of the shadows, takes a look at");
                mprint("the corpse, and runs away!");
                strcpy(Shadowlord,nameprint());
                Shadowlordbehavior = 2912;
                break;
            case 8:
                mprint("An aide-de-camp approaches, removes the corpse's insignia,");
                mprint("and departs.");
                strcpy(Commandant,nameprint());
                Commandantbehavior = 2912;
                break;
            case 9:
                mprint("An odd glow surrounds the corpse, and slowly fades.");
                strcpy(Archmage,nameprint());
                Archmagebehavior = 2933;
                break;
            case 10:
                mprint("A demon materializes, takes a quick look at the corpse,");
                mprint("and teleports away with a faint popping noise.");
                strcpy(Prime,nameprint());
                Primebehavior = 2932;
                break;
            case 11:
                mprint("A sports columnist rushes forward and takes a quick photo");
                mprint("of the corpse and rushes off muttering about a deadline.");
                strcpy(Champion,nameprint());
                Championbehavior = 2913;
                break;
            case 12:
                mprint("You hear a fanfare in the distance, and feel dismayed.");
                strcpy(Duke,nameprint());
                Dukebehavior = 2911;
                break;
            case 13:
                if (Player.alignment > 10) mprint("You feel smug.");
                else if (Player.alignment < 10) mprint("You feel ashamed.");
                strcpy(Chaoslord,nameprint());
                Chaoslordbehavior = 2912;
                break;
            case 14:
                if (Player.alignment < 10) mprint("You feel smug.");
                else if (Player.alignment > 10) mprint("You feel ashamed.");
                strcpy(Lawlord,nameprint());
                Lawlordbehavior = 2911;
                break;
            case 15:
                /* just a tad complicated. Promote a new justiciar if any
                   guards are left in the city, otherwise Destroy the Order! */
                Player.alignment -= 100;
                if (! gamestatusp(DESTROYED_ORDER)) {
                    curr = Level->site[this->x][this->y].things;
                    while (curr && curr->thing->id != OB_JUSTICIAR_BADGE) {
                        prev = curr;
                        curr = curr->next;
                    }
                    strcpy(Justiciar,nameprint());
                    Justiciarbehavior = 2911;
                    mprint("In the distance you hear a trumpet. A Servant of Law");
                    /* promote one of the city guards to be justiciar */
                    ml = City->mlist;
                    while ((! found) && (ml != NULL)) {
                        found = ((ml->monster->id == GUARD) && (ml->monster->hp > 0));
                        if (! found) ml=ml->next;
                    }
                    if (ml != NULL) {
                        if (curr) {
                            mprint("materializes, sheds a tear, picks up the badge, and leaves.");
                            ml->monster->m_pickup(curr->thing);
                            if (prev)
                                prev->next = curr->next;
                            else
                                Level->site[this->x][this->y].things = curr->next;
                            free(curr);
                        }
                        else
                            mprint("materializes, sheds a tear, and leaves.");
                        mprint("A new justiciar has been promoted!");
                        x = ml->monster->x;
                        y = ml->monster->y;
                        make_hiscore_npc(this, 15);
                        ml->monster->x = x;
                        ml->monster->y = y;
                        ml->monster->click = (Tick + 1) % 60;
                        m_status_reset(ml->monster,AWAKE);
                        m_status_reset(ml->monster,HOSTILE);
                    }
                    else {
                        mprint("materializes, sheds a tear, and leaves.");
                        morewait();
                    }
                    alert_guards();
                    /* will cause order to be destroyed if no guards or justiciar*/
                }
                else {
                    mprint("A Servant of Chaos materializes, grabs the corpse,");
                    mprint("snickers a bit, and vanishes.");
                }
                break;
            case 16:
            	if (random_range(3)) {
            		/* paraphrase of harlan ellison... ? PGM */
            		mprint("It doesn't rain this day, anywhere in the known universe.");
            	} else {
            		mprint("The universal equilibrium slides down a notch.");
            	}

                strcpy(Grandmaster,nameprint());
                Grandmasterbehavior = 2933;
                break;
            }
            save_hiscore_npc(this->aux2);
            break;
        case GUARD: /* guard */
            Player.alignment -= 10;
            if ((Current_Environment == E_CITY) ||
                    (Current_Environment == E_VILLAGE))
                alert_guards();
            break;
        case GOBLIN_KING:
            if (! gamestatusp(ATTACKED_ORACLE)) {
                mprint("You seem to hear a woman's voice from far off:");
                mprint("'Well done! Come to me now....'");
            }
            setgamestatus(COMPLETED_CAVES);
            break; /* gob king */
        case GREAT_WYRM:
            if (! gamestatusp(ATTACKED_ORACLE)) {
                mprint("A female voice sounds from just behind your ear:");
                mprint("'Well fought! I have some new advice for you....'");
            }
            setgamestatus(COMPLETED_SEWERS);
            break; /*grt worm */
        case EATER:
            setgamestatus(KILLED_EATER);
            break;
        case LAWBRINGER:
            setgamestatus(KILLED_LAWBRINGER);
            break;
        case DRAGON_LORD:
            setgamestatus(KILLED_DRAGONLORD);
            break;
        case DEMON_EMP:
            setgamestatus(COMPLETED_VOLCANO);
            if (! gamestatusp(ATTACKED_ORACLE)) {
                mprint("You feel a soft touch on your shoulder...");
                mprint("You turn around but there is no one there!");
                mprint("You turn back and see a note: 'See me soon.'");
                mprint("The note vanishes in a burst of blue fire!");
            }
            break;
        case ELEM_MASTER:
            if (! gamestatusp(ATTACKED_ORACLE)) {
                mprint("Words appear before you, traced in blue flame!");
                mprint("'Return to the Prime Plane via the Circle of Sorcerors....'");
            }
            break; /* elem mast */
        }
        switch (this->specialf) {
        case M_SP_COURT:
        case M_SP_LAIR:
            m_status_set(this,HOSTILE);
            monster_action(this->specialf);
        }
    }
}

void Monster::monster_move()
{
    monster_action(movef);
}

void Monster::monster_strike()
{
    if (player_on_sanctuary())
        print1("The aegis of your deity protects you!");
    else {
        /* being attacked wakes you up/stops fast move */
        resetgamestatus(FAST_MOVE);

        /* It's lawful to wait to be attacked */
        if (this->wasAttackedByPlayer == false) Player.alignment++;

        this->wasAttackedByPlayer = true;

        monster_action(this->strikef);
    }
}

void Monster::monster_special()
{
    /* since many special functions are really attacks, cancel them
       all if on sanctuary */
    if (! player_on_sanctuary())
    {
        /* being attacked wakes you up/stops fast move */
        resetgamestatus(FAST_MOVE);

        monster_action(this->specialf);
    }
}

void Monster::monster_talk()
{
    monster_action(this->talkf);
}

void Monster::monster_action(int action)
{
    int meleef;
    if ((action >= M_MELEE_NORMAL) && (action < M_MOVE_NORMAL)) {
        /* kluge allows multiple attack forms */
        if (distance(this->x,this->y,Player.x,Player.y)<2) {
            meleef = this->meleef;
            this->meleef = action;
            tacmonster(this);
            this->meleef = meleef;
        }
    }
    else switch(action) {
        case M_NO_OP:
            break;
        case M_MOVE_NORMAL:
            m_normal_move();
            break;
        case M_MOVE_FLUTTER:
            m_flutter_move();
            break;
        case M_MOVE_FOLLOW:
            m_follow_move();
            break;
        case M_MOVE_TELEPORT:
            m_teleport();
            break;
        case M_MOVE_RANDOM:
            m_random_move();
            break;
        case M_MOVE_SMART:
            m_smart_move();
            break;
        case M_MOVE_SPIRIT:
            m_spirit_move();
            break;
        case M_MOVE_CONFUSED:
            m_confused_move();
            break;
        case M_MOVE_SCAREDY:
            m_scaredy_move();
            break;
        case M_MOVE_ANIMAL:
            m_move_animal();
            break;
        case M_MOVE_LEASH:
            m_move_leash();
            break;
        case M_STRIKE_MISSILE:
            m_nbolt(this);
            break;
        case M_STRIKE_FBOLT:
            m_firebolt(this);
            break;
        case M_STRIKE_LBALL:
            m_lball(this);
            break;
        case M_STRIKE_FBALL:
            m_fireball(this);
            break;
        case M_STRIKE_SNOWBALL:
            m_snowball(this);
            break;
        case M_STRIKE_BLIND:
            m_blind_strike(this);
            break;
        case M_STRIKE_SONIC:
            m_strike_sonic(this);
            break;

        case M_TALK_HORSE:
            m_talk_horse(this);
            break;
        case M_TALK_THIEF:
            m_talk_thief(this);
            break;
        case M_TALK_STUPID:
            m_talk_stupid(this);
            break;
        case M_TALK_SILENT:
            m_talk_silent(this);
            break;
        case M_TALK_HUNGRY:
            m_talk_hungry(this);
            break;
        case M_TALK_GREEDY:
            m_talk_greedy(this);
            break;
        case M_TALK_TITTER:
            m_talk_titter(this);
            break;
        case M_TALK_MP:
            m_talk_mp(this);
            break;
        case M_TALK_IM:
            m_talk_im(this);
            break;
        case M_TALK_MAN:
            m_talk_man(this);
            break;
        case M_TALK_ROBOT:
            m_talk_robot(this);
            break;
        case M_TALK_EVIL:
            m_talk_evil(this);
            break;
        case M_TALK_GUARD:
            m_talk_guard(this);
            break;
        case M_TALK_MIMSY:
            m_talk_mimsy(this);
            break;
        case M_TALK_SLITHY:
            m_talk_slithy(this);
            break;
        case M_TALK_BURBLE:
            m_talk_burble(this);
            break;
        case M_TALK_BEG:
            m_talk_beg(this);
            break;
        case M_TALK_HINT:
            m_talk_hint(this);
            break;
        case M_TALK_EF:
            m_talk_ef(this);
            break;
        case M_TALK_GF:
            m_talk_gf(this);
            break;
        case M_TALK_SEDUCTOR:
            m_talk_seductor(this);
            break;
        case M_TALK_DEMONLOVER:
            m_talk_demonlover(this);
            break;
        case M_TALK_NINJA:
            m_talk_ninja(this);
            break;
        case M_TALK_ASSASSIN:
            m_talk_assassin(this);
            break;
        case M_TALK_SERVANT:
            m_talk_servant(this);
            break;
        case M_TALK_ANIMAL:
            m_talk_animal(this);
            break;
        case M_TALK_SCREAM:
            m_talk_scream(this);
            break;
        case M_TALK_PARROT:
            m_talk_parrot(this);
            break;
        case M_TALK_HYENA:
            m_talk_hyena(this);
            break;
        case M_TALK_DRUID:
            m_talk_druid(this);
            break;
        case M_TALK_ARCHMAGE:
            m_talk_archmage(this);
            break;
        case M_TALK_MERCHANT:
            m_talk_merchant(this);
            break;
        case M_TALK_PRIME:
            m_talk_prime(this);
            break;
        case M_TALK_MAHARAJA:
            m_talk_maharaja(this);
            break;

        case M_SP_BOG:
            m_sp_bogthing(this);
            break;
        case M_SP_WERE:
            m_sp_were(this);
            break;
        case M_SP_WHISTLEBLOWER:
            m_sp_whistleblower(this);
            break;
        case M_SP_MERCHANT:
            m_sp_merchant(this);
            break;
        case M_SP_SURPRISE:
            m_sp_surprise(this);
            break;
        case M_SP_MP:
            m_sp_mp(this);
            break;
        case M_SP_THIEF:
            m_thief_f(this);
            break;
        case M_SP_DEMONLOVER:
            m_sp_demonlover(this);
            break;
        case M_SP_AGGRAVATE:
            m_aggravate(this);
            break;
        case M_SP_POISON_CLOUD:
            m_sp_poison_cloud(this);
            break;
        case M_SP_HUGE:
            m_huge_sounds(this);
            break;
        case M_SP_SUMMON:
            m_summon(this);
            break;
        case M_SP_ILLUSION:
            m_illusion(this);
            break;
        case M_SP_ESCAPE:
            m_sp_escape(this);
            break;
        case M_SP_FLUTTER:
            m_flutter_move();
            break;
        case M_SP_EXPLODE:
            m_sp_explode(this);
            break;
        case M_SP_DEMON:
            m_sp_demon(this);
            break;
        case M_SP_ACID_CLOUD:
            m_sp_acid_cloud(this);
            break;
        case M_SP_GHOST:
            m_sp_ghost(this);
            break;
        case M_SP_SPELL:
            m_sp_spell(this);
            break;
        case M_SP_SEDUCTOR:
            m_sp_seductor(this);
            break;
        case M_SP_EATER:
            m_sp_eater(this);
            break;
        case M_SP_DRAGONLORD:
            m_sp_dragonlord(this);
            break;
        case M_SP_BLACKOUT:
            m_sp_blackout(this);
            break;
        case M_SP_SWARM:
            m_sp_swarm(this);
            break;
        case M_SP_ANGEL:
            m_sp_angel(this);
            break;
        case M_SP_SERVANT:
            m_sp_servant(this);
            break;
        case M_SP_AV:
            m_sp_av(this);
            break;
        case M_SP_LW:
            m_sp_lw(this);
            break;
        case M_SP_MB:
            m_sp_mb(this);
            break;
        case M_SP_RAISE:
            m_sp_raise(this);
            break;
        case M_SP_MIRROR:
            m_sp_mirror(this);
            break;
        case M_SP_COURT:
            m_sp_court(this);
            break;
        case M_SP_LAIR:
            m_sp_lair(this);
            break;
        case M_SP_PRIME:
            m_sp_prime(this);
            break;
        }
}

/* sets npc behavior given level and behavior code */
void Monster::determine_npc_behavior(int level, int behavior)
{
    int combatype,competence,talktype;
    hp = (level+1)*20;
    status = AWAKE+MOBILE+WANDERING;
    combatype = (behavior % 100) / 10;
    competence = (behavior % 1000) / 100;
    talktype = behavior / 1000;
    level = competence;
    if (level < 2*difficulty()) status += HOSTILE;
    xpv = level*20;
    switch (combatype) {
    case 1: /* melee */
        meleef = M_MELEE_NORMAL;
        dmg = competence*5;
        hit = competence*3;
        speed = 3;
        break;
    case 2: /*missile*/
        meleef = M_MELEE_NORMAL;
        strikef = M_STRIKE_MISSILE;
        dmg = competence*3;
        hit = competence*2;
        speed = 4;
        break;
    case 3: /* spellcasting */
        meleef = M_MELEE_NORMAL;
        dmg = competence;
        hit = competence;
        specialf = M_SP_SPELL;
        speed = 6;
        break;
    case 4: /* thievery */
        meleef = M_MELEE_NORMAL;
        dmg = competence;
        hit = competence;
        specialf=M_SP_THIEF;
        speed = 3;
        break;
    case 5: /* flee */
        dmg = competence;
        hit = competence;
        meleef = M_MELEE_NORMAL;
        specialf = M_MOVE_SCAREDY;
        speed = 3;
        break;
    }
    if (talkf == M_TALK_MAN)
        switch (talktype) {
        case 1:
            talkf = M_TALK_EVIL;
            break;
        case 2:
            talkf = M_TALK_MAN;
            break;
        case 3:
            talkf = M_TALK_HINT;
            break;
        case 4:
            talkf = M_TALK_BEG;
            break;
        case 5:
            talkf = M_TALK_SILENT;
            break;
        default:
            mprint("Say Whutt? (npc talk weirdness)");
            break;
        }
    uniqueness = UNIQUE_MADE;
}

void Monster::m_trap_dart()
{
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," was hit by a dart!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    m_damage(difficulty()*2,NORMAL_DAMAGE);
}

void Monster::m_trap_pit()
{
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," fell into a pit!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    if (! m_statusp(this,INTANGIBLE))
        m_status_reset(this,MOBILE);
    m_damage(difficulty()*5,NORMAL_DAMAGE);
}

void Monster::m_trap_door()
{
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," fell into a trap door!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    m_vanish();
}

void Monster::m_trap_abyss()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," fell into the infinite abyss!");
        mprint(Str1);
        Level->site[x][y].locchar = ABYSS;
        lset(x, y, CHANGED);
        Level->site[x][y].p_locf = L_ABYSS;
        lset(x, y, CHANGED);
    }
    setgamestatus(SUPPRESS_PRINTING);
    m_vanish();
    resetgamestatus(SUPPRESS_PRINTING);
}

void Monster::m_trap_snare()
{
    char Str1[80];
    Level->site[x][y].locchar = TRAP;
    lset(x, y, CHANGED);
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," was caught in a snare!");
        mprint(Str1);
    }
    if (! m_statusp(this,INTANGIBLE)) m_status_reset(this,MOBILE);
}

void Monster::m_trap_blade()
{
    char Str1[80];
    Level->site[x][y].locchar = TRAP;
    lset(x, y, CHANGED);
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," was hit by a blade trap!");
        mprint(Str1);
    }
    m_damage((difficulty()+1)*7-Player.defense,NORMAL_DAMAGE);
}

void Monster::m_trap_fire()
{
    char Str1[80];
    Level->site[x][y].locchar = TRAP;
    lset(x, y, CHANGED);
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," was hit by a fire trap!");
        mprint(Str1);
    }
    m_damage((difficulty()+1)*5,FLAME);
}

void Monster::m_fire()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," was blasted by fire!");
        mprint(Str1);
    }
    m_damage(random_range(100),FLAME);
}

void Monster::m_trap_teleport()
{
    char Str1[80];
    Level->site[x][y].locchar = TRAP;
    lset(x, y, CHANGED);
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," walked into a teleport trap!");
        mprint(Str1);
    }
    m_teleport();
}

void Monster::m_trap_disintegrate()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," walked into a disintegration trap!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    disintegrate(x,y);
}

void Monster::m_trap_sleepgas()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," walked into a sleepgas trap!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    if (! m_immunityp(this,SLEEP)) m_status_reset(this,AWAKE);
}

void Monster::m_trap_acid()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," walked into an acid bath trap!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    m_damage(random_range(difficulty()*difficulty()),ACID);
}

void Monster::m_trap_manadrain()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," walked into a manadrain trap!");
        mprint(Str1);
        Level->site[x][y].locchar = TRAP;
        lset(x, y, CHANGED);
    }
    if (specialf == M_SP_SPELL) specialf = M_NO_OP;
}

void Monster::m_water()
{
    char Str1[80];
    if ((! m_statusp(this,INTANGIBLE)) &&
            (! m_statusp(this,SWIMMING)) &&
            (! m_statusp(this,ONLYSWIM))) {
        if (los_p(x,y,Player.x,Player.y)) {
            if (uniqueness != COMMON) strcpy(Str1,name);
            else {
                strcpy(Str1,"The ");
                strcat(Str1,name);
            }
            strcat(Str1," drowned!");
            mprint(Str1);
        }
        m_death();
    }
}

void Monster::m_abyss()
{
    char Str1[80];
    if (los_p(x,y,Player.x,Player.y)) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," fell into the infinite abyss!");
        mprint(Str1);
    }
    m_vanish();
}

void Monster::m_lava()
{
    char Str1[80];
    if ((! m_immunityp(this,FLAME)) ||
            ((! m_statusp(this,SWIMMING))&& (! m_statusp(this,ONLYSWIM)))) {
        if (los_p(x,y,Player.x,Player.y)) {
            if (uniqueness != COMMON) strcpy(Str1,name);
            else {
                strcpy(Str1,"The ");
                strcat(Str1,name);
            }
            strcat(Str1," died in a pool of lava!");
            mprint(Str1);
        }
        m_death();
    }
}

void Monster::m_altar()
{
    int visible = view_los_p(Player.x,Player.y,x,y);
    int reaction = 0;
    int altar = Level->site[x][y].aux;

    if (visible) {
        if (uniqueness != COMMON) strcpy(Str1,name);
        else {
            strcpy(Str1,"The ");
            strcat(Str1,name);
        }
        strcat(Str1," walks next to an altar...");
        mprint(Str1);
    }
    if (!m_statusp(this, HOSTILE))
        reaction = 0;
    else if (id == HISCORE_NPC && aux2 == altar)
        reaction = 1;	/* high priest of same deity */
    else if ((id == ANGEL || id == HIGH_ANGEL || id == ARCHANGEL) &&
             aux1 == altar)
        reaction = 1;	/* angel of same deity */
    else if (altar == Player.patron)
        reaction = -1;	/* friendly deity will zap hostile monster */
    else if (((Player.patron == ODIN || Player.patron == ATHENA) &&
              (altar == SET || altar == HECATE)) ||
             ((Player.patron == SET || Player.patron == HECATE) &&
              (altar == ODIN || altar == ATHENA)))
        reaction = 1;	/* hostile deity will help hostile monster */
    switch (reaction) {
    case -1:
        if (visible) {
            mprint("Your deity is angry!");
            mprint("A bolt of godsfire strikes the monster....");
        }
        disrupt(x,y,Player.rank[PRIESTHOOD]*50);
        break;
    case 1:
        if (visible) {
            mprint("The deity of the altar smiles on the monster....");
            mprint("A shaft of light zaps the altar...");
        }
        hp = Monsters[id].hp*2;
        break;
    default:
        if (visible)
            mprint("but nothing much seems to happen");
        break;
    }
}

void Monster::strengthen_death()
{
    pol ol = ((pol)checkmalloc(sizeof(oltype)));
    pob scythe = ((pob)checkmalloc(sizeof(objtype)));

    xpv += min(10000,xpv+1000);
    hit += min(1000,hit+10);
    dmg = min(10000,dmg*2);
    ac += min(1000,ac+10);
    speed = max(speed-1,1);
    movef = M_MOVE_SMART;

	/* In order not to have to make the hp's into longs or unsigned,
    which would involve lots of changes, I'll make it max out at 30000. */
	hp = min(30000, 100 * dmg * 10);

    *scythe = Objects[OB_SCYTHE_DEATH];
    ol->thing = scythe;
    ol->next = NULL;
    possessions = ol;
}
