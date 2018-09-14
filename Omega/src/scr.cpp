/* omega (c) 1987,1988,1989 by Laurence Raphael Brothers */
/* scr.c */
/* functions that use curses routines directly */
/* plus a few file i/o stuff */
/* also some in file.c */

#define CHARATTR(c)	((c) & ~0xff)

#include "glob.h"

// Screen flicker fix
#undef wclear
#define wclear werase

WINDOW *Bankw;

/* note these variables are not exported to other files */

WINDOW *Levelw,*Dataw,*Flagw,*Timew,*Menuw,*Locw,*Morew,*Phasew;
WINDOW *Comwin,*Msg1w,*Msg2w,*Msg3w,*Msgw;

WINDOW *Cinema;
/* WDT: interactions and special effects which currently
 * go into the message window should instead go here.  This
 * should simplify history-keeping, since we only need to
 * record actual messages, not special effects. */

WINDOW *Showline[MAXITEMS];

WINDOW *Wizlistw;

#define SL3_LEN 160
char screenLine3[SL3_LEN];

/* static function prototypes */

static long input_number (WINDOW * w);

/* function definitions */

void phaseprint(void)
{
    wclear(Phasew);
    wprintw(Phasew,"Moon's Phase:\n");
    switch(Phase/2) {
    case 0:
        wprintw(Phasew,"NEW");
        break;
    case 1:
    case 11:
        wprintw(Phasew,"CRESCENT");
        break;
    case 2:
    case 10:
        wprintw(Phasew,"1/4");
        break;
    case 3:
    case 9:
        wprintw(Phasew,"HALF");
        break;
    case 4:
    case 8:
        wprintw(Phasew,"3/4");
        break;
    case 5:
    case 7:
        wprintw(Phasew,"GIBBOUS");
        break;
    case 6:
        wprintw(Phasew,"FULL");
        break;
    }
    wrefresh(Phasew);
}

static void show_screen_level (int top, int left, int bottom, int right)
{
    int j;
    int last_attr = 0;

    for (j = top; j < bottom; ++j)
    {
        int i;

        wmove(Levelw, screenmody(j), 0);

        for (i = left; i < right; ++i)
        {
            int c;

            if (loc_statusp(i, j, SEEN))
                c = getspot(i, j, false);
            else
                c = SPACE;

            if (optionp(SHOW_COLOUR) && CHARATTR(c) != last_attr)
            {
                last_attr = CHARATTR(c);
                wattrset(Levelw, last_attr);
            }

            waddch(Levelw, c & 0xff);
        }
    }
}

static void show_screen_country (int top, int left, int bottom, int right)
{
    int j;
    int last_attr = 0;

    for (j = top; j < bottom; ++j)
    {
        int i;

        wmove(Levelw, screenmody(j), 0);

        for (i = left; i < right; ++i)
        {
            int c;

            if (c_statusp(i, j, SEEN))
                c = Country[i][j].current_terrain_type;
            else
                c = SPACE;

            if (optionp(SHOW_COLOUR) && CHARATTR(c) != last_attr)
            {
                last_attr = CHARATTR(c);
                wattrset(Levelw, last_attr);
            }

            waddch(Levelw, c & 0xff);
        }
    }
}

void show_screen(void)
{
    int top;
    int left;
    int right;
    int bottom;

    wclear(Levelw);

    if (ScreenOffset < 0) ScreenOffset = 0;
    if (ScreenXOffset < 0) ScreenXOffset = 0;

    top = ScreenOffset;
    left = ScreenXOffset;
    right = ScreenXOffset + ScreenWidth;
    bottom = ScreenOffset + ScreenLength;

    if (Current_Environment != E_COUNTRYSIDE)
    {
        if (right > Level->level_width) right = Level->level_width;
        if (bottom > Level->level_length) bottom = Level->level_length;
    }
    else
    {
        if (right > COUNTRY_WIDTH) right = COUNTRY_WIDTH;
        if (bottom > COUNTRY_LENGTH) bottom = COUNTRY_LENGTH;
    }

    if (Current_Environment != E_COUNTRYSIDE)
    {
        show_screen_level(top, left, bottom, right);
    }
    else
    {
        show_screen_country(top, left, bottom, right);
#if 0
        for (j=top; j<bottom; j++)
        {
            for (i=left; i<right; i++)
            {
                wmove(Levelw,screenmody(j),screenmodx(i));
                c = ((c_statusp(i,j,SEEN)) ? Country[i][j].current_terrain_type:SPACE);
                if (optionp(SHOW_COLOUR) && CHARATTR(c) != last_attr)
                {
                    last_attr = CHARATTR(c);
                    wattrset(Levelw,last_attr);
                }
                waddch(Levelw,c&0xff);
            }
            wrefresh(Levelw);
        }
#endif
    }
}

// Ensure that curses provides the following key codes
// Note: Some linux terminals do not provide key codes for all shifted arrow keys
//       it depends on the type of terminal used.
//       eg. xterm provides codes for all shifted keys, but kconsole does not support
//           shifted-left arrow or shifted-right arrow (up and down do work though...)
#ifndef CTL_UP
#define CTL_UP 568
#endif
#ifndef CTL_DOWN
#define CTL_DOWN 527
#endif
#ifndef CTL_LEFT
#define CTL_LEFT 547
#endif
#ifndef CTL_RIGHT
#define CTL_RIGHT 562
#endif

#ifndef KEY_SUP
#define KEY_SUP 337
#endif
#ifndef KEY_SDOWN
#define KEY_SDOWN 336
#endif
#ifndef KEY_LEFT
#define KEY_SUP 393
#endif
#ifndef KEY_SRIGHT
#define KEY_SDOWN 402
#endif

// Catch CTRL key sequences and remap to standard character key.
int remapspecials(int c)
{
	switch (c)
	{
	case CTL_LEFT:
		while((c = wgetch(Msgw)) == CTL_LEFT);
		if (c == CTL_UP) c = 'y';
		if (c == CTL_DOWN) c = 'b';
		break;
	case CTL_RIGHT:
		while((c = wgetch(Msgw)) == CTL_RIGHT);
		if (c == CTL_UP) c = 'u';
		if (c == CTL_DOWN) c = 'n';
		break;
	case CTL_UP:
		while((c = wgetch(Msgw)) == CTL_UP);
		if (c == CTL_LEFT) c = 'y';
		if (c == CTL_RIGHT) c = 'u';
		break;
	case CTL_DOWN:
		while((c = wgetch(Msgw)) == CTL_DOWN);
		if (c == CTL_LEFT) c = 'b';
		if (c == CTL_RIGHT) c = 'n';
		break;
	}

	return c;
}

int mgetc(void)
{
	// get user input
	int c = wgetch(Msgw);

	// check & handle specials
	c = remapspecials(c);

	return c;
}

/* case insensitive mgetc -- sends uppercase to lowercase */
int mcigetc(void)
{
	// get user input
    int c = mgetc();

	// convert user input to lowercase if required
	if ((c>=(int)'A') && (c<=(int)'Z'))
        return(c+(int)('a'-'A'));
    else 
		return(c);
}

char menugetc(void)
{
    return(wgetch(Menuw));
}

char lgetc(void)
{
    return(wgetch(Levelw));
}

int ynq(void)
{
    int p='*'; /* the user's choice; start with something impossible
              * to prevent a loop. */
    while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE) &&
            (p != EOF) && (p != ' '))
        p = wgetch(Msgw);
    switch (p) {
    case 'y':
        wprintw(Msgw,"yes. ");
        break;
    case 'n':
        wprintw(Msgw,"no. ");
        break;

    case ESCAPE:
        p = 'q'; /* fall through to 'q' */
    case ' ':
        p = 'q';    /* fall through to 'q' */
    case 'q':
        wprintw(Msgw,"quit. ");
        break;
    default:
        assert( p == EOF );
    }
    wrefresh(Msgw);
    return(p);
}


