/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* mspec.c */
/* monster special functions */

#include "glob.h"

void Monster::m_sp_mp()
{
    if (this->wasAttackedByPlayer && (random_range(3) == 1)) {
        mprint("You feel cursed!");
        p_damage(10,UNSTOPPABLE,"a mendicant priest's curse");
        this->m_vanish();
    }
    else if (! m_statusp(this,NEEDY)) {
        mprint("The mendicant priest makes a mystical gesture....");
        mprint("You feel impressed...");
        Player.alignment += 5;
        if (Player.alignment > 20)
            Player.hp = max(Player.hp,Player.maxhp);
        this->m_vanish();
    }
}

void Monster::m_sp_ng()
{
    if (distance(this->x,this->y,Player.x,Player.y) < 2)
        if ((random_range(5) == 1) || (Player.status[VULNERABLE]>0)) {
            mprint("The night gaunt grabs you and carries you off!");
            mprint("Its leathery wings flap and flap, and it giggles insanely.");
            mprint("It tickles you cunningly to render you incapable of escape.");
            mprint("Finally, it deposits you in a strange place.");
            p_teleport(0);
        }
}

void Monster::m_sp_poison_cloud()
{
    if (distance(this->x,this->y,Player.x,Player.y) < 3) {
        mprint("A cloud of poison gas surrounds you!");
        if (Player.status[BREATHING] > 0)
            mprint("You can breathe freely, however.");
        else p_poison(7);
    }
}

void Monster::m_sp_explode()
{
    if ((distance(Player.x,Player.y,this->x,this->y)<2) &&
            (this-> hp > 0) &&
            (this->hp < Monsters[this->id].hp))
        fball(this->x,this->y,this->x,this->y,this->hp);
}

void Monster::m_sp_demon ()
{
    int mid;

    if (random_range(2))
    {
        if ((this->id != INCUBUS) /* succubi don't give fear */
                && los_p(this->x, this->y, Player.x, Player.y)
                && (random_range(30) > (Player.level + 10))
                && (0 == Player.status[AFRAID]))
        {
            mprint("You are stricken with fear!");
            if (!p_immune(FEAR))
                Player.status[AFRAID] += this->level;
            else
                mprint("You master your reptile brain and stand fast.");
        }
        else
        {
            m_sp_spell();
        }
    }

    if ((this->hp < (this->level * 5)) && (this->hp > 1))
    {
        mprint("The demon uses its waning lifeforce to summon help!");
        this->hp = 1;
        switch(this->level)
        {
        case 3:
            mid = NIGHT_GAUNT;
            break;
        case 4:
        case 5:
            mid = L_FDEMON;
            break; /* lesser frost demon */
        case 6:
            mid = FROST_DEMON;
            break;
        case 7:
            mid = OUTER_DEMON;
            break; /* outer circle demon */
        case 8:
            mid = DEMON_SERP;
            break; /* demon serpent */
        case 9:
            mid = INNER_DEMON;
            break; /* inner circle demon */
        default:
            mid = 0;
            assert(false); /* bomb on error */
        }

        summon(-1, mid);
        summon(-1, mid);
    }
}

void Monster::m_sp_acid_cloud()
{
    if (m_statusp(this,HOSTILE) &&
            (distance(this->x,this->y,Player.x,Player.y) < 3))
        acid_cloud();
}

void Monster::m_sp_escape()
{
    if (m_statusp(this,HOSTILE))
        this->m_vanish();
}

void Monster::m_sp_ghost()
{
    if (m_statusp(this,HOSTILE)) {
        mprint("The ghost moans horribly....");
        p_damage(1,FEAR,"a ghost-inspired heart attack");
        mprint("You've been terrorized!");
        if (! p_immune(FEAR)) Player.status[AFRAID] += this->level;
        else mprint("You master your reptile brain and stand fast.");
    }
}

