#ifdef WIN32
#include "../../lib/pdcurses.h"
#else
#include <curses.h>
#endif

// Ensure that curses provides the following key codes
#ifndef CTL_UP
#define KEY_CUP 566
#endif /* CTL_UP */
#ifndef CTL_DOWN
#define CTL_DOWN 525
#endif /* CTL_DOWN */
#ifndef CTL_LEFT
#define CTL_LEFT 545
#endif /* CTL_LEFT */
#ifndef CTL_RIGHT
#define CTL_RIGHT 560
#endif /* CTL_RIGHT */

#ifndef KEY_SUP
#define KEY_SUP 337
#endif /* KEY_SUP */
#ifndef KEY_SDOWN
#define KEY_SDOWN 336
#endif /* KEY_SDOWN */