int ynq1(void)
{
    int p='*'; /* the user's choice; start with something impossible
              * to prevent a loop. */
    while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE) &&
            (p != ' ') && (p != EOF))
        p = wgetch(Msg1w);
    switch (p) {
    case 'y':
        wprintw(Msg1w,"yes. ");
        break;
    case 'n':
        wprintw(Msg1w,"no. ");
        break;

    case ESCAPE:
        p = 'q'; /* fall through to 'q' */
    case ' ':
        p = 'q';    /* fall through to 'q' */
    case 'q':
        wprintw(Msg1w,"quit. ");
        break;
    default:
        assert( p == EOF );
    }
    wrefresh(Msg1w);
    return(p);
}


int ynq2(void)
{
    int p='*'; /* the user's choice; start with something impossible
              * to prevent a loop. */
    while ((p != 'n') && (p != 'y') && (p != 'q') && (p != ESCAPE) &&
            (p != ' ') && (p != EOF))
        p = wgetch(Msg2w);
    switch (p) {
    case 'y':
        wprintw(Msg2w,"yes. ");
        break;
    case 'n':
        wprintw(Msg2w,"no. ");
        break;

    case ESCAPE:
        p = 'q'; /* fall through to 'q' */
    case ' ':
        p = 'q';    /* fall through to 'q' */
    case 'q':
        wprintw(Msg2w,"quit. ");
        break;
    default:
        assert( p == EOF );
    }
    wrefresh(Msg2w);
    return(p);
}

void erasemsg3 (void)
{
    werase(Msg3w);
    memset(screenLine3, 0, SL3_LEN);
}

/* puts up a morewait to allow reading if anything in top two lines */
void checkclear(void)
{
    int x1,y,x2;
    getyx(Msg1w,x1,y);
    getyx(Msg2w,x2,y);
    if ((x1 != 0) || (x2 != 0)) {
        morewait();
        wclear(Msg1w);
        wclear(Msg2w);
        wrefresh(Msg1w);
        wrefresh(Msg2w);
    }
}

/* for external call */
void clearmsg(void)
{
    wclear(Msg1w);
    wclear(Msg2w);
    erasemsg3();
    Msgw = Msg1w;
    wrefresh(Msg1w);
    wrefresh(Msg2w);
    wrefresh(Msg3w);
}

void clearmsg3(void)
{
    erasemsg3();
    wrefresh(Msg3w);
}

void clearmsg1(void)
{
    wclear(Msg1w);
    wclear(Msg2w);
    Msgw = Msg1w;
    wrefresh(Msg1w);
    wrefresh(Msg2w);
}

void erase_level(void)
{
    wclear(Levelw);
    wrefresh(Levelw);
}

/* direct print to first msg line */
void print1(const char *s)
{
    if (! State.getSuppressPrinting()) {
        buffercycle(s);
        wclear(Msg1w);
        wprintw(Msg1w,s);
        wrefresh(Msg1w);
    }
}

/* for run on-messages -- print1 clears first.... */
void nprint1(const char *s)
{
    if (! State.getSuppressPrinting()) {
        if (bufferappend(s)) {
            wprintw(Msg1w,s);
            wrefresh(Msg1w);
        }
    }
}

/* direct print to second msg line */
void print2(const char *s)
{
    if (! State.getSuppressPrinting()) {
        buffercycle(s);
        wclear(Msg2w);
        wprintw(Msg2w,s);
        wrefresh(Msg2w);
    }
}

/* for run on-messages -- print2 clears first.... */
void nprint2(const char *s)
{
    if (! State.getSuppressPrinting()) {
        if (bufferappend(s)) {
            wprintw(Msg2w,s);
            wrefresh(Msg2w);
        }
    }
}


/* msg line 3 is not part of the region that mprint or printm can reach */
/* typical use of print3 is for "you can't do that" type error messages */
/* WDT: it's also typically used for messages intended to persist a while.
 * Thus I'm having print3 save its input in a global variable which other
 * things can use. */
void print3 (const char *s)
{
    if (!State.getSuppressPrinting())
    {
        buffercycle(s);
        erasemsg3();
        wprintw(Msg3w, s);
        strcpy(screenLine3, s);
        wrefresh(Msg3w);
    }
}

/* for run on-messages -- print3 clears first.... */
void nprint3(const char *s)
{
    if (!State.getSuppressPrinting())
    {
        if (bufferappend(s))
        {
            strcat(screenLine3, s);
            wprintw(Msg3w,s);
            wrefresh(Msg3w);
        }
    }
}



/* prints wherever cursor is in window, but checks to see if
it should morewait and clear window */
void mprint(const char *s)
{
    int x,y;
    if (! State.getSuppressPrinting()) {
        getyx(Msgw,y,x);
        if (x+strlen(s) >= ScreenWidth) {
            buffercycle(s);
            if (Msgw == Msg1w) {
                wclear(Msg2w);
                Msgw = Msg2w;
            }
            else {
                morewait();
                wclear(Msg1w);
                wclear(Msg2w);
                wrefresh(Msg2w);
                Msgw = Msg1w;
            }
        }
        else if (x > 0)
            bufferappend(s);
        else
            buffercycle(s);
        wprintw(Msgw,s);
        waddch(Msgw,' ');
        wrefresh(Msgw);
    }
}


void omega_title(void)
{
    showmotd();
    clear();
    touchwin(stdscr);
    refresh();
    /*  showscores();*/ /* DG */
}



/* blanks out ith line of Menuw or Levelw */
void hide_line(int i)
{
    wclear(Showline[i]);
    touchwin(Showline[i]);
    wrefresh(Showline[i]);
}

#ifdef USE_OPCURSES
static void load_tiles (void)
{
    char * path;

    path = malloc(strlen(OMEGALIB) + strlen(TILEFILE) + 1);
    if (0 == path) return;

    strcpy(path, OMEGALIB);
    strcat(path, TILEFILE);

    set_tiles(path);

    free(path);
}
#endif

/* initialize, screen, windows */
void initgraf(void)
{
    int i;

#ifdef USE_OPCURSES
    load_tiles();
#endif

    initscr();
    start_color();
    clrgen_init();

    if (LINES < 24 || COLS < 80) {
        printf("Minimum Screen Size: 24 Lines by 80 Columns.");
        exit(0);
    }
    ScreenLength = LINES - 6;
    ScreenWidth = 64; /* PGM */
    Msg1w = newwin(1,80,0,0);
    scrollok(Msg1w, 0);
    Msg2w = newwin(1,80,1,0);
    scrollok(Msg2w, 0);
    Msg3w = newwin(1,80,2,0);
    scrollok(Msg3w, 0);
    Msgw = Msg1w;
    Morew = newwin(1,15,3,65);
    scrollok(Morew, 0);

    // Bank window
    Bankw = newwin( 20, 50, ( ScreenLength - 20 ) / 2 + 3, 7 );
    scrollok( Bankw, 0 );

    // Wiz flag list window
    Wizlistw = newwin( 20, 50, 3, 0 );
    scrollok( Wizlistw, 0 );

    Locw = newwin(1,80,ScreenLength+3,0);
    scrollok(Locw, 0);
    Levelw = newwin(ScreenLength,ScreenWidth,3,0);
    scrollok(Levelw, 0);
    for(i=0; i<MAXITEMS; i++) {
        Showline[i] = newwin(1,ScreenWidth,i+3,0);
        scrollok(Showline[i], 0);
        wclear(Showline[i]);
    }
    Menuw = newwin(ScreenLength,ScreenWidth,3,0);
    scrollok(Menuw, 0);
    Dataw = newwin(2,80,ScreenLength+4,0);
    scrollok(Dataw, 0);
    Timew = newwin(2,15,4,65);
    scrollok(Timew, 0);
    Phasew = newwin(2,15,6,65);
    scrollok(Phasew, 0);
    Flagw = newwin(4,15,9,65);
    scrollok(Flagw, 0);
    Comwin = newwin(8,15,14,65);
    scrollok(Comwin, 0);
    Cinema = newwin(3,80,0,0); /* the same position and size as the three
                              * message windows. */
    scrollok(Cinema,0);

    noecho();
    crmode();

    clear();
    touchwin(stdscr);

	// SL: Allow input of extended characters
	keypad(Msgw, TRUE);
	keypad(Menuw, TRUE);
	keypad(Levelw, TRUE);
	keypad(Msg1w, TRUE);
	keypad(Msg2w, TRUE);
	keypad(stdscr, TRUE);

    /*  omega_title();*/
    /*  clear();*/
    /*  touchwin(stdscr);*/
    /*  refresh();*/          /* DG */
}

