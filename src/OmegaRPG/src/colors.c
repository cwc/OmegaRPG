#include "glob.h"

void clrgen_init (void)
{
    if (!has_colors()) {
      fprintf(stderr,"Omega currently requires ncurses or op-curses color support.\n");
      return;
    } else
    if (25 > COLOR_PAIRS - 1) {
	endwin();
	fprintf (stderr,"Too few available color pairs (found %d, need 25)!\n", COLOR_PAIRS);
	exit (1);
    }

    init_pair (1, COLOR_BLACK, COLOR_BLACK);
    init_pair (2, COLOR_BLACK, COLOR_GREEN);
    init_pair (3, COLOR_BLACK, COLOR_RED);
    init_pair (4, COLOR_BLACK, COLOR_WHITE);
    init_pair (5, COLOR_BLACK, COLOR_YELLOW);
    init_pair (6, COLOR_BLUE, COLOR_BLACK);
    init_pair (7, COLOR_BLUE, COLOR_WHITE);
    init_pair (8, COLOR_CYAN, COLOR_BLACK);
    init_pair (9, COLOR_GREEN, COLOR_BLACK);
    init_pair (10, COLOR_GREEN, COLOR_BLUE);
    init_pair (11, COLOR_GREEN, COLOR_RED);
    init_pair (12, COLOR_GREEN, COLOR_YELLOW);
    init_pair (13, COLOR_MAGENTA, COLOR_BLACK);
    init_pair (14, COLOR_MAGENTA, COLOR_WHITE);
    init_pair (15, COLOR_RED, COLOR_BLACK);
    init_pair (16, COLOR_RED, COLOR_WHITE);
    init_pair (17, COLOR_WHITE, COLOR_BLACK);
    init_pair (18, COLOR_WHITE, COLOR_BLUE);
    init_pair (19, COLOR_WHITE, COLOR_RED);
    init_pair (20, COLOR_WHITE, COLOR_YELLOW);
    init_pair (21, COLOR_YELLOW, COLOR_BLACK);
    init_pair (22, COLOR_YELLOW, COLOR_BLUE);
    init_pair (23, COLOR_YELLOW, COLOR_RED);
    init_pair (24, COLOR_YELLOW, COLOR_WHITE);
    init_pair (25, COLOR_YELLOW, COLOR_YELLOW);
}
