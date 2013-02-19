/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* mstrike.c */
/* monster strike functions */

#include "glob.h"

void Monster::m_firebolt()
{
    fbolt(this->x,this->y,Player.x,Player.y,this->hit,this->dmg);
}

void Monster::m_nbolt()
{
    nbolt(this->x,this->y,Player.x,Player.y,this->hit,this->dmg);
}

void Monster::m_lball()
{
    lball(this->x,this->y,Player.x,Player.y,this->dmg);
}

void Monster::m_fireball()
{
    fball(this->x,this->y,Player.x,Player.y,this->dmg);
}

void Monster::m_snowball()
{
    snowball(this->x,this->y,Player.x,Player.y,this->dmg);
}

void Monster::m_blind_strike()
{

    MonsterList* ml;
    if ((Player.status[BLINDED] == 0) &&
            los_p(this->x,this->y,Player.x,Player.y) &&
            (distance(this->x,this->y,Player.x,Player.y) < 5)) {
        if (this->uniqueness == COMMON) {
            strcpy(Str2,"The ");
            strcat(Str2,this->name);
        }
        else strcpy(Str2,this->name);
        strcat(Str2," gazes at you menacingly");
        mprint(Str2);
        if (! p_immune(GAZE)) {
            mprint("You've been blinded!");
            Player.status[BLINDED] = random_range(4)+1;
            for(ml=Level->mlist; ml!=NULL; ml=ml->next)
                plotspot(ml->monster->x,ml->monster->y,false);
        }
        else mprint("You gaze steadily back....");
    }
}




void Monster::m_strike_sonic()
{
    if (this->uniqueness == COMMON) {
        strcpy(Str2,"The ");
        strcat(Str2,this->name);
    }
    else strcpy(Str2,this->name);
    strcat(Str2," screams at you!");
    mprint(Str2);
    p_damage(this->dmg,OTHER_MAGIC,"a sonic blast");
}