/* cinema_display: pop up a window out of the way of the screen and fill it
 * with 3 lines of text.  The lines may be NULL pointers. */
void cinema_display (const char *line1, const char *line2, const char *line3)
{
    werase(Cinema);

    if (line1)
    {
        if (line2)
        {
            if (line3)
            {
                mvwprintw(Cinema, 0, 0, line1);
                mvwprintw(Cinema, 1, 0, line2);
                mvwprintw(Cinema, 2, 0, line3);
            }
            else
            {
                mvwprintw(Cinema, 2, 0, screenLine3);
                mvwprintw(Cinema, 0, 0, line1);
                mvwprintw(Cinema, 1, 0, line2);
            }
        }
        else
        {
            if (line3)
            {
                mvwprintw(Cinema, 0, 0, line1);
                mvwprintw(Cinema, 2, 0, line3);
            }
            else
            {
                mvwprintw(Cinema, 2, 0, screenLine3);
                mvwprintw(Cinema, 0, 0, line1);
            }
        }
    }
    else
    {
        if (line2)
        {
            if (line3)
            {
                mvwprintw(Cinema, 1, 0, line2);
                mvwprintw(Cinema, 2, 0, line3);
            }
            else
            {
                mvwprintw(Cinema, 2, 0, screenLine3);
                mvwprintw(Cinema, 1, 0, line2);
            }
        }
        else
        {
            if (line3)
            {
                mvwprintw(Cinema, 2, 0, line3);
            }
            else
            {
                mvwprintw(Cinema, 2, 0, screenLine3);
            }
        }
    }

    wrefresh(Cinema);
}

void cinema_blank (void)
{
    werase(Cinema);
}

static void cinema_blank_line (int line)
{
    int x;
    int maxy, maxx;

    getmaxyx(Cinema, maxy, maxx);
    wmove(Cinema, line, 0);

    for (x = 0; x < maxx; ++x)
        waddch(Cinema, ' ');

    wrefresh(Cinema);
}

void cinema_print_line (int line, const char * text)
{
    mvwprintw(Cinema, line, 0, text);
    wrefresh(Cinema);
}

/* Doggone it, there ought to be a way to simply hide a window, thereby
 * redrawing anything which happened to be under it. */
void cinema_hide (void)
{
    touchwin(Msg1w);
    wrefresh(Msg1w);
    touchwin(Msg2w);
    wrefresh(Msg2w);
    touchwin(Msg3w);
    wrefresh(Msg3w);
}

int cinema_getch (void)
{
    int response;
    response = wgetch(Cinema);
    if (EOF == response || ESCAPE == response) response = -1;
    return response;
}

/* cinema_interact: pop up a window out of the way of the screen, fill
 * it with up to three lines of text, wait for user input, close the
 * window, and return the user's input.
 *
 * Params:
 *
 *   choices: a \0-terminated array containing the characters which are
 * 	   acceptable responses to this interaction.  ESC is always
 * 	   acceptable, regardless of choices.  \0 is never acceptable
 * 	   and may not be embedded in the string.
 *
 *   line1-3: The 3 lines of text, each at most 80 chars long.  Any may
 *         be NULL, and none may contain CRs or escape sequences.
 *
 * Return Value:
 *   -1 if ESC was pressed; otherwise the character chosen.
 */
int cinema_interact (const char *choices, const char *line1, const char *line2, const char *line3)
{
    int response;

    assert(choices && strlen(choices) > 0);

    while (1)
    {
        cinema_display(line1, line2, line3);
        response = cinema_getch();
        if (-1 == response) break;
        if (strchr(choices, response)) break;
        if (!line3) line3 = "What? Try again!";
    }

    cinema_hide();
    return response;
}

int cinema_interact_line (int line, const char * choices, const char * text)
{
    int response;

    assert(0 == line || 1 == line);
    assert(choices && strlen(choices) > 0);

    while (1)
    {
        cinema_print_line(line, text);
        response = cinema_getch();
        if (ABORT == response) break;
        if (strchr(choices, response)) break;
        cinema_print_line(2, "What? Try again!");
    }

    return response;
}

int cinema_ynq_line (int line, const char * prompt)
{
    return cinema_interact_line(line, "ynqYNQ", prompt);
}

int cinema_getnum_line (int line, const char * prompt)
{
    int num;

    assert(0 == line || 1 == line);

    cinema_blank_line(line);
    cinema_print_line(line, prompt);
    num = (int)input_number(Cinema);

    return num;
}

int cinema_confirm (const char * action_description)
{
    const char *msg;

    switch (random_range(4))
    {
    case 0:
        msg = "Are you sure? [yn] ";
        break;
    case 1:
        msg = "Certain about that? [yn] ";
        break;
    case 2:
        msg = "Do you really mean it? [yn] ";
        break;
    default:
        msg = "Confirm that, would you? [yn] ";
        break;
    }

    return cinema_interact("ynqYNQ", action_description, msg, NULL);
}

int cinema_ynq (const char *action_description)
{
    return cinema_interact("ynqYNQ", action_description, NULL, NULL);
}

void cinema_scene (const char *line1, const char *line2, const char *line3)
{
    cinema_display(line1, line2, line3);
    morewait();
    cinema_hide();
}

int lastx= -1,lasty= -1;

void drawplayer(void)
{
    int c;

    if (E_COUNTRYSIDE == Current_Environment)
    {
        if (inbounds(lastx, lasty) && !offscreen(lastx, lasty))
        {
            wmove(Levelw, screenmody(lasty), screenmodx(lastx));
            c = Country[lastx][lasty].current_terrain_type;

            if (optionp(SHOW_COLOUR)) wattrset(Levelw, CHARATTR(c));

            waddch(Levelw, c & 0xff);
        }

        wmove(Levelw, screenmody(Player.y), screenmodx(Player.x));

        if (optionp(SHOW_COLOUR)) wattrset(Levelw, CHARATTR(PLAYER));

        waddch(Levelw, PLAYER & 0xff);
    }
    else
    {
        if (inbounds(lastx, lasty) && !offscreen(lastx, lasty))
            plotspot(lastx, lasty, (Player.status[BLINDED] > 0 ? false : true));

        wmove(Levelw, screenmody(Player.y), screenmodx(Player.x));

        if ((!Player.status[INVISIBLE]) || Player.status[TRUESIGHT])
        {
            if (optionp(SHOW_COLOUR)) wattrset(Levelw, CHARATTR(PLAYER));
            waddch(Levelw, PLAYER & 0xff);
        }
    }

    lastx = Player.x;
    lasty = Player.y;
}

void setlastxy(int new_x, int new_y) /* used when changing environments */
{
    lastx = new_x;
    lasty = new_y;
}

int litroom(int x, int y)
{
    if (Level->site[x][y].roomnumber < ROOMBASE) return(false);
    else return(loc_statusp(x,y,LIT) ||
                    Player.status[ILLUMINATION]);
}

