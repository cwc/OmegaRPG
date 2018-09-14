/* omega copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* time.c */

/* this file deals with the passage of time in omega */

#include "glob.h"

/* This function coordinates monsters and player actions, as well as
random events. Each tick is a second. There are therefore 60 ticks to
the minute and 60 minutes to the hour.
*/

void time_clock(int reset)
{
    int env;
    MonsterList* ml, **prev;

    ++Tick;

    if (Tick > 60)
    {
        Tick = 0;
        minute_status_check(); /* see about some player statuses each minute */

        ++Time;
        if (0 == (Time % 10)) tenminute_check();
    }

    if (reset) Tick = (Player.click = 0);

    env = Current_Environment;
    while ((Tick == Player.click) && (Current_Environment != E_COUNTRYSIDE) &&
            Current_Environment == env) {
        if (!State.getSkipPlayer())
            do {
                State.setSkipMonsters(false);
                if ((! Player.status[SLEPT])  &&
                        (Current_Environment != E_COUNTRYSIDE)) p_process();
            } while (State.getSkipMonsters() &&
                     (Current_Environment != E_COUNTRYSIDE));
        else State.setSkipPlayer( false );
        Player.click = (Player.click + Command_Duration) % 60;
    }

    /* klugy but what the heck. w/o this line, if the player caused
    a change-environment to the country, the monsters on the old Level
    will still act, causing all kinds of anomalies and core dumps,
    intermittently. However, any other environment change will reset
    Level appropriately, so only have to check for countryside */

    if (Current_Environment != E_COUNTRYSIDE) {

        prev = &(Level->mlist);
        ml = *prev;
        while (ml)
            if (ml->monster->hp > 0) {
                /* following is a hack until I discover source of phantom monsters */
                if (Level->site[ml->monster->x][ml->monster->y].creature != ml->monster)
                    fix_phantom(ml->monster);
                if (Tick == ml->monster->click) {
                    ml->monster->click += ml->monster->speed;
                    while (ml->monster->click > 60) ml->monster->click -= 60;
                    ml->monster->m_pulse();
                }
                prev = &(ml->next);
                ml = ml->next;
            }
            else if (ml->monster != Arena_Monster) {
                *prev = ml->next;
                /* DAG free the name & corpseString if allocated */
                if ( m_statusp( ml->monster, ALLOC ) )
                {
                    free( (char *)ml->monster->name );
                    free( (char *)ml->monster->corpseString );
                }
                free((char *) ml->monster);
                free((char *) ml);
                ml = *prev;
            }
            else
                ml = ml->next;
    }
}

/* remedies occasional defective monsters */
void fix_phantom(Monster* m)
{
    if (Level->site[m->x][m->y].creature == NULL) {
        mprint("You hear a sound like a sigh of relief....");
        Level->site[m->x][m->y].creature = m;
    }
    else {
        mprint("You hear a puff of displaced air....");
        findspace(&(m->x),&(m->y),-1);
        Level->site[m->x][m->y].creature = m;
        m->m_death();

        /* PGM: shouldn't this be
         * m_remove? m_death causes an experience gain.*/

    }
}
