/* copyright (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* utils.c */

/* Random utility functions called from all over */

#include "glob.h"

void setPlayerXY(int x, int y)
{
    Player.x = x;
    Player.y = y;
}

/* x and y on level? */
int inbounds(int x, int y)
{
    if (Current_Environment != E_COUNTRYSIDE)
    {
        return((x>=0)&&(y>=0)&&(x<(Level->level_width))&&(y<(Level->level_length)));
    }
    else
        return((x>=0)&&(y>=0)&&(x<COUNTRY_WIDTH)&&(y<COUNTRY_LENGTH));
}

/* RANDFUNCTION is defined in odefs.h */
int random_range(unsigned k)
{
    return( k==0 ? 0 : (RANDFUNCTION() % k) ) ;
}


/* modify absolute y coord relative to which part of level we are on */
int screenmody(int y)
{
    return(y-ScreenOffset);
}

/* modify absolute x coord relative to which part of level we are on */
/* PGM - horizontal scrolling */
int screenmodx(int x)
{
    return(x-ScreenXOffset);
}

/* this could be optimized; is it neccessary? PGM */
int offscreen(int x, int y)
{

    /* PGM : The Country not acually being a level, it is naturally a
     * special case... PGM */

    int width = COUNTRY_WIDTH;
    int length = COUNTRY_LENGTH;

    int xyes, yyes;

    if (Current_Environment != E_COUNTRYSIDE)
    {
        width = Level->level_width;
        length = Level->level_length;
    }

    yyes = ((y<0)||
            (y<ScreenOffset) ||
            (y>ScreenOffset+ScreenLength-1) ||
            (y>length));

    xyes = ((x<0)||
            (x<ScreenXOffset) ||
            (x>ScreenXOffset+ScreenWidth-1) ||
            (x>width));

    return ( xyes || yyes );
}

/* always hit on a natural 0; never hit on a natural 19 */
int hitp(int hit, int ac)
{
    int roll = random_range(20);
    if (roll == 0) return(true);
    else if (roll == 19) return(false);
    else return((roll < (hit - ac)) ? true : false );
}

/* number of moves from x1,y1 to x2,y2 */
int distance(int x1, int y1, int x2, int y2)
{
    return(max(abs(x2-x1),abs(y2-y1)));
}

/* can you shoot, or move monsters through a spot? */
int unblocked(int x, int y)
{
    if ((! inbounds(x,y)) ||
            (Level->site[x][y].creature != NULL) ||
            (Level->site[x][y].locchar == WALL) ||
            (Level->site[x][y].locchar == PORTCULLIS) ||
            (Level->site[x][y].locchar == STATUE) ||
            (Level->site[x][y].locchar == HEDGE) ||
            (Level->site[x][y].locchar == CLOSED_DOOR) ||
            loc_statusp(x,y,SECRET) ||
            ((x==Player.x) && (y==Player.y)))
        return(false);
    else
        return(true);
}