void drawvision(int x, int y)
{
    static int oldx = -1,oldy = -1;
    int i,j,c;

    if (Current_Environment != E_COUNTRYSIDE) {
        if (Player.status[BLINDED]) {
            drawspot(oldx,oldy);
            drawspot(x,y);
            drawplayer();
        }
        else {
            if (Player.status[ILLUMINATION] > 0) {
                for (i= -2; i<3; i++)
                    for (j= -2; j<3; j++)
                        if (inbounds(x+i,y+j))
                            if (view_los_p(x+i,y+j,Player.x,Player.y))
                                dodrawspot(x+i,y+j);
            }
            else {
                for (i= -1; i<2; i++)
                    for (j= -1; j<2; j++)
                        if (inbounds(x+i,y+j))
                            dodrawspot(x+i,y+j);
            }
            drawplayer();
            drawmonsters(false); /* erase all monsters */
            drawmonsters(true);  /* draw those now visible */
        }
        if ((State.getFastMove() == false) || (! optionp(JUMPMOVE)))
            omshowcursor(Player.x,Player.y);
        oldx = x;
        oldy = y;
    }
    else {
        for (i= -1; i<2; i++)
            for (j= -1; j<2; j++)
                if (inbounds(x+i,y+j)) {
                    c_set(x+i, y+j, SEEN);
                    if (!offscreen(x+i,y+j)) {
                        wmove(Levelw,screenmody(y+j),screenmodx(x+i));
                        c = Country[x+i][y+j].current_terrain_type;
                        if (optionp(SHOW_COLOUR))
                            wattrset(Levelw, CHARATTR(c));
                        waddch(Levelw,(c&0xff));
                    }
                }
        drawplayer();
        omshowcursor(Player.x,Player.y);
    }
}


void omshowcursor(int x, int y)
{
    if (! offscreen(x,y)) {
        wmove(Levelw,screenmody(y),screenmodx(x));
        wrefresh(Levelw);
    }
}

void levelrefresh(void)
{
    wrefresh(Levelw);
}


/* draws a particular spot under if in line-of-sight */
void drawspot(int x, int y)
{
    Symbol c;
    if (inbounds(x,y)) {
        c = getspot(x,y,false);
        if (c != Level->site[x][y].showchar)
            if (view_los_p(Player.x,Player.y,x,y)) {
                lset(x,y,SEEN);
                Level->site[x][y].showchar = c;
                putspot(x,y,c);
            }
    }
}



/* draws a particular spot regardless of line-of-sight */
void dodrawspot(int x, int y)
{
    Symbol c;
    if (inbounds(x,y)) {
        c = getspot(x,y,false);
        if (c != Level->site[x][y].showchar) {
            lset(x,y,SEEN);
            Level->site[x][y].showchar = c;
            putspot(x,y,c);
        }
    }
}

/* write a blank to a spot if it is floor */
void blankoutspot(int i, int j)
{
    if (inbounds(i,j)) {
        lreset(i,j,LIT);
        lset(i, j, CHANGED);
        if (Level->site[i][j].locchar == FLOOR)  {
            Level->site[i][j].showchar = SPACE;
            putspot(i,j,SPACE);
        }
    }
}

/* blank out a spot regardless */
void blotspot(int i, int j)
{
    if (inbounds(i,j)) {
        lreset(i,j,SEEN);
        Level->site[i][j].showchar = SPACE;
        if (! offscreen(i,j)) {
            wmove(Levelw,screenmody(j),screenmodx(i));
            wattrset(Levelw, CHARATTR(SPACE));
            waddch(Levelw, SPACE&0xff);
        }
    }
}


/* for displaying activity specifically at some point */
void plotspot(int x, int y, int showmonster)
{
    if (loc_statusp(x,y,SEEN))
        putspot(x,y,getspot(x,y,showmonster));
    else
        putspot(x,y,SPACE);
}


/* Puts c at x,y on screen. No fuss, no bother. */
void putspot(int x, int y, Symbol c)
{
    if (! offscreen(x,y)) {
        wmove(Levelw,screenmody(y),screenmodx(x));
        if (optionp(SHOW_COLOUR))
            wattrset(Levelw, CHARATTR(c));
        waddch(Levelw,(0xff&c));
    }
}


/* regardless of line of sight, etc, draw a monster */
void plotmon(Monster* m)
{
    if (! offscreen(m->x,m->y)) {
        wmove(Levelw,screenmody(m->y),screenmodx(m->x));
        if (optionp(SHOW_COLOUR))
            wattrset(Levelw, CHARATTR(m->symbol));
        waddch(Levelw,(m->symbol&0xff));
    }
}

/* if display, displays monsters, otherwise erases them */
void drawmonsters(int display)
{
    MonsterList* ml;
    for (ml=Level->mlist; ml!=NULL; ml=ml->next) {
        if (ml->monster->hp > 0) {
            if (display) {
                if (view_los_p(Player.x,Player.y,ml->monster->x,ml->monster->y)) {
                    if (Player.status[TRUESIGHT] || (! m_statusp(ml->monster,M_INVISIBLE))) {
                        if (!optionp(SHOW_COLOUR) &&
                                (ml->monster->level > 5) &&
                                ((ml->monster->symbol&0xff) != '@') &&
                                ((ml->monster->symbol&0xff) != '|')) wstandout(Levelw);
                        putspot(ml->monster->x,ml->monster->y,ml->monster->symbol);
                        if (!optionp(SHOW_COLOUR))
                            wstandend(Levelw);
                    }
                }
            }
            else erase_monster(ml->monster);
        }
    }
}

/* replace monster with what would be displayed if monster weren't there */
void erase_monster(Monster* m)
{
    if (loc_statusp(m->x,m->y,SEEN))
        putspot(m->x,m->y,getspot(m->x,m->y,false));
    else blotspot(m->x,m->y);
}

/* find apt char to display at some location */
Symbol getspot(int x, int y, int showmonster)
{
#ifdef CENTER_ON_PLAYER
    /* funny scrolling may cause this. PGM */
    if (!inbounds(x,y ) )
    {
        return SPACE;
    }
#endif

    if (loc_statusp(x,y,SECRET))
    {
        if ( Level->site[x][y].roomnumber == RS_DRUID )
            return(HEDGE);  /* secret doors in druid temple are brambles -DAG */
        else
            return(WALL);
    }
    else switch (Level->site[x][y].locchar) {
        case WATER:
            if (Level->site[x][y].creature == NULL)
                return(WATER);
            else if (m_statusp(Level->site[x][y].creature,SWIMMING))
                return(WATER);
            else if (showmonster)
                return(Level->site[x][y].creature->symbol);
            else return(WATER);
            /* these sites never show anything but their location char's */
        case CLOSED_DOOR:
        case LAVA:
        case FIRE:
        case ABYSS:
            return(Level->site[x][y].locchar);
            /* rubble and hedge don't show items on their location */
        case RUBBLE:
        case HEDGE:
            if (showmonster && (Level->site[x][y].creature != NULL)) {
                if ((m_statusp(Level->site[x][y].creature,M_INVISIBLE)) &&
                        (! Player.status[TRUESIGHT]))
                    return(getspot(x,y,false));
                else return (Level->site[x][y].creature->symbol);
            }
            else return(Level->site[x][y].locchar);
            /* everywhere else, first try to show monster, next show items, next show
               location char */
        default:
            if (showmonster && (Level->site[x][y].creature != NULL)) {
                if ((m_statusp(Level->site[x][y].creature,M_INVISIBLE)) &&
                        (! Player.status[TRUESIGHT]))
                    return(getspot(x,y,false));
                else return (Level->site[x][y].creature->symbol);
            }
            else if (Level->site[x][y].things != NULL) {
                if (Level->site[x][y].things->next != NULL)
                    return(PILE);
                else return(Level->site[x][y].things->thing->objchar);
            }
            else return(Level->site[x][y].locchar);
        }
}

void commanderror(void)
{
    erasemsg3();
    wprintw(Msg3w, "%c : unknown command", Cmd);
    wrefresh(Msg3w);
}

