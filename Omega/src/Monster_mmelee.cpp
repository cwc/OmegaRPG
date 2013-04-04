/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* mmelee */
/* various functions to do with monster melee */

#include "glob.h"

void Monster::m_hit(int dtype)
{
    if (this->uniqueness == COMMON) {
        strcpy(Str3,"a ");
        strcat(Str3,this->name);
    }
    else strcpy(Str3,this->name);
    if ((Player.status[DISPLACED] > 0) && (random_range(2) == 1))
        mprint("The attack was displaced!");
    else  p_damage(random_range(this->dmg),dtype,Str3);
}

/* execute monster attacks versus player */
void Monster::tacmonster()
{
    int i=0;
    drawvision(Player.x,Player.y);
    transcribe_monster_actions();
    while ((i < strlen(this->combatManeuvers)) && (this->hp > 0)) {
        if (this->uniqueness == COMMON) {
            strcpy(Str4,"The ");
            strcat(Str4,this->name);
        }
        else strcpy(Str4,this->name);
        if (this->combatManeuvers[i] == 'A') {
            strcat(Str4," attacks ");
            strcat(Str4,actionlocstr(this->combatManeuvers[i+1]));
            if (Verbosity == VERBOSE) mprint(Str4);
            monster_melee(this->combatManeuvers[i+1],0);
        }
        else if (this->combatManeuvers[i] == 'L') {
            strcat(Str4," lunges ");
            strcat(Str4,actionlocstr(this->combatManeuvers[i+1]));
            if (Verbosity == VERBOSE) mprint(Str4);
            monster_melee(this->combatManeuvers[i+1],this->level);
        }
        i+=2;
    }
}