/* do monsters want to move through a spot */
int m_unblocked(Monster* m, int x, int y)
{
    if ((! inbounds(x,y)) || ((x==Player.x) && (y==Player.y)))
        return(false);
    else if ((Level->site[x][y].creature != NULL) ||
             (Level->site[x][y].locchar == SPACE)) return(false);
    else if (m_statusp(m,ONLYSWIM))
        return(Level->site[x][y].locchar == WATER);
    else if (loc_statusp(x,y,SECRET)) {
        if (m->movef == M_MOVE_SMART) {
            if (los_p(x, y, Player.x, Player.y)) {
                mprint("You see a secret door swing open!");
                lreset(x, y, SECRET);
                lset(x, y, CHANGED);
            }
            else
                mprint("You hear a door creak open, and then close again.");
            /* smart monsters would close secret doors behind them if the */
            /* player didn't see them using it */
            return(true);
        }
        else
            return(m_statusp(m,INTANGIBLE));
    }
    else if ((Level->site[x][y].locchar == FLOOR) ||
             (Level->site[x][y].locchar == OPEN_DOOR))
        return(true);
    else if ((Level->site[x][y].locchar == PORTCULLIS) ||
             (Level->site[x][y].locchar == WALL) ||
             (Level->site[x][y].locchar == STATUE))
        return(m_statusp(m,INTANGIBLE));
    else if (Level->site[x][y].locchar==WATER)
        return(m_statusp(m,SWIMMING) ||
               m_statusp(m,ONLYSWIM) ||
               m_statusp(m,INTANGIBLE) ||
               m_statusp(m,FLYING));
    else if (Level->site[x][y].locchar == CLOSED_DOOR) {
        if (m->movef==M_MOVE_SMART) {
            mprint("You hear a door creak open.");
            Level->site[x][y].locchar = OPEN_DOOR;
            lset(x, y, CHANGED);
            return(true);
        }
        else if (random_range(m->dmg) > random_range(100)) {
            mprint("You hear a door shattering.");
            Level->site[x][y].locchar = RUBBLE;
            lset(x, y, CHANGED);
            return(true);
        }
        else return(m_statusp(m,INTANGIBLE));
    }
    else if (Level->site[x][y].locchar == LAVA)
        return((m_immunityp(m,FLAME) &&
                m_statusp(m,SWIMMING)) ||
               m_statusp(m,INTANGIBLE) ||
               m_statusp(m,FLYING));
    else if (Level->site[x][y].locchar == FIRE)
        return(m_statusp(m,INTANGIBLE) ||
               m_immunityp(m,FLAME));
    else if ((Level->site[x][y].locchar == TRAP) ||
             (Level->site[x][y].locchar == HEDGE) ||
             (Level->site[x][y].locchar == ABYSS))
        return((m->movef == M_MOVE_CONFUSED) ||
               m_statusp(m,INTANGIBLE) ||
               m_statusp(m,FLYING));
    else return(true);
}



/* can you see through a spot? */
int view_unblocked(int x, int y)
{
    if (! inbounds(x,y)) return(false);
    else if ((Level->site[x][y].locchar == WALL) ||
             (Level->site[x][y].locchar == STATUE) ||
             (Level->site[x][y].locchar == HEDGE) ||
             (Level->site[x][y].locchar == FIRE) ||
             (Level->site[x][y].locchar == CLOSED_DOOR) ||
             loc_statusp(x,y,SECRET))
        return(false);
    else
        return(true);
}

/* do_los moves pyx along a lineofsight from x1 to x2 */
/* x1 and x2 are pointers because as a side effect they are changed */
/* to the final location of the pyx */
void do_los(Symbol pyx, int *x1, int *y1, int x2, int y2)
{
    int dx,dy,ox,oy;
    int major, minor;
    int error, delta, step;
    int blocked;

    if (x2 - *x1 < 0) dx = 5;
    else if (x2 - *x1 > 0) dx = 4;
    else dx = -1;
    if (y2 - *y1 < 0) dy = 7;
    else if (y2 - *y1 > 0) dy = 6;
    else dy = -1;
    if (abs(x2 - *x1) > abs(y2 - *y1)) {
        major = dx;
        minor = dy;
        step = abs(x2 - *x1);
        delta = 2*abs(y2 - *y1);
    }
    else {
        major = dy;
        minor = dx;
        step = abs(y2 - *y1);
        delta = 2*abs(x2 - *x1);
    }
    if (major == -1)	/* x1,y2 already == x2,y2 */
        return;
    error = 0;
    do {
        ox = *x1;
        oy = *y1;
        *x1 += Dirs[0][major];
        *y1 += Dirs[1][major];
        error += delta;
        if (error > step) {	/* don't need to check that minor >= 0 */
            *x1 += Dirs[0][minor];
            *y1 += Dirs[1][minor];
            error -= 2*step;
        }
        blocked = !unblocked(*x1,*y1);
        if (error < 0 && (*x1 != x2 || *y1 != y2) && blocked) {
            *x1 -= Dirs[0][minor];
            *y1 -= Dirs[1][minor];
            error += 2*step;
            blocked = !unblocked(*x1,*y1);
        }
        Level->site[*x1][*y1].showchar = pyx;
        plotchar(pyx,*x1,*y1);
        plotspot(ox,oy,true);
        usleep(50000);
    } while ((*x1 != x2 || *y1 != y2) && !blocked);
    plotspot(*x1,*y1,true);
    levelrefresh();
}