void timeprint(void)
{
    static const char * old_month = 0;

    /* WSS * HACK * This shouldn't be here at all but time is so screwed up */
    /* WSS * HACK * I don't know where else I can check for a new month reliably */
    if (!old_month)
        old_month = month();
    else
    {
        /* WSS * HACK * ptr comparision... works cause they're static */
        if (old_month != month() && Player.rank[LEGION] > 0)
        {
            bank_account * account;
            account = bank_index_number(SalaryAccount);
            if (account) account->balance += SalaryAmount;
            old_month = month();
        }
    }

    wclear(Timew);
    wprintw(Timew,"%d:%d",showhour(),showminute());
    if (showminute()==0) waddch(Timew,'0');
    wprintw(Timew,hour()>11 ? " PM \n" : " AM \n");
    wprintw(Timew,month());
    wprintw(Timew," the %d",day());
    wprintw(Timew,ordinal(day()));
    wrefresh(Timew);
}


void comwinprint(void)
{
    wclear(Comwin);
    wprintw(Comwin,"Hit: %d  \n",Player.hit);
    wprintw(Comwin,"Dmg: %d  \n",Player.dmg);
    wprintw(Comwin,"Def: %d  \n",Player.defense);
    wprintw(Comwin,"Arm: %d  \n",Player.absorption);
    wprintw(Comwin,"Spd: %d.%d  \n", 5/Player.speed, 500/Player.speed%100);
    wrefresh(Comwin);
}

void dataprint(void)
{
    wclear(Dataw);
    /* WDT HACK: I should make these fields spaced and appropriately justified.
     * Maybe I don't feel like it right now. */
    wprintw(Dataw,"Hp:%d/%d Mana:%ld/%ld Au:%ld Level:%d/%ld Carry:%d/%d \n",
            Player.hp,Player.maxhp,Player.mana,Player.maxmana,Player.cash,
            Player.level,Player.xp,Player.itemweight,Player.maxweight);
    wprintw(Dataw,"Str:%d/%d Con:%d/%d Dex:%d/%d Agi:%d/%d Int:%d/%d Pow:%d/%d   ",
            Player.str,Player.maxstr,Player.con,Player.maxcon,
            Player.dex,Player.maxdex,Player.agi,Player.maxagi,
            Player.iq,Player.maxiq,Player.pow,Player.maxpow);
    wrefresh(Dataw);
}


/* redraw everything currently displayed */
void redraw(void)
{
    touchwin(curscr);
    wrefresh(curscr);
}

/* redraw each permanent window */
void xredraw(void)
{
    touchwin(Msgw);
    touchwin(Msg3w);
    touchwin(Levelw);
    touchwin(Timew);
    touchwin(Flagw);
    touchwin(Dataw);
    touchwin(Locw);
    touchwin(Morew);
    touchwin(Phasew);
    touchwin(Comwin);
    wrefresh(Msgw);
    wrefresh(Msg3w);
    wrefresh(Levelw);
    wrefresh(Timew);
    wrefresh(Flagw);
    wrefresh(Dataw);
    wrefresh(Locw);
    wrefresh(Morew);
    wrefresh(Phasew);
    wrefresh(Comwin);
}



void menuaddch(char c)
{
    waddch(Menuw,c);
    wrefresh(Menuw);
}



void morewait(void)
{
    int display=true;
    int c;
    if (State.getSuppressPrinting())
        return;
    do {
        wclear(Morew);
        if (display) wprintw(Morew,"***  MORE  ***");
        else wprintw(Morew,"+++  MORE  +++");
        display = ! display;
        wrefresh(Morew);
        /* c = wgetch(Msgw); */
        c = wgetch(Morew);
    } while ((c != ' ') && (c != RETURN) && (c != EOF));
    wclear(Morew);
    wrefresh(Morew);
}

int stillonblock(void)
{
    int display=true;
    int c;
    do {
        wclear(Morew);
        if (display) wprintw(Morew,"<<<STAY?>>>");
        else wprintw(Morew,">>>STAY?<<<");
        display = ! display;
        wrefresh(Morew);
        c = wgetch(Msgw);
    } while ((c != ' ') && (c != ESCAPE) && (c != EOF));
    wclear(Morew);
    wrefresh(Morew);
    return (c == ' ');
}

void menuclear(void)
{
    wclear(Menuw);
    touchwin(Menuw);
    wrefresh(Menuw);
}

void menuspellprint(int i)
{
    int x,y;
    getyx(Menuw,y,x);
    if (y >= ScreenLength - 2) {
        wrefresh(Menuw);
        morewait();
        wclear(Menuw);
        touchwin(Menuw);
    }
    wprintw(Menuw,spellid(i));
    wprintw(Menuw,"(%d)\n",Spells[i].powerdrain);
}

void menuprint(const char *s)
{
    int x,y;
    getyx(Menuw,y,x);
    if (y >= ScreenLength - 2) {
        wrefresh(Menuw);
        morewait();
        wclear(Menuw);
        touchwin(Menuw);
    }
    wprintw(Menuw,s);
}

void showmenu(void)
{
    wrefresh(Menuw);
}


void endgraf(void)
{
    clear();
    touchwin(stdscr);
    refresh();
    endwin();
}


void plotchar(Symbol pyx, int x, int y)
{
    if (! offscreen(x,y)) {
        wmove(Levelw,screenmody(y),screenmodx(x));
        if (optionp(SHOW_COLOUR))
            wattrset(Levelw, CHARATTR(pyx));
        waddch(Levelw,(pyx&0xff));
        wrefresh(Levelw);
    }
}



void draw_explosion(Symbol pyx, int x, int y)
{
    int i,j;

    for(j=0; j<3; j++) {
        for(i=0; i<9; i++)
            plotchar(pyx,x+Dirs[0][i],y+Dirs[1][i]);
        usleep(150000);
        for(i=0; i<9; i++)
            plotchar(SPACE,x+Dirs[0][i],y+Dirs[1][i]);
        usleep(150000);
    }
    for(i=0; i<9; i++)
        plotspot(x+Dirs[0][i],y+Dirs[1][i],true);
    wrefresh(Levelw);
}

char *msgscanstring(void)
{
    static char instring[80],byte='x';
    int i=0;

    instring[0]=0;
    byte = mgetc();
    while (byte != '\n') {
        if ((byte == 8) || (byte == 127)) { /* ^h or delete */
            if (i>0) {
                i--;
                dobackspace();
            }
            instring[i] = 0;
        }
        else {
            instring[i] = byte;
            waddch(Msgw,byte);
            wrefresh(Msgw);
            i++;
            instring[i] = 0;
        }
        byte = mgetc();
    }
    return(instring);
}


void locprint(const char *s)
{
    wclear(Locw);
    wprintw(Locw,s);
    wrefresh(Locw);
}

/* draw everything whether visible or not */
void drawscreen(void)
{
    int i,j;
    if (Current_Environment == E_COUNTRYSIDE)
        for (i=0; i<COUNTRY_WIDTH; i++)
            for(j=0; j<COUNTRY_LENGTH; j++)
                c_set(i, j, SEEN);
    else for (i=0; i<Level->level_width; i++)
            for(j=0; j<Level->level_length; j++)
                lset(i,j,SEEN);
    if (Current_Environment == E_CITY)
        for (i = 0; i < NUMCITYSITES; i++)
            CitySiteList[i][0] = 1;
    show_screen();
}

/*selects a number up to range */
/* WDT HACK!  This should use the interaction code, and should allow
 * direct input. */
int getnumber(int range)
{
    int done=false,value=1;
    int atom;

    if (range==1) return(1);
    else while (! done) {
            clearmsg();
            wprintw(Msg1w,"How many? Change with < or >, ESCAPE to select:");
            mnumprint(value);
#if !defined(WIN32)
            do atom=mcigetc();
            while ((atom != '<')&&(atom != '>')&&(atom!=ESCAPE));
            if ((atom=='>') && (value < range)) value++;
            else if ((atom=='<') && (value > 1)) value--;
            else if (atom==ESCAPE) done = true;
#else
            atom=mcigetc();
            switch (atom)
            {
            case '>':
            case 'k':
#ifdef KEY_UP
            case KEY_UP:
#endif
                if (value < range)
                    value++;
                break;
            case '<':
            case 'j':
#ifdef KEY_DOWN
            case KEY_DOWN:
#endif
                if (value > 1)
                    value--;
                break;
#ifdef KEY_HOME
            case KEY_HOME:
#endif
                value = 1;
                break;
#ifdef KEY_LL
            case KEY_LL:
#endif
                value = range;
                break;
            case ESCAPE:
                clearmsg();
                done = true;
                break;
            }
#endif
        }
    return(value);
}