/* random spell cast by monster */
void Monster::m_sp_spell()
{
    char action[80];
    if (m_statusp(this,HOSTILE) && los_p(Player.x,Player.y,this->x,this->y)) {
        if (this->uniqueness == COMMON) strcpy(action,"The ");
        else strcpy(action,"");
        strcat(action,this->name);
        strcat(action," casts a spell...");
        mprint(action);
        if (! magic_resist(this->level)) switch (random_range(this->level+7)) {
            case 0:
                nbolt(this->x,this->y,Player.x,Player.y,this->hit,10);
                break;
            case 1:
                mprint("It seems stronger...");
                this->hp += random_range(this->level*this->level);
                break;
            case 2:
                haste(-1);
                break;
            case 3:
                cure(-1);
                break;
            case 4:
                /* WDT: I'd like to make this (and "case 5" below) dependant on
                 * the monster's IQ in some way -- dumb but powerful monsters
                 * deserve what they get :).  No rush. */
                if (m_immunityp(this, ELECTRICITY) ||
                        distance(this->x,this->y,Player.x,Player.y) > 2)
                    lball(this->x,this->y,Player.x,Player.y,20);
                else
                    lbolt(this->x,this->y,Player.x,Player.y,this->hit,20);
                break;
            case 5:
                if (m_immunityp(this, COLD) ||
                        distance(this->x,this->y,Player.x,Player.y) > 2)
                    snowball(this->x,this->y,Player.x,Player.y,30);
                else
                    icebolt(this->x,this->y,Player.x,Player.y,this->hit,30);
                break;
            case 6:
                enchant(-1);
                break;
            case 7:
                bless(0-this->level);
                break;
            case 8:
                p_poison(this->level);
                break;
            case 9:
                sleep_player(this->level/2);
                break;
            case 10:
                fbolt(this->x,this->y,Player.x,Player.y,this->hit*3,50);
                break;
            case 11:
                acquire(0-this->level);
                break;
            case 12:
                dispel(-1);
                break;
            case 13:
                disrupt(Player.x,Player.y,50);
                break;
            case 14:
                if (this->uniqueness == COMMON) {
                    strcpy(Str2,"a ");
                    strcat(Str2,this->name);
                }
                else strcpy(Str2,this->name);
                level_drain(this->level,Str2);
                break;
            case 15:
            case 16:
                disintegrate(Player.x,Player.y);
                break;
            }
    }
}

/* monsters with this have some way to hide, camouflage, etc until they
   attack */
void Monster::m_sp_surprise()
{
    if (this->wasAttackedByPlayer) {
        if (m_statusp(this,HOSTILE) &&
                (! Player.status[TRUESIGHT]) &&
                m_statusp(this,M_INVISIBLE)) {
            this->symbol = Monsters[this->id].symbol;
            if (! Player.status[ALERT]) {
                switch(random_range(4)) {
                case 0:
                    mprint("You are surprised by a sudden treacherous attack!");
                    break;
                case 1:
                    mprint("You are shocked out of your reverie by the scream of battle!");
                    break;
                case 2:
                    mprint("Suddenly, from out of the shadows, a surprise attack!");
                    break;
                case 3:
                    mprint("A shriek of hatred causes you to momentarily freeze up!");
                    break;
                }
                morewait();
                State.setSkipPlayer( true );
                m_status_reset(this,M_INVISIBLE);
            }
            else {
                mprint("You alertly sense the presence of an attacker!");
                m_status_reset(this,M_INVISIBLE);
            }
        }
    }
}

void Monster::m_sp_whistleblower()
{
    if (m_statusp(this,HOSTILE)) {
        alert_guards();
        this->specialf = M_MELEE_NORMAL;
    }
}

void Monster::m_sp_seductor()
{
    if (m_statusp(this,HOSTILE)) {
        if (this->uniqueness == COMMON) {
            strcpy(Str2,"The ");
            strcat(Str2,this->name);
        }
        else strcpy(Str2,this->name);
        strcat(Str2," runs away screaming for help....");
        mprint(Str2);
        this->m_vanish();
        summon(-1,-1);
        summon(-1,-1);
        summon(-1,-1);
    }
    else if (distance(Player.x,Player.y,this->x,this->y) < 2)
        m_talk_seductor();
}