/* This is the same as do_los, except we stop before hitting nonliving
obstructions */
void do_object_los(Symbol pyx, int *x1, int *y1, int x2, int y2)
{
    int dx,dy,ox,oy;
    int major, minor;
    int error, delta, step;
    int blocked;

    if (x2 - *x1 < 0) dx = 5;
    else if (x2 - *x1 > 0) dx = 4;
    else dx = -1;
    if (y2 - *y1 < 0) dy = 7;
    else if (y2 - *y1 > 0) dy = 6;
    else dy = -1;
    if (abs(x2 - *x1) > abs(y2 - *y1)) {
        major = dx;
        minor = dy;
        step = abs(x2 - *x1);
        delta = 2*abs(y2 - *y1);
    }
    else {
        major = dy;
        minor = dx;
        step = abs(y2 - *y1);
        delta = 2*abs(x2 - *x1);
    }
    if (major == -1)	/* x1,y2 already == x2,y2 */
        return;
    error = 0;
    do {
        ox = *x1;
        oy = *y1;
        *x1 += Dirs[0][major];
        *y1 += Dirs[1][major];
        error += delta;
        if (error > step) {	/* don't need to check that minor >= 0 */
            *x1 += Dirs[0][minor];
            *y1 += Dirs[1][minor];
            error -= 2*step;
        }
        blocked = !unblocked(*x1,*y1);
        if (error < 0 && (*x1 != x2 || *y1 != y2) && blocked) {
            *x1 -= Dirs[0][minor];
            *y1 -= Dirs[1][minor];
            error += 2*step;
            blocked = !unblocked(*x1,*y1);
        }
        plotspot(ox,oy,true);
        if (unblocked(*x1,*y1)) {
            plotchar(pyx,*x1,*y1);
            Level->site[*x1][*y1].showchar = pyx;
            usleep(50000);
        }
    } while ((*x1 != x2 || *y1 != y2) && !blocked);
    if (Level->site[*x1][*y1].creature == NULL && blocked) {
        *x1 = ox;
        *y1 = oy;
    }
    plotspot(*x1,*y1,true);
    levelrefresh();
}


/* los_p checks to see whether there is an unblocked los from x1,y1 to x2,y2 */
int los_p(int x1, int y1, int x2, int y2)
{
    int dx,dy;
    int major, minor;
    int error, delta, step;
    int blocked;

    if (x2-x1 < 0) dx = 5;
    else if (x2-x1 > 0) dx = 4;
    else dx = -1;
    if (y2-y1 < 0) dy = 7;
    else if (y2-y1 > 0) dy = 6;
    else dy = -1;
    if (abs(x2-x1) > abs(y2-y1)) {
        major = dx;
        minor = dy;
        step = abs(x2 - x1);
        delta = 2*abs(y2 - y1);
    }
    else {
        major = dy;
        minor = dx;
        step = abs(y2 - y1);
        delta = 2*abs(x2 - x1);
    }
    if (major == -1)	/* x1,y2 already == x2,y2 */
        return true;
    error = 0;
    do {
        x1 += Dirs[0][major];
        y1 += Dirs[1][major];
        error += delta;
        if (error > step) {	/* don't need to check that minor >= 0 */
            x1 += Dirs[0][minor];
            y1 += Dirs[1][minor];
            error -= 2*step;
        }
        blocked = !unblocked(x1,y1);
        if (error < 0 && (x1 != x2 || y1 != y2) && blocked) {
            x1 -= Dirs[0][minor];
            y1 -= Dirs[1][minor];
            error += 2*step;
            blocked = !unblocked(x1,y1);
        }
    } while ((x1 != x2 || y1 != y2) && !blocked);
    return((x1==x2) && (y1==y2));
}


/* view_los_p sees through monsters */
int view_los_p(int x1, int y1, int x2, int y2)
{
    int dx,dy;
    int major, minor;
    int error, delta, step;
    int blocked;

    if (x2-x1 < 0) dx = 5;
    else if (x2-x1 > 0) dx = 4;
    else dx = -1;
    if (y2-y1 < 0) dy = 7;
    else if (y2-y1 > 0) dy = 6;
    else dy = -1;
    if (abs(x2-x1) > abs(y2-y1)) {
        major = dx;
        minor = dy;
        step = abs(x2 - x1);
        delta = 2*abs(y2 - y1);
    }
    else {
        major = dy;
        minor = dx;
        step = abs(y2 - y1);
        delta = 2*abs(x2 - x1);
    }
    if (major == -1)	/* x1,y2 already == x2,y2 */
        return true;
    error = 0;
    do {
        x1 += Dirs[0][major];
        y1 += Dirs[1][major];
        error += delta;
        if (error > step) {
            x1 += Dirs[0][minor];
            y1 += Dirs[1][minor];
            error -= 2*step;
        }
        blocked = !view_unblocked(x1,y1);
        if (error < 0 && (x1 != x2 || y1 != y2) && blocked) {
            x1 -= Dirs[0][minor];
            y1 -= Dirs[1][minor];
            error += 2*step;
            blocked = !view_unblocked(x1,y1);
        }
    } while ((x1 != x2 || y1 != y2) && !blocked);
    return((x1==x2) && (y1==y2));
}