static long input_number (WINDOW * w)
{
    int ch;
    int ypos, xpos;
    int numlen = 0;
    int amount = 0;

    getyx(w, ypos, xpos);

    while (1)
    {
        ch = mvwgetch(w, ypos, xpos);

        if ('\n' == ch || '\r' == ch)
        {
            return amount;
        }
        else if (EOF == ch || ESCAPE == ch)
        {
            return ABORT;
        }
        else if (BACKSPACE == ch || DELETE_CHAR == ch)
        {
            if (numlen > 0)
            {
                --numlen;
                amount /= 10;
                mvwaddch(w, ypos, --xpos, ' ');
            }
        }
        else if (isdigit(ch) && (numlen < 9))
        {
            ++xpos;
            ++numlen;
            waddch(w, ch);
            amount = 10 * amount + (ch - '0');
        }
        else
        {
            beep();
        }
    }
}

/* reads a positive number up to 999999 */
/* WDT: now asks for the number, and receives it through the cinema
 * interface.  Returns ABORT (-1) if aborted. */
long parsenum (const char *message)
{
    int number[8];
    int place = -1;
    int i,x,y,mult=1;
    long num=0;
    char byte=' ';
    int entering_digits = true;

    cinema_display(message, "enter a number or ESC: ", NULL);
    while (entering_digits) {
        byte = wgetch(Cinema);
        if ((byte == BACKSPACE) || (byte == DELETE_CHAR)) {
            if (place > -1) {
                number[place] = 0;
                place--;
                getyx(Cinema,y,x);
                wmove(Cinema,y,x-1);
                waddch(Cinema,' ');
                wmove(Cinema,y,x-1);
                wrefresh(Cinema);
            }
        }
        else if ((byte <= '9') && (byte >= '0') && (place < 7)) {
            place++;
            number[place]=byte;
            waddch(Cinema,byte);
            wrefresh(Cinema);
        }
        else if (isprint(byte)) {
            /* I suppose I should beep here, to indicate that only digits belong.
             * Right now I'm just ignoring them.
             */
        }
        else entering_digits = 0;
    }
    if (byte != '\n') num = ABORT;
    else {
        for (i=place; i>=0; i--) {
            num += mult*(number[i]-'0');
            mult*=10;
        }
    }
    cinema_hide();
    return num;
}



void maddch(char c)
{
    waddch(Msgw,c);
    wrefresh(Msgw);
}


void display_death(const char *source)
{
    clear();
    touchwin(stdscr);
    printw("\n\n\n\n");
    printw("Requiescat In Pace, ");
    printw(Player.name);
    printw(" (%ld points)",calc_points());
    strcpy(Str4,"Killed by ");
    strcat(Str4,source);
    printw("\n");
    printw(Str4);
    printw(".");
    printw("\n\n\n\n\nHit 'c' to continue.");
    refresh();
    while (wgetch(stdscr) != 'c')
        ;
    clear();
    touchwin(stdscr);
    refresh();
    extendlog(Str4,DEAD);
}


void display_win(void)
{
    clear();
    touchwin(stdscr);
    printw("\n\n\n\n");
    printw(Player.name);
    if (Player.rank[ADEPT]) {
        printw(" is a total master of omega with %ld points!",FixedPoints);
        strcpy(Str4,"A total master of omega");
    }
    else {
        strcpy(Str4,"retired a winner");
        printw(" triumphed in omega with %ld points!",calc_points());
    }
    printw("\n\n\n\n\nHit 'c' to continue.");
    refresh();
    while (wgetch(stdscr) != 'c')
        ;
    clear();
    touchwin(stdscr);
    refresh();
    if (Player.rank[ADEPT])
        extendlog(Str4,BIGWIN);
    else extendlog(Str4,WIN);
}

void display_quit(void)
{
    clear();
    touchwin(stdscr);
    printw("\n\n\n\n");
    printw(Player.name);
    strcpy(Str4,"A quitter.");
    printw(" wimped out with %ld points!",calc_points());
    printw("\n\n\n\n\nHit 'c' to continue.");
    refresh();
    while (wgetch(stdscr) != 'c')
        ;
    clear();
    touchwin(stdscr);
    refresh();
    extendlog(Str4,QUIT);
}


void display_bigwin(void)
{
    clear();
    touchwin(stdscr);
    printw("\n\n\n\n");
    printw(Player.name);
    strcpy(Str4,"retired, an Adept of Omega.");
    printw(" retired, an Adept of Omega with %ld points!",FixedPoints);
    printw("\n\n\n\n\nHit 'c' to continue.");
    refresh();
    while (wgetch(stdscr) != 'c')
        ;
    clear();
    touchwin(stdscr);
    refresh();
    extendlog(Str4,BIGWIN);
}


void mnumprint(int n)
{
    char numstr[20];
    sprintf(numstr,"%d",n);
    bufferappend(numstr);
    wprintw(Msgw,"%d",n);
    wrefresh(Msgw);
}

void mlongprint(long n)
{
    char numstr[20];
    sprintf(numstr,"%ld",n);
    bufferappend(numstr);
    wprintw(Msgw,"%ld",n);
    wrefresh(Msgw);
}


void menunumprint(int n)
{
    int x,y;
    getyx(Menuw,y,x);
    if (y >= ScreenLength - 2) {
        wrefresh(Menuw);
        morewait();
        wclear(Menuw);
        touchwin(Menuw);
    }
    wprintw(Menuw,"%d",n);
}

void menulongprint(long n)
{
    int x,y;
    getyx(Menuw,y,x);
    if (y >= ScreenLength - 2) {
        wrefresh(Menuw);
        morewait();
        wclear(Menuw);
        touchwin(Menuw);
    }
    wprintw(Menuw,"%ld",n);
}


void dobackspace(void)
{
    int x,y;

    getyx(Msgw,y,x);
    if (x > 0) {
        waddch(Msgw,' ');
        wmove(Msgw,y,x-1);
        waddch(Msgw,' ');
        wmove(Msgw,y,x-1);
    }
    wrefresh(Msgw);
}


void showflags(void)
{

    phaseprint();
    wclear(Flagw);
    if (Player.food < 0)
        wprintw(Flagw,"Starving\n");
    else if (Player.food <= 3)
        wprintw(Flagw,"Weak\n");
    else if (Player.food <= 10)
        wprintw(Flagw,"Ravenous\n");
    else if (Player.food <= 20)
        wprintw(Flagw,"Hungry\n");
    else if (Player.food <= 30)
        wprintw(Flagw,"A mite peckish\n");
    else if (Player.food <= 36)
        wprintw(Flagw,"Content\n");
    else if (Player.food <= 44)
        wprintw(Flagw,"Satiated\n");
    else wprintw(Flagw,"Bloated\n");


    if (Player.status[POISONED]>0)
        wprintw(Flagw,"Poisoned\n");
    else wprintw(Flagw,"Vigorous\n");

    if (Player.status[DISEASED]>0)
        wprintw(Flagw,"Diseased\n");
    else wprintw(Flagw,"Healthy\n");

    if (State.getMounted()) wprintw(Flagw,"Mounted\n");
    else if (Player.status[LEVITATING]) wprintw(Flagw,"Levitating\n");
    else wprintw(Flagw,"Afoot\n");

    wrefresh(Flagw);
}