void Monster::m_sp_demonlover()
{
    if (distance(Player.x,Player.y,this->x,this->y) < 2)
        m_talk_demonlover();
}

void Monster::m_sp_eater()
{
    if (Player.rank[COLLEGE]) m_status_set(this,HOSTILE);
    if (m_statusp(this,HOSTILE))
        if (los_p(this->x,this->y,Player.x,Player.y)) {
            mprint("A strange numbing sensation comes over you...");
            morewait();
            Player.mana = Player.mana / 2;
            if (random_range(4)) enchant(-1);
            else dispel(-1);
            Player.pow--;
            if (--Player.pow < 1) p_death("the Eater of Magic");
        }
    if (this->hp < 10) {
        mprint("The Eater explodes in a burst of mana!");
        manastorm(this->x,this->y,1000);
    }
}

void Monster::m_sp_dragonlord()
{
    if (m_statusp(this,HOSTILE)) {
        if (distance(this->x,this->y,Player.x,Player.y)<2) {
            if (! Player.status[IMMOBILE]) {
                mprint("A gust of wind from the Dragonlord's wings knocks you down!");
                p_damage(25,NORMAL_DAMAGE,"a gust of wind");
                State.setSkipPlayer( true );
                Player.status[IMMOBILE]+=2;
            }
            else if (! Constriction) {
                mprint("The Dragonlord grabs you with his tail!");
                Constriction = 25;
                Player.status[IMMOBILE]+=1;
            }
            else if (random_range(2)) {
                mprint("The coils squeeze tighter and tighter...");
                p_damage(Constriction,NORMAL_DAMAGE,"the Dragonlord");
                Player.status[IMMOBILE]+=1;
                Constriction *=2;
            }
            else {
                mprint("The Dragonlord hurls you to the ground!");
                p_damage(2*Constriction,NORMAL_DAMAGE,"the Dragonlord");
                Constriction = 0;
            }
            m_sp_spell();
        }
        else {
            Constriction = 0;
            if (view_los_p(this->x,this->y,Player.x,Player.y)) {
                if ((! Player.immunity[FEAR]) && (! Player.status[AFRAID])) {
                    mprint("You are awestruck at the sight of the Dragonlord.");
                    Player.status[AFRAID]+=5;
                }
                if (random_range(3)) {
                    m_sp_spell();
                    m_sp_spell();
                }
            }
        }
    }
    else if (distance(this->x,this->y,Player.x,Player.y)<2)
        mprint("You are extremely impressed at the sight of the Dragonlord.");
}

void Monster::m_sp_blackout()
{
    if ((distance(this->x,this->y,Player.x,Player.y) < 4) &&
            (Player.status[BLINDED] == 0)) {
        mprint("The fungus emits a burst of black spores. You've been blinded!");
        if (Player.status[TRUESIGHT] > 0) mprint("The blindness quickly passes.");
        else Player.status[BLINDED]+=4;
    }
    if (loc_statusp(this->x,this->y,LIT)) {
        mprint("The fungus chirps.... ");
        mprint("The area is plunged into darkness.");
        torch_check();
        torch_check();
        torch_check();
        torch_check();
        torch_check();
        torch_check();
        spreadroomdark(this->x,this->y,Level->site[this->x][this->y].roomnumber);
        levelrefresh();
    }
}

void Monster::m_sp_bogthing()
{
    if (Player.status[IMMOBILE] &&
            (distance(Player.x,Player.y,this->x,this->y) < 2)) {
        if (! Player.status[AFRAID]) {
            mprint("As the bogthing touches you, you feel a frisson of terror....");
            if (Player.immunity[FEAR]) mprint("which you shake off.");
            else Player.status[AFRAID]+=2;
        }
        else {
            mprint("The bogthing's touch causes you scream in agony!");
            p_damage(50,UNSTOPPABLE,"fright");
            mprint("Your struggles grow steadily weaker....");
            Player.con--;
            Player.str--;
            if ((Player.con < 3) || (Player.str < 3))
                p_death("congestive heart failure");
        }
    }
}