#if !defined(WIN32)
/* returns the command direction from the index into Dirs */
char inversedir(int dirindex)
{
    switch (dirindex) {
    case 0:
        return('n');
    case 1:
        return('u');
    case 2:
        return('b');
    case 3:
        return('y');
    case 4:
        return('l');
    case 5:
        return('h');
    case 6:
        return('j');
    case 7:
        return('k');
    default:
        return('\0');
    }
}
#endif


long calc_points(void)
{
    int i;
    long points=0;

    if (State.getSpokeToDruid()) points += 50;
    if (State.getCompletedCaves()) points += 100;
    if (State.getCompletedSewers()) points += 200;
    if (State.getCompletedCastle()) points += 300;
    if (State.getCompletedAstral()) points += 400;
    if (State.getCompletedVolcano()) points += 500;
    if (State.getKilledDragonlord()) points += 100;
    if (State.getKilledEater()) points += 100;
    if (State.getKilledLawbringer()) points += 100;

    points += Player.xp/50;

    points += Player.cash/500;

    for (i=0; i<MAXITEMS; i++)
        if (Player.possessions[i] != NULL)
            points += Player.possessions[i]->level*(Player.possessions[i]->known+1);

    for (i=0; i<MAXPACK; i++)
        if (Player.pack[i] != NULL)
            points += Player.pack[i]->level*(Player.pack[i]->known+1);

    for (i=0; i<NUMRANKS; i++) {
        if (Player.rank[i] == 5) points += 500;
        else points += 20*Player.rank[i];
    }

    if (Player.hp < 1)
        points = (points / 2);

    else if (Player.rank[ADEPT])
        points *= 10;

    return(points);
}


/* returns the 24 hour clock hour */
int hour(void)
{
    return((int)(((Time+720) / 60) % 24));
}

/* returns 0, 10, 20, 30, 40, or 50 */
int showminute(void)
{
    return((int)((Time % 60)/10)*10);
}

/* returns the 12 hour clock hour */
int showhour(void)
{
    int showtime;
    if ((hour() == 0) || (hour() == 12)) showtime = 12;
    else showtime = (hour() % 12);
    return(showtime);
}

/* nighttime is defined from 9 PM to 6AM */
int nighttime(void)
{
    return((hour() > 20) || (hour() < 7));
}

const char *getarticle(const char *str)
{
    if ((str[0]=='a') || (str[0]=='A') ||
            (str[0]=='e') || (str[0]=='E') ||
            (str[0]=='i') || (str[0]=='I') ||
            (str[0]=='o') || (str[0]=='O') ||
            (str[0]=='u') || (str[0]=='U') ||
            (((str[0]=='h') || (str[0]=='H')) &&
             ((str[1]=='i') || (str[1]=='e'))))
        return("an ");
    else return("a ");
}

int day(void)
{
    return ((Date % 30) + 1);
}

const char *ordinal(int number)
{
    if ((number == 11) || (number == 12) || (number == 13)) return("th");
    else  switch(number % 10) {
        case 1:
            return("st");
        case 2:
            return("nd");
        case 3:
            return("rd");
        default:
            return("th");
        }
}

const char *month(void)
{
    switch((Date % 360) / 30) {
    case 0:
        return("Freeze");
    case 1:
        return("Ice");
    case 2:
        return("Mud");
    case 3:
        return("Storm");
    case 4:
        return("Breeze");
    case 5:
        return("Light");
    case 6:
        return("Flame");
    case 7:
        return("Broil");
    case 8:
        return("Cool");
    case 9:
        return("Haunt");
    case 10:
        return("Chill");
    case 11:
        return("Dark");
    case 12:
        return("Twixt");
    default:
        return("***Error***");
    }
}