void drawomega(void)
{
    int i;
    clear();
    touchwin(stdscr);
    for(i=0; i<7; i++) {
        move(1, 1);
        if (optionp(SHOW_COLOUR))
            wattrset(stdscr, CHARATTR(CLR(LIGHT_BLUE)));
        printw("\n\n\n");
        printw("\n                                    *****");
        printw("\n                               ******   ******");
        printw("\n                             ***             ***");
        printw("\n                           ***                 ***");
        printw("\n                          **                     **");
        printw("\n                         ***                     ***");
        printw("\n                         **                       **");
        printw("\n                         **                       **");
        printw("\n                         ***                     ***");
        printw("\n                          ***                   ***");
        printw("\n                            **                 **");
        printw("\n                       *   ***                ***   *");
        printw("\n                        ****                    ****");
        refresh();
        usleep(200000);
        move(1, 1);
        if (optionp(SHOW_COLOUR))
            wattrset(stdscr, CHARATTR(CLR(CYAN)));
        printw("\n\n\n");
        printw("\n                                    +++++");
        printw("\n                               ++++++   ++++++");
        printw("\n                             +++             +++");
        printw("\n                           +++                 +++");
        printw("\n                          ++                     ++");
        printw("\n                         +++                     +++");
        printw("\n                         ++                       ++");
        printw("\n                         ++                       ++");
        printw("\n                         +++                     +++");
        printw("\n                          +++                   +++");
        printw("\n                            ++                 ++");
        printw("\n                       +   +++                +++   +");
        printw("\n                        ++++                    ++++");
        refresh();
        usleep(200000);
        move(1, 1);
        if (optionp(SHOW_COLOUR))
            wattrset(stdscr, CHARATTR(CLR(BLUE)));
        printw("\n\n\n");
        printw("\n                                    .....");
        printw("\n                               ......   ......");
        printw("\n                             ...             ...");
        printw("\n                           ...                 ...");
        printw("\n                          ..                     ..");
        printw("\n                         ...                     ...");
        printw("\n                         ..                       ..");
        printw("\n                         ..                       ..");
        printw("\n                         ...                     ...");
        printw("\n                          ...                   ...");
        printw("\n                            ..                 ..");
        printw("\n                       .   ...                ...   .");
        printw("\n                        ....                    ....");
        refresh();
        usleep(200000);
    }
    wattrset(stdscr, CHARATTR(CLR(WHITE)));
}

/* y is an absolute coordinate */
/* ScreenOffset is the upper left hand corner of the current screen
   in absolute coordinates */
#if 0
void screencheck(int y)
{
    if (((y-ScreenOffset) < (ScreenLength/8)) ||
            ((y-ScreenOffset) > (7*ScreenLength/8))) {
        ScreenOffset = y - (ScreenLength/2);
        show_screen();
        if (Current_Environment != E_COUNTRYSIDE)
            drawmonsters(true);
        if (!offscreen(Player.x,Player.y))
            drawplayer();
    }
}
#else
void screencheck(int x, int y)
{
    int change = 0;
#ifdef CENTER_ON_PLAYER

    change = 1;
    ScreenOffset = y - (ScreenLength/2);
    ScreenXOffset = x - (ScreenWidth/2);


#else
    if (((y-ScreenOffset) < (ScreenLength/8)) ||
            ((y-ScreenOffset) > (7*ScreenLength/8))) {
        change = 1;
        ScreenOffset = y - (ScreenLength/2);
    }

    if (((x-ScreenXOffset) < (ScreenWidth/8)) ||
            ((x-ScreenXOffset) > (7*ScreenWidth/8))) {

#if 0
    	int width;
        if ( Current_Environment == E_COUNTRYSIDE )
            width = COUNTRY_WIDTH;
        else
            width = Level->level_width;

        /*
          PGM: this code prevents scrolling when near the right edge of the level,
          but this unfortunately results in a badly placed view when, for examples,
          restoring a save near the edge of a level. Ideas on fixing this, or moving
          it, appreciated.  PGM July 1999.
        */
        if ( ( (x-ScreenXOffset) + (7*ScreenWidth/8) ) < width +( ScreenWidth/8 ) )
#endif
        {
            change = 1;
            ScreenXOffset = x - (ScreenWidth/2);
            if (ScreenXOffset < 0)
                ScreenXOffset = 0;
        }
    }

#endif
    if ( change == 1 ) {
        show_screen();
        if (Current_Environment != E_COUNTRYSIDE)
            drawmonsters(true);
        if (!offscreen(Player.x,Player.y))
            drawplayer();
    }
}
#endif

void spreadroomlight(int x, int y, int roomno)
{
    if (inbounds(x,y) && !loc_statusp(x,y,LIT) &&
            Level->site[x][y].roomnumber == roomno) {
        lightspot(x,y);
        spreadroomlight(x+1,y,roomno);
        spreadroomlight(x,y+1,roomno);
        spreadroomlight(x-1,y,roomno);
        spreadroomlight(x,y-1,roomno);
    }
}

/* illuminate one spot at x y */
void lightspot(int x, int y)
{
    Symbol c;
    lset(x,y,LIT);
    lset(x,y,SEEN);
    lset(x, y, CHANGED);
    c = getspot(x,y,false);
    Level->site[x][y].showchar = c;
    putspot(x,y,c);
}


void spreadroomdark(int x, int y, int roomno)
{
    if (inbounds(x,y))
        if (loc_statusp(x,y,LIT) && (Level->site[x][y].roomnumber == roomno)) {
            blankoutspot(x,y);
            spreadroomdark(x+1,y,roomno);
            spreadroomdark(x,y+1,roomno);
            spreadroomdark(x-1,y,roomno);
            spreadroomdark(x,y-1,roomno);
        }
}


void display_pack(void)
{
    int i;
    if (Player.packptr < 1) print3("Pack is empty.");
    else {
        menuclear();
        menuprint("Items in Pack:\n");
        for(i=0; i<Player.packptr; i++) {
            sprintf(Str1, "  %c: %s\n", i + 'A', itemid(Player.pack[i]));
            menuprint(Str1);
        }
        showmenu();
    }
}


void display_possessions(void)
{
    int i;
    for(i=0; i<MAXITEMS; i++)
        display_inventory_slot(i,false);
}


void display_inventory_slot(int slotnum, int topline)
{
    WINDOW *W;
    char usechar = ')', idchar = '-';
    if (Player.possessions[slotnum] != NULL)
        if (Player.possessions[slotnum]->used)
            usechar = '>';
    if (topline) W = Msg3w;
    else {
        W = Showline[slotnum];
        hide_line(slotnum);
    }
    idchar = index_to_key(slotnum);
    touchwin(W);
    wclear(W);
    switch(slotnum) {
    case O_UP_IN_AIR:
        wprintw(W,"-- Object 'up in air':",usechar);
        break;
    case O_READY_HAND:
        wprintw(W,"-- %c%c ready hand: ",idchar,usechar);
        break;
    case O_WEAPON_HAND:
        wprintw(W,"-- %c%c weapon hand: ",idchar,usechar);
        break;
    case O_LEFT_SHOULDER:
        wprintw(W,"-- %c%c left shoulder: ",idchar,usechar);
        break;
    case O_RIGHT_SHOULDER:
        wprintw(W,"-- %c%c right shoulder: ",idchar,usechar);
        break;
    case O_BELT1:
        wprintw(W,"-- %c%c belt: ",idchar,usechar);
        break;
    case O_BELT2:
        wprintw(W,"-- %c%c belt: ",idchar,usechar);
        break;
    case O_BELT3:
        wprintw(W,"-- %c%c belt: ",idchar,usechar);
        break;
    case O_SHIELD:
        wprintw(W,"-- %c%c shield: ",idchar,usechar);
        break;
    case O_ARMOR:
        wprintw(W,"-- %c%c armor: ",idchar,usechar);
        break;
    case O_BOOTS:
        wprintw(W,"-- %c%c boots: ",idchar,usechar);
        break;
    case O_CLOAK:
        wprintw(W,"-- %c%c cloak: ",idchar,usechar);
        break;
    case O_RING1:
        wprintw(W,"-- %c%c finger: ",idchar,usechar);
        break;
    case O_RING2:
        wprintw(W,"-- %c%c finger: ",idchar,usechar);
        break;
    case O_RING3:
        wprintw(W,"-- %c%c finger: ",idchar,usechar);
        break;
    case O_RING4:
        wprintw(W,"-- %c%c finger: ",idchar,usechar);
        break;
    }
    if (Player.possessions[slotnum] != NULL)
        wprintw(W,itemid(Player.possessions[slotnum]));
    wrefresh(W);
}