void Monster::m_sp_were()
{
    int mid;
    if (m_statusp(this,HOSTILE) || (Phase == 6)) {
        do mid = random_range(ML9-NML_0)+ML1;
        /* log npc, 0th level npc, high score npc or were-creature */
        while (mid == NPC || mid == ZERO_NPC ||
                mid == HISCORE_NPC || mid == WEREHUMAN ||
                (Monsters[mid].uniqueness != COMMON) ||
                (! m_statusp(&(Monsters[mid]),MOBILE)) ||
                (! m_statusp(&(Monsters[mid]),HOSTILE))
              );
        this->id = Monsters[mid].id;
        this->hp += Monsters[mid].hp;
        this->status |= Monsters[mid].status;
        this->ac = Monsters[mid].ac;
        this->dmg = Monsters[mid].dmg;
        this->speed = Monsters[mid].speed;
        this->immunity |= Monsters[mid].immunity;
        this->xpv += Monsters[mid].xpv;
        this->corpseWeight = Monsters[mid].corpseWeight;
        this->symbol = Monsters[mid].symbol;
        this->talkf = Monsters[mid].talkf;
        this->meleef = Monsters[mid].meleef;
        this->strikef = Monsters[mid].strikef;
        this->specialf = Monsters[mid].specialf;
        strcpy(Str1,"were-");
        strcat(Str1,Monsters[mid].name);
        strcpy(Str2,"dead were-");
        strcat(Str2,Monsters[mid].name);
        this->name = salloc(Str1);
        this->corpseString = salloc(Str2);
        m_status_set( this, ALLOC );
        this->immunity |= pow2(NORMAL_DAMAGE); /* WDT: not +=, rather |=. */
        if (los_p(this->x,this->y,Player.x,Player.y))
            mprint("You witness a hideous transformation!");
        else mprint("You hear a distant howl.");
    }
}

void Monster::m_sp_servant()
{
    if ((this->id == SERV_LAW) && (Player.alignment < 0))
        m_status_set(this,HOSTILE);
    else if ((this->id == SERV_CHAOS) && (Player.alignment > 0))
        m_status_set(this,HOSTILE);
}

void Monster::m_sp_av()
{
    if (Player.mana > 0) {
        mprint("You feel a sudden loss of mana!");
        Player.mana -= (max(0,10-distance(this->x,this->y,Player.x,Player.y)));
        dataprint();
    }
}

void Monster::m_sp_lw()
{
    if (random_range(2)) {
        if (Level->site[this->x][this->y].locchar == FLOOR) {
            Level->site[this->x][this->y].locchar = LAVA;
            Level->site[this->x][this->y].p_locf = L_LAVA;
            lset(this->x, this->y, CHANGED);
        }
        else if (Level->site[this->x][this->y].locchar == WATER) {
            Level->site[this->x][this->y].locchar = FLOOR;
            Level->site[this->x][this->y].p_locf = L_NO_OP;
            lset(this->x, this->y, CHANGED);
        }
    }
}

void Monster::m_sp_angel()
{
    int mid,hostile = false;
    switch(this->aux1) {
    case ATHENA:
    case ODIN:
        hostile = ((Player.patron == HECATE) || (Player.patron == SET));
        break;
    case SET:
    case HECATE:
        hostile = ((Player.patron == ODIN) || (Player.patron == ATHENA));
        break;
    case DESTINY:
        hostile = (Player.patron != DESTINY);
        break;
    }
    if (hostile)
        m_status_set(this,HOSTILE);
    if (m_statusp(this,HOSTILE)) {
        mprint("The angel summons a heavenly host!");
        switch(this->level) {
        case 9:
            mid = HIGH_ANGEL;
            break;
        case 8:
            mid = ANGEL;
            break;
        default:
        case 6:
            mid = PHANTOM;
            break;
        }
        summon(-1,mid);
        summon(-1,mid);
        summon(-1,mid);
        /* prevent angel from summoning infinitely */
        this->specialf = M_NO_OP;
    }
}