/* finds floor space on level with buildaux not equal to baux,
sets x,y there. There must *be* floor space somewhere on level */

static int spaceok (int x_idx, int y_idx, int baux)
{
    if (FLOOR != Level->site[x_idx][y_idx].locchar) return false;
    if (Level->site[x_idx][y_idx].creature) return false;
    if (loc_statusp(x_idx, y_idx, SECRET)) return false;
    if (Level->site[x_idx][y_idx].buildaux == baux) return false;

    return true;
}

static int findspace_method_one (int * x_idx, int * y_idx, struct level * lev, int baux)
{
    int count = 0;

    for (count = 0; count < (lev->level_width * lev->level_length / 4); ++count)
    {
        *x_idx = random_range(lev->level_width);
        *y_idx = random_range(lev->level_length);
        if (spaceok(*x_idx, *y_idx, baux))
            return true;
    }

    return false;
}

static int findspace_method_two (int * x_out, int * y_out, struct level * lev, int baux)
{
    int x_start, x_idx;
    int y_start, y_idx;

    int x_dir, y_dir;

    x_idx = x_start = random_range(lev->level_width);
    y_idx = y_start = random_range(lev->level_length);

    x_dir = random_range(2);
    y_dir = random_range(2);

    if (random_range(2))
    {
        if (x_dir)
        {
            if (y_dir)
            {
                // case 0: X in outer loop, decrement X, decrement Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        --y_idx;
                        if (y_idx == -1)
                            y_idx = lev->level_length - 1;
                    }
                    while (y_idx != y_start);

                    --x_idx;
                    if (x_idx == -1)
                        x_idx = lev->level_width - 1;
                }
                while (x_idx != x_start);
            }
            else
            {
                // case 1: X in outer loop, decrement X, increment Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        ++y_idx;
                        if (y_idx == lev->level_length)
                            y_idx = 0;
                    }
                    while (y_idx != y_start);

                    --x_idx;
                    if (x_idx == -1)
                        x_idx = lev->level_width - 1;
                }
                while (x_idx != x_start);
            }
        }
        else
        {
            if (y_dir)
            {
                // case 2: X in outer loop, increment X, decrement Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        --y_idx;
                        if (y_idx == -1)
                            y_idx = lev->level_length - 1;
                    }
                    while (y_idx != y_start);

                    ++x_idx;
                    if (x_idx == lev->level_width)
                        x_idx = 0;
                }
                while (x_idx != x_start);
            }
            else
            {
                // case 3: X in outer loop, increment X, increment Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        ++y_idx;
                        if (y_idx == lev->level_length)
                            y_idx = 0;
                    }
                    while (y_idx != y_start);

                    ++x_idx;
                    if (x_idx == lev->level_width)
                        x_idx = 0;
                }
                while (x_idx != x_start);
            }
        }
    }
    else
    {
        if (y_dir)
        {
            if (x_dir)
            {
                // case 4: Y in outer loop, decrement X, decrement Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        --x_idx;
                        if (x_idx == -1)
                            x_idx = lev->level_width - 1;
                    }
                    while (x_idx != x_start);

                    --y_idx;
                    if (y_idx == -1)
                        y_idx = lev->level_length - 1;
                }
                while (y_idx != y_start);
            }
            else
            {
                // case 5: Y in outer loop, decrement X, increment Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        ++x_idx;
                        if (x_idx == lev->level_width)
                            x_idx = 0;
                    }
                    while (x_idx != x_start);

                    --y_idx;
                    if (y_idx == -1)
                        y_idx = lev->level_length - 1;
                }
                while (y_idx != y_start);
            }
        }
        else
        {
            if (x_dir)
            {
                // case 6: Y in outer loop, increment X, decrement Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        --x_idx;
                        if (x_idx == -1)
                            x_idx = lev->level_width - 1;
                    }
                    while (x_idx != x_start);

                    ++y_idx;
                    if (y_idx == lev->level_length)
                        y_idx = 0;
                }
                while (y_idx != y_start);
            }
            else
            {
                // case 7: Y in outer loop, increment X, increment Y
                do
                {
                    do
                    {
                        if (spaceok(x_idx, y_idx, baux))
                            goto found_space;

                        ++x_idx;
                        if (x_idx == lev->level_width)
                            x_idx = 0;
                    }
                    while (x_idx != x_start);

                    ++y_idx;
                    if (y_idx == lev->level_length)
                        y_idx = 0;
                }
                while (y_idx != y_start);
            }
        }
    }

    return false;