int move_slot(int oldslot, int newslot, int maxslot)
{
    if ((newslot >= 0) && (newslot < maxslot)) {
        wmove(Showline[oldslot],0,0);
        waddstr(Showline[oldslot],"--");
        wrefresh(Showline[oldslot]);
        wmove(Showline[newslot],0,0);
        wstandout(Showline[newslot]);
        waddstr(Showline[newslot],">>");
        wstandend(Showline[newslot]);
        wrefresh(Showline[newslot]);
        return(newslot);
    }
    else return(oldslot);
}

void colour_on(void)
{
}

void colour_off(void)
{
    wattrset(Levelw, CHARATTR(CLR(WHITE)));
}

void display_option_slot(int slot)
{
    hide_line(slot);
    wclear(Showline[slot]);
    switch(slot) {
    case 1:
        wprintw(Showline[slot],"-- Option BELLICOSE [TF]: ");
        wprintw(Showline[slot], optionp(BELLICOSE) ? "(now T) " : "(now F) ");
        break;
    case 2:
        wprintw(Showline[slot],"-- Option JUMPMOVE [TF]: ");
        wprintw(Showline[slot], optionp(JUMPMOVE) ? "(now T) " : "(now F) ");
        break;
    case 3:
        wprintw(Showline[slot],"-- Option RUNSTOP [TF]: ");
        wprintw(Showline[slot], optionp(RUNSTOP) ? "(now T) " : "(now F) ");
        break;
    case 4:
        wprintw(Showline[slot],"-- Option PICKUP [TF]: ");
        wprintw(Showline[slot], optionp(PICKUP) ? "(now T) " : "(now F) ");
        break;
    case 5:
        wprintw(Showline[slot],"-- Option CONFIRM [TF]: ");
        wprintw(Showline[slot], optionp(CONFIRM) ? "(now T) " : "(now F) ");
        break;
    case 6:
        wprintw(Showline[slot],"-- Option TOPINV [TF]: ");
        wprintw(Showline[slot], optionp(TOPINV) ? "(now T) " : "(now F) ");
        break;
    case 7:
        wprintw(Showline[slot],"-- Option PACKADD [TF]: ");
        wprintw(Showline[slot], optionp(PACKADD) ? "(now T) " : "(now F) ");
        break;
    case 8:
        wprintw(Showline[slot],"-- Option COLOUR [TF]: ");
        wprintw(Showline[slot], optionp(SHOW_COLOUR) ? "(now T) " : "(now F) ");
        break;
    case VERBOSITY_LEVEL:
        wprintw(Showline[slot],
                "-- Option VERBOSITY [(T)erse,(M)edium,(V)erbose]: (now ");
        if (Verbosity == VERBOSE) wprintw(Showline[slot],"Verbose)");
        else if (Verbosity == MEDIUM) wprintw(Showline[slot],"Medium)");
        else wprintw(Showline[slot],"Terse)");
        break;
    case SEARCH_DURATION:
        wprintw(Showline[slot],"-- Option SEARCHNUM [0>x>10]: (now %d)",Searchnum);
        break;
    }
    wrefresh(Showline[slot]);
}

void display_stat_slot(int slot)
{
    hide_line(slot);
    wclear(Showline[slot]);
    switch(slot) {
    case 1:
        wprintw(Showline[slot],"-- Strength ... [%d]: ", Player.str);
        break;
    case 2:
        wprintw(Showline[slot],"-- Constitution [%d]: ", Player.con);
        break;
    case 3:
        wprintw(Showline[slot],"-- Dexterity .. [%d]: ", Player.dex);
        break;
    case 4:
        wprintw(Showline[slot],"-- Agility .... [%d]: ", Player.agi);
        break;
    case 5:
        wprintw(Showline[slot],"-- Intelligence [%d]: ", Player.iq);
        break;
    case 6:
        wprintw(Showline[slot],"-- Power ...... [%d]: ", Player.pow);
        break;
    case 7:
        wprintw(Showline[slot],"-- HP ......... [%d]: ", Player.hp);
        break;
    case 8:
        wprintw(Showline[slot],"-- Max HP ..... [%d]: ", Player.maxhp);
        break;
    case 9:
        wprintw(Showline[slot],"-- Mana ....... [%d]: ", Player.mana);
        break;
    case 10:
        wprintw(Showline[slot],"-- Max Mana ... [%d]: ", Player.maxmana);
        break;
    case 11:
        wprintw(Showline[slot],"-- Cash ....... [%d]: ", Player.cash);
        break;
    }
    wrefresh(Showline[slot]);
}

void display_options(void)
{
    int i;
    menuclear();
    hide_line(0);
    for(i=1; i<=NUMOPTIONS; i++)
        display_option_slot(i);
}

void display_stats(void)
{
    int i;
    menuclear();
    hide_line(0);
    for(i=1; i<=NUMSTATS; i++)
        display_stat_slot(i);
}

/* nya ha ha ha ha haaaa.... */
void deathprint(void)
{
    mgetc();
    waddch(Msgw,'D');
    wrefresh(Msgw);
    mgetc();
    waddch(Msgw,'e');
    wrefresh(Msgw);
    mgetc();
    waddch(Msgw,'a');
    wrefresh(Msgw);
    mgetc();
    waddch(Msgw,'t');
    wrefresh(Msgw);
    mgetc();
    waddch(Msgw,'h');
    wrefresh(Msgw);
    mgetc();
}

void clear_if_necessary(void)
{
    int x,y;
    getyx(Msg1w,y,x);

    if (x != 0) {
        wclear(Msg1w);
        wrefresh(Msg1w);
    }

    getyx(Msg2w,y,x);

    if (x != 0) {
        wclear(Msg2w);
        wrefresh(Msg2w);
    }

    getyx(Msg3w,y,x);

    if (x != 0) {
        erasemsg3();
        wrefresh(Msg3w);
    }
}

int bufferpos = 0;

void buffercycle(const char *s)
{
    strcpy(Stringbuffer[bufferpos++],s);
    if (bufferpos >= STRING_BUFFER_SIZE)
        bufferpos = 0;
}

int bufferappend(const char *s)
{
    int pos = bufferpos - 1;

    if (pos < 0)
        pos = STRING_BUFFER_SIZE - 1;
    if (strlen(Stringbuffer[pos]) + strlen(s) < 80 - 1) {
        strcat(Stringbuffer[pos],s);
        return 1;
    }
    else
        return 0;
}


void bufferprint(void)
{
    int i = bufferpos - 1, c, finished = 0;
    clearmsg();
#if !defined(WIN32)
    wprintw(Msg1w,"^p for previous message, ^n for next, anything else to quit.");
#else
    wprintw(Msg1w,"^o for last message, ^n for next, anything else to quit.");
#endif
    wrefresh(Msg1w);
    do {
        if (i >= STRING_BUFFER_SIZE) i = 0;
        if (i < 0) i = STRING_BUFFER_SIZE - 1;
        wclear(Msg2w);
        wprintw(Msg2w,Stringbuffer[i]);
        wrefresh(Msg2w);
        c = mgetc();
#if !defined(WIN32)
        if (c == 16)	/* ^p */
#else
        if (c == 15)	/* ^o */
#endif
            i--;
        else if (c == 14)	/* ^n */
            i++;
        else
            finished = 1;
    } while (!finished);
    clearmsg();
    omshowcursor(Player.x,Player.y);
}

void clear_screen(void)
{
    clear();
    touchwin(stdscr);
    refresh();
}