/* Could completely fill up level */
void Monster::m_sp_swarm()
{
    if (random_range(5)==1) {
        if (view_los_p(this->x,this->y,Player.x,Player.y))
            mprint("The swarm expands!");
        else mprint("You hear an aggravating humming noise.");
        summon(-1,SWARM);
    }
}

/* raise nearby corpses from the dead.... */
void Monster::m_sp_raise()
{
    int x,y;
    pol t;
    for(x=this->x-2; x<=this->x+2; x++)
        for(y=this->y-2; y<=this->y+2; y++)
            if (inbounds(x,y))
                if (Level->site[x][y].things != NULL)
                    if (Level->site[x][y].things->thing->id == CORPSEID) {
                        mprint("The Zombie Overlord makes a mystical gesture...");
                        summon(-1,Level->site[x][y].things->thing->charge);
                        t = Level->site[x][y].things;
                        Level->site[x][y].things = Level->site[x][y].things->next;
                        free_obj( t->thing, true );
                        free((char *) t);
                    }
}

void Monster::m_sp_mb()
{
    if (distance(this->x,this->y,Player.x,Player.y)==1) {
        mprint("The manaburst explodes!");
        if (m_statusp(this,HOSTILE)) {
            mprint("You get blasted!");
            p_damage(random_range(100),UNSTOPPABLE,"a manaburst");
            mprint("You feel cold all over!");
            Player.pow-=3;
            Player.iq--;
            Player.con--;
            Player.str-=2;
            Player.dex--;
            Player.agi--;
            dispel(-1);
            /* DAG -- need calc_melee/calcmana here, because of new stats */
            calc_melee();
            Player.maxmana = calcmana();
            Player.mana = min( Player.mana, Player.maxmana );
        }
        else {
            mprint("You feel toasty warm inside!");
            Player.pow++;
            Player.mana = max(Player.mana,calcmana());
            Player.hp = max(Player.hp,++Player.maxhp);
        }
        /* DAG -- to fix the explode but not die manaburst bug */
        this->m_remove();
    }
}

void Monster::m_sp_mirror()
{
    int i,x,y;
    if (view_los_p(this->x,this->y,Player.x,Player.y)) {
        if (random_range(20)+6 < this->level) {
            summon(-1,this->id);
            mprint("You hear the sound of a mirror shattering!");
        }
        else for(i=0; i<5; i++) {
                x = this->x + random_range(13)-6;
                y = this->y + random_range(13)-6;
                if (inbounds(x,y)) {
                    Level->site[x][y].showchar = this->symbol;
                    putspot(x,y,this->symbol);
                }
            }
    }
}

void Monster::m_illusion()
{
    int i = random_range(NUMMONSTERS);
    if (i==NPC || i==HISCORE_NPC || i==ZERO_NPC) i = this->id; /* can't imitate NPC */
    if (Player.status[TRUESIGHT]) {
        this->symbol = Monsters[this->id].symbol;
        this->name = Monsters[this->id].name;
    }
    else  if (random_range(5) == 1) {
        this->symbol = Monsters[i].symbol;
        this->name = Monsters[i].name;
    }
}

void Monster::m_huge_sounds()
{
    if (m_statusp(this,AWAKE) &&
            (! los_p(this->x,this->y,Player.x,Player.y)) &&
            (random_range(10) == 1))
        mprint("The dungeon shakes!");
}