found_space:
    *x_out = x_idx;
    *y_out = y_idx;
    return true;
}

void findspace (int * x_out, int * y_out, int baux)
{
    /* try to quickly find a space by sampling randomly */
    if (findspace_method_one(x_out, y_out, Level, baux))
        return;

    /* examine every space on the level */
    if (findspace_method_two(x_out, y_out, Level, baux))
        return;

    /* this could theoretically happen but if it does, it's an error */
    assert(false);
}

/* is prefix a prefix of s? */
int strprefix(char *prefix, char *s)
{
    int i=0,matched=true;
    if (strlen(prefix) > strlen(s)) return(false);
    else {
        while (matched && (i<strlen(prefix))) {
            matched = (prefix[i] == s[i]);
            i++;
        }
        return(matched);
    }
}

/* WSS - Maybe I'm not understanding something here, but isn't "strmem" just */
/* WSS - a lame version of the standard library function strchr? */

/* is character c a member of string s */
int strmem(char c, char *s)
{
    int i,found=false;
    for(i=0; ((i<strlen(s)) && (! found)); i++)
        found = (s[i] == c);
    return(found);
}

void calc_weight(void)
{
    int i,weight=0;

    for(i=1; i<MAXITEMS; i++)
        if (Player.possessions[i] != NULL)
            weight += Player.possessions[i]->weight *	Player.possessions[i]->number;
    if ((Player.possessions[O_WEAPON_HAND] != NULL) &&
            (Player.possessions[O_READY_HAND] == Player.possessions[O_WEAPON_HAND]))
        weight -= Player.possessions[O_READY_HAND]->weight *
                  Player.possessions[O_READY_HAND]->number;
    for(i=0; i<MAXPACK; i++)
        if (Player.pack[i] != NULL)
            weight += Player.pack[i]->weight *
                      Player.pack[i]->number;
    Player.itemweight = weight;
    dataprint();
}

char joinString[256], temp[256];
char *strjoin(const char *one, const char *two)
{
    assert(one != two);
    if (joinString == one) goto DirectJoin;
    if (joinString == two) {
        strcpy(temp,two);
        two = temp;
    }
    strcpy(joinString,one);
DirectJoin:
    strcat(joinString,two);
    return joinString;
}

/* returns true if its ok to get rid of a level */
int ok_to_free(plv level)
{
    if (level == NULL) return(false);
    else return((level->environment != E_CITY) &&
                    (level->environment != E_VILLAGE) &&
                    (level->environment != Current_Dungeon));
}

/* DAG frees object; if flag true, final free, free any allocated corpse string */
void free_obj( Object* obj, int flag )
{
    if ( flag && (obj->id == CORPSEID) && (obj->level & ALLOC) )
        free( (char *)obj->objstr );
    if(obj->id == FOODID+0) {
        delete obj;
    } else {
        free( (char *) obj );
    }
}

Object* copy_obj ( Object* obj )
{
    Object* newObject;
    newObject = ((Object*) checkmalloc(sizeof(Object)));
    *newObject = *obj;
    if ( (obj->id == CORPSEID) && (obj->level & ALLOC) )
    {
        newObject->objstr = newObject->cursestr = newObject->truename = salloc( obj->objstr );
    }
    return newObject;
}

void free_objlist(pol po)
{
    pol tmp;

    while (po) {
        free_obj( (tmp = po)->thing, true );
        po = po->next;
        free(tmp);
    }
}

void free_mons_and_objs(MonsterList* mlist)
{
    MonsterList* tmp;

    while (mlist) {
        free_objlist((tmp = mlist)->monster->possessions);
        /* DAG free the name & corpseString if allocated */
        if ( m_statusp( tmp->monster, ALLOC ) )
        {
            free( (char *)tmp->monster->name );
            free( (char *)tmp->monster->corpseString );
        }
        free(tmp->monster);
        mlist = mlist->next;
        free(tmp);
    }
}

/* Free up monsters and items on a level*/
void free_level(plv level)
{
    int i,j;

    free_mons_and_objs(level->mlist);
    for (i = 0; i < level->level_width; i++)
        for (j = 0; j < level->level_length; j++)
            if (level->site[i][j].things) {
                free_objlist(level->site[i][j].things);
                level->site[i][j].things = NULL;
            }

    free(level);
}