void Monster::monster_melee(char hitloc, int bonus)
{
    if (player_on_sanctuary())
        print1("The aegis of your deity protects you!");
    else {
        /* being attacked wakes you up/stops fast move */
        State.setFastMove(false);

        /* It's lawful to wait to be attacked */
        if (this->wasAttackedByPlayer == false) Player.alignment++;

        this->wasAttackedByPlayer = true;

        if (this->uniqueness == COMMON) {
            strcpy(Str2,"The ");
            strcat(Str2,this->name);
        }
        else strcpy(Str2,this->name);
        if (monster_hit(hitloc,bonus))
            switch(this->meleef) {
            case M_NO_OP:
                strcat(Str2," touches you.");
                mprint(Str2);
                break;
            case M_MELEE_NORMAL:
                strcat(Str2," hits you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                break;
            case M_MELEE_NG:
                strcat(Str2," hits you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                if (random_range(5)==3) this->m_sp_ng();
                break;
            case M_MELEE_FIRE:
                strcat(Str2," blasts you with fire.");
                mprint(Str2);
                m_hit(FLAME);
                break;
            case M_MELEE_DRAGON:
                strcat(Str2," hits you and blasts you with fire.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                m_hit(FLAME);
                break;
            case M_MELEE_ELEC:
                strcat(Str2," lashes you with electricity.");
                mprint(Str2);
                m_hit(ELECTRICITY);
                break;
            case M_MELEE_COLD:
                strcat(Str2," freezes you with cold.");
                mprint(Str2);
                m_hit(ELECTRICITY);
                break;
            case M_MELEE_POISON:
                strcat(Str2," hits you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                if (random_range(10) < this->level) {
                    mprint("You've been poisoned!");
                    p_poison(this->dmg);
                }
                break;
            case M_MELEE_GRAPPLE:
                strcat(Str2," grabs you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                Player.status[IMMOBILE]++;
                break;
            case M_MELEE_SPIRIT:
                strcat(Str2," touches you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                drain_life(this->level);
                break;
            case M_MELEE_DISEASE:
                strcat(Str2," hits you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                if (random_range(10) < this->level) {
                    mprint("You've been infected!");
                    disease(this->level);
                }
                break;
            case M_MELEE_SLEEP:
                strcat(Str2," hit you.");
                mprint(Str2);
                m_hit(NORMAL_DAMAGE);
                if (random_range(10) < this->level) {
                    mprint("You feel drowsy");
                    sleep_player(this->level);
                }
                break;
            }
        else {
            if (random_range(10)) strcat(Str2," missed you.");
            else {
                if (Verbosity == TERSE) {
                    switch(random_range(10)) {
                    case 0:
                        strcat(Str2," blundered severely.");
                        this->m_damage(this->dmg,UNSTOPPABLE);
                        break;
                    case 1:
                        strcat(Str2," tripped while attacking.");
                        this->m_dropstuff();
                        break;
                    case 2:
                        strcat(Str2," seems seriously confused.");
                        this->speed = min(30,this->speed*2);
                        break;
                    default:
                        strcat(Str2," missed you.");
                    }
                }
                switch(random_range(10)) {
                case 0:
                    strcat(Str2," flailed stupidly at you.");
                    break;
                case 1:
                    strcat(Str2," made you laugh.");
                    break;
                case 2:
                    strcat(Str2," blundered severely.");
                    this->m_damage(this->dmg,UNSTOPPABLE);
                    break;
                case 3:
                    strcat(Str2," tripped while attacking.");
                    this->m_dropstuff();
                    break;
                case 4:
                    strcat(Str2," seems seriously confused.");
                    this->speed = min(30,this->speed*2);
                    break;
                case 5:
                    strcat(Str2," is seriously ashamed.");
                    break;
                case 6:
                    strcat(Str2," made a boo-boo.");
                    break;
                case 7:
                    strcat(Str2," blundered.");
                    break;
                case 8:
                    strcat(Str2," cries out in anger and frustration.");
                    break;
                case 9:
                    strcat(Str2," curses your ancestry.");
                    break;
                }
            }
            mprint(Str2);
        }
    }
}

/* checks to see if player hits with hitmod vs. monster this at location hitloc */
int Monster::monster_hit(char hitloc, int bonus)
{
    int i=0,blocks=false,goodblocks=0,hit,riposte=false;
    while (i<strlen(Player.combatManeuvers)) {
        if ((Player.combatManeuvers[i] == 'B') || (Player.combatManeuvers[i] == 'R')) {
            blocks = true;
            if (hitloc == Player.combatManeuvers[i+1]) {
                goodblocks++;
                if (Player.combatManeuvers[i] == 'R') riposte = true;
            }
        }
        i+=2;
    }
    if (! blocks) goodblocks = -1;
    hit = hitp(this->hit+bonus,Player.defense+goodblocks*10);
    if ((! hit) && (goodblocks > 0)) {
        if (Verbosity == VERBOSE) mprint("You blocked it!");
        if (riposte) {
            if (Verbosity != TERSE) mprint("You got a riposte!");
            if (hitp(Player.hit,this->ac)) {
                mprint("You hit!");
                weapon_use(0,Player.possessions[O_WEAPON_HAND],this);
            }
            else mprint("You missed.");
        }
    }
    return(hit);
}

/* decide monster actions in tactical combat mode */
/* if monster is skilled, it can try see the player's attacks coming and
   try to block appropriately. */
void Monster::transcribe_monster_actions()
{
    int i;
    char attack_loc,block_loc;
    static char mmstr[80];

    int p_blocks[3];
    int p_attacks[3];

    for(i=0; i<3; i++)p_blocks[i] = p_attacks[i] = 0;

    /* Find which area player blocks and attacks least in */
    i = 0;
    while (i<strlen(Player.combatManeuvers)) {
        if ((Player.combatManeuvers[i] == 'B') ||
                (Player.combatManeuvers[i] == 'R')) {
            if (Player.combatManeuvers[i+1] == 'H') p_blocks[0]++;
            if (Player.combatManeuvers[i+1] == 'C') p_blocks[1]++;
            if (Player.combatManeuvers[i+1] == 'L') p_blocks[2]++;
        }
        else if ((Player.combatManeuvers[i] == 'A') ||
                 (Player.combatManeuvers[i] == 'L')) {
            if (Player.combatManeuvers[i+1] == 'H') p_attacks[0]++;
            if (Player.combatManeuvers[i+1] == 'C') p_attacks[1]++;
            if (Player.combatManeuvers[i+1] == 'L') p_attacks[2]++;
        }
        i+=2;
    }

    if ((p_blocks[2] <= p_blocks[1]) &&
            (p_blocks[2] <= p_blocks[0]))
        attack_loc = 'L';
    else if ((p_blocks[1] <= p_blocks[2]) &&
             (p_blocks[1] <= p_blocks[0]))
        attack_loc = 'C'; /* DG 12/31/98 */
    else attack_loc = 'H';
    if ((p_attacks[2] <= p_attacks[1]) &&
            (p_attacks[2] <= p_attacks[0]))
        block_loc = 'L';
    else if ((p_attacks[1] <= p_attacks[2]) &&
             (p_attacks[1] <= p_attacks[0]))
        block_loc = 'C';
    else block_loc = 'H';

    this->combatManeuvers = mmstr;

    if (this->id != NPC)
        strcpy(this->combatManeuvers,Monsters[this->id].combatManeuvers);
    else {
        strcpy(this->combatManeuvers,"");
        for(i=0; i<this->level; i+=2)
            strcat(this->combatManeuvers,"L?R?");
    }

    i = 0;
    while (i<strlen(this->combatManeuvers)) {
        if ((this->combatManeuvers[i] == 'A') || (this->combatManeuvers[i] == 'L')) {
            if (this->combatManeuvers[i+1] == '?') {
                if (this->level+random_range(30) > Player.level+random_range(20))
                    this->combatManeuvers[i+1] = attack_loc;
                else this->combatManeuvers[i+1] = random_loc();
            }
            else if (this->combatManeuvers[i+1] == 'X') this->combatManeuvers[i+1] = random_loc();
        }
        else if ((this->combatManeuvers[i] == 'B') || (this->combatManeuvers[i] == 'R')) {
            if (this->combatManeuvers[i+1] == '?') {
                if (this->level+random_range(30) > Player.level+random_range(20))
                    this->combatManeuvers[i+1] = block_loc;
                else this->combatManeuvers[i+1] = random_loc();
            }
            else if (this->combatManeuvers[i+1] == 'X') this->combatManeuvers[i+1] = random_loc();
        }
        i+=2;
    }
}

char random_loc(void)
{
    switch(random_range(3)) {
    case 0:
        return('H');
    case 1:
        return('C');
    default:
        return('L');
    }
}