void Monster::m_thief_f()
{
    int i = stolen_item();
    if (random_range(3) == 1) {
        if (distance(Player.x,Player.y,this->x,this->y) < 2) {
            if (p_immune(THEFT) || (Player.level > (this->level*2)+random_range(20)))
                mprint("You feel secure.");
            else {
                if (i == ABORT)
                    mprint("You feel fortunate.");
                else if ((Player.possessions[i]->used) ||
                         (Player.dex < this->level*random_range(10))) {
                    mprint("You feel a sharp tug.... You hold on!");
                }
                else {
                    mprint("You feel uneasy for a moment.");
                    if (this->uniqueness == COMMON) {
                        strcpy(Str2,"The ");
                        strcat(Str2,this->name);
                    }
                    else strcpy(Str2,this->name);
                    strcat(Str2," suddenly runs away for some reason.");
                    mprint(Str2);
                    this->m_teleport();
                    this->movef = M_MOVE_SCAREDY;
                    this->specialf = M_MOVE_SCAREDY;
                    this->m_pickup(Player.possessions[i]);
                    conform_unused_object(Player.possessions[i]);
                    Player.possessions[i] = NULL;
                }
            }
        }
    }
}

void Monster::m_summon()
{
    if ((distance(Player.x,Player.y,this->x,this->y) < 2) &&
            (random_range(3) == 1)) {
        summon(0,-1);
        summon(0,-1);
    }
}

void Monster::m_aggravate()
{

    if (m_statusp(this,HOSTILE)) {
        if (this->uniqueness == COMMON) {
            strcpy(Str2,"The ");
            strcat(Str2,this->name);
        }
        else strcpy(Str2,this->name);
        strcat(Str2," emits an irritating humming sound.");
        mprint(Str2);
        aggravate();
        m_status_reset(this,HOSTILE);
    }
}

void Monster::m_sp_merchant()
{
    MonsterList* ml;
    if (m_statusp(this,HOSTILE))
        if (Current_Environment == E_VILLAGE) {
            mprint("The merchant screams: 'Help! Murder! Guards! Help!'");
            mprint("You hear the sound of police whistles and running feet.");
            for (ml=Level->mlist; ml!=NULL; ml=ml->next) {
                m_status_set(ml->monster,AWAKE);
                m_status_set(ml->monster,HOSTILE);
            }
            this->specialf = M_NO_OP;
        }
}

/* The special function of the various people in the court of the archmage */
/* and the sorcerors' circle */
void Monster::m_sp_court()
{
    MonsterList* ml;
    if (m_statusp(this,HOSTILE)) {
        mprint("A storm of spells hits you!");
        for(ml=Level->mlist; ml!=NULL; ml=ml->next) {
            m_status_set(ml->monster,HOSTILE);
            ml->monster->m_sp_spell();
            if (ml->monster->specialf == M_SP_COURT)
                ml->monster->specialf = M_SP_SPELL;
        }
    }
}

/* The special function of the dragons in the dragons' lair */
void Monster::m_sp_lair()
{
    MonsterList* ml;
    if (m_statusp(this,HOSTILE)) {
        mprint("You notice a number of dragons waking up....");
        mprint("You are struck by a quantity of firebolts.");
        morewait();
        for(ml=Level->mlist; ml!=NULL; ml=ml->next)
            if (ml->monster->hp > 0 && ml->monster->specialf == M_SP_LAIR) {
                m_status_set(ml->monster,HOSTILE);
                fbolt(ml->monster->x,ml->monster->y,Player.x,Player.y,100,100);
                if (ml->monster->id == DRAGON_LORD)
                    ml->monster->specialf = M_SP_DRAGONLORD;
                else
                    ml->monster->specialf = M_STRIKE_FBOLT;
            }
    }
}

void Monster::m_sp_prime()
{
    if (m_statusp(this,HOSTILE)) {
        mprint("The prime sorceror gestures and a pentacular gate opens!");
        mprint("You are surrounded by demons!");
        summon(-1,DEMON_PRINCE);
        summon(-1,DEMON_PRINCE);
        summon(-1,DEMON_PRINCE);
        summon(-1,DEMON_PRINCE);
    }
    this->specialf = M_SP_SPELL;
}