/* malloc function that checks its return value - if NULL, tries to free */
/* some memory... */
void *checkmalloc(unsigned int bytes)
{
    void *ptr = malloc(bytes);
    struct level *curr, **prev, **oldest;

    if (ptr)
        return ptr;
    for (curr = Dungeon, oldest = prev = &Dungeon; curr; curr = curr->next) {
        if ((*oldest)->last_visited > curr->last_visited)
            oldest = prev;
        prev = &(curr->next);
    }
    if (*oldest && *oldest != Level) {
        curr = *oldest;
        *oldest = (*oldest)->next;
        free_level(curr);
        ptr = malloc(bytes);
    }
    if (ptr)
        return ptr;
    else {
        print1("Out of memory!  Saving and quitting.");
        morewait();
        save(true);
        endgraf();
        exit(0);
    }
}

/* alloc just enough string space for str, strcpy, and return pointer */
const char *salloc(const char *str)
{
    char *s = (char*) checkmalloc((unsigned)(strlen(str)+1));
    strcpy(s,str);
    return(s);
}

#if defined(WIN32)
/* ****Moved here from another file**** */
/* reads a string from a file. If it is a line with more than 80 char's,
   then remainder of line to \n is consumed */
void filescanstring(FILE *fd, char *fstr)
{
    int i= -1;
    int byte='x';
    while ((i<80) && (byte != '\n') && (byte != EOF)) {
        i++;
        byte=fgetc(fd);
        fstr[i] = byte;
    }
    if (byte != '\n')
        while((byte!='\n') && (byte != EOF))
            byte=fgetc(fd);
    fstr[i]=0;
}
#endif

#if defined(WIN32)
/* ****Moved here from another file**** */
/* returns a "level of difficulty" based on current environment
   and depth in dungeon. Is somewhat arbitrary. value between 1 and 10.
   May not actually represent real difficulty, but instead level
   of items, monsters encountered.    */
int difficulty(void)
{
    int depth = 1;
    if (Level != NULL) depth = Level->depth;
    switch(Current_Environment) {
    case E_COUNTRYSIDE:
        return(7);
    case E_CITY:
        return(3);
    case E_VILLAGE:
        return(1);
    case E_TACTICAL_MAP:
        return(7);
    case E_SEWERS:
        return(depth/6)+3;
    case E_CASTLE:
        return(depth/4)+4;
    case E_CAVES:
        return(depth/3)+1;
    case E_VOLCANO:
        return(depth/4)+5;
    case E_ASTRAL:
        return(8);
    case E_ARENA:
        return(5);
    case E_HOVEL:
        return(3);
    case E_MANSION:
        return(7);
    case E_HOUSE:
        return(5);
    case E_DLAIR:
        return(9);
    case E_ABYSS:
        return(10);
    case E_STARPEAK:
        return(9);
    case E_CIRCLE:
        return(8);
    case E_MAGIC_ISLE:
        return(8);
    case E_TEMPLE:
        return(8);

        /* PGM why is this duplicated in lev.c ? */
    case E_PALACE:  /* varies by phase of moon */
    {
        int diff = 0;
        switch(Phase/2) {
        case 0:
            diff = 0;
            break;
        case 1:
        case 11:
            diff = 1;
            break;
        case 2:
        case 10:
            diff = 2;
            break;
        case 3:
        case 9:
            diff = 3;
            break;
        case 4:
        case 8:
            diff = 5;
            break;
        case 5:
        case 7:
            diff = 6;
            break;
        case 6:
            diff = 7;
            break;
        }
        return( min( ((depth+diff)/3)+4,9));
    }
    default:
        return(3);
    }
}
#endif

char cryptkey(const char *fname)
{
    int pos, key = 0;

    for (pos = 0; fname[pos]; pos++)
        key += 3*(fname[pos] - ' ');
    return (key&0xff);
}

int game_uid;
int user_uid;

void init_perms(void)
{
#if defined(UNIX)
    user_uid = getuid();
    game_uid = geteuid();
#endif
}

void change_to_user_perms(void)
{
#ifdef UNIX
    seteuid(user_uid);
#endif
}

void change_to_game_perms(void)
{
#ifdef UNIX
    seteuid(game_uid);
#endif
}
