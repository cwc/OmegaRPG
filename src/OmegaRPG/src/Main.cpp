extern "C" {
#include "glob.h"
}

#include "OmegaRPG.h"

using namespace Omega;

int main(int argc, char *argv[])
{
    OmegaRPG* game;

#ifndef NOGETOPT
    int i;

    while(( i= getopt( argc, argv, "dsh")) != -1)
    {
        switch (i)
        {
        case 'd':
#ifdef DEBUG
            DG_debug_flag++;
#endif
            break;
        case 's':
            scores_only = 1;
            break;
        case 'h':
#ifdef DEBUG
            printf("Usage: omega [-shd] [savefile]\n");
#else
            printf("Usage: omega [-sh] [savefile]\n");
#endif
            printf("Options:\n");
            printf("  -s  Display high score list\n");
            printf("  -h  Display this message\n");
#ifdef DEBUG
            printf("  -d  Enable debug mode\n");
#endif
            exit(0);
            break;
        case '?':
            /* error parsing args... ignore? */
            printf("'%c' is an invalid option, ignoring\n", optopt );
            break;
        }
    }

    if (optind >= argc ) {
        /* no save file given */
#if defined( BSD ) || defined( SYSV )
        sprintf( SaveFileName, "Omega%d", getuid() );
#else
        strcpy( SaveFileName,"Omega");
#endif
    } else {
        /* savefile given */
        continuing = 1;
        strcpy(SaveFileName,argv[optind]);
    }

#else
    // Alternate code for people who don't support getopt() - no enhancement
    if (argc == 2) {
        game = new OmegaRPG(argv[1]);
    } else {
        game = new OmegaRPG();
    }
#endif

// Always catch ^c and hang-up signals
#ifdef SIGINT
    signal(SIGINT,signalquit);
#endif

#ifdef SIGHUP
    signal(SIGHUP,signalsave);
#endif

#if !defined(MSDOS) && !defined(WIN32)
    if (CATCH_SIGNALS) {
        signal(SIGQUIT,signalexit);
        signal(SIGILL,signalexit);
#ifdef DEBUG
        if( DG_debug_flag ) {
#endif
            signal(SIGTRAP,signalexit);
            signal(SIGFPE,signalexit);
            signal(SIGSEGV,signalexit);
#ifdef DEBUG
        }
#endif
#ifdef SIGIOT
        signal(SIGIOT,signalexit);
#endif
#ifdef SIGABRT
        signal(SIGABRT,signalexit);
#endif
#ifdef SIGEMT
        signal(SIGEMT,signalexit);
#endif
#ifdef SIGBUS
        signal(SIGBUS,signalexit);
#endif
#ifdef SIGSYS
        signal(SIGSYS,signalexit);
#endif
    }
#endif

    Omegalib = OMEGALIB;

#ifndef FIXED_OMEGALIB
    Omegalib = getenv("OMEGALIB");
#endif

    // If filecheck is 0, some necessary data files are missing
    if (filecheck() == 0) exit(-1);

    // All kinds of initialization
    init_perms();
    initgraf();
#if !defined(MSDOS) && !defined(WIN32)
    initdirs();
#endif
    initrand(E_RANDOM, 0);
    initspells();

#ifdef DEBUG
    /* initialize debug log file */
    DG_debug_log = fopen( "/tmp/omega_dbg_log", "a" );
    assert( DG_debug_log ); /* WDT :) */
    setvbuf( DG_debug_log, NULL, _IOLBF, 0);
    fprintf(DG_debug_log, "##############  new game started ##############\n");
#endif

    for (int count = 0; count < STRING_BUFFER_SIZE; count++)
    {
        strcpy(Stringbuffer[count], "<nothing>");
    }

#ifdef SAVE_LEVELS
    msdos_init();
#endif

    omega_title();
    showscores();

    if (game->ScoresOnly()) {
        endgraf();
        exit(0);
    }

    /* game restore attempts to restore game if there is an argument */
    if (game->Continuing())
    {
        game_restore(argv[1]);
        mprint("Your adventure continues....");
    }
    else
    {
        /* monsters initialized in game_restore if game is being restored */
        /* items initialized in game_restore if game is being restored */
        inititem(TRUE);
        Date = random_range(360);
        Phase = random_range(24);
#ifdef NEW_BANK
        bank_init();
#else
        strcpy(Password, "");
#endif
        game->Continuing(initplayer()); /* RM: 04-19-2000 loading patch */
    }

    if (!game->Continuing())
    {
        init_world();  /* RM: 04-19-2000 loading patch */
        mprint("'?' for help or commandlist, 'Q' to quit.");
    }

    timeprint();
    calc_melee();

    if (Current_Environment != E_COUNTRYSIDE)
    {
        showroom(Level->site[Player.x][Player.y].roomnumber);
    }
    else
    {
        terrain_check(FALSE);
    }

    if (optionp(SHOW_COLOUR))
    {
        colour_on();
    }
    else
    {
        colour_off();
    }

    screencheck(Player.x,Player.y);

    // Game cycle
    if (!game->Continuing())
    {
        time_clock(TRUE);
    }

    while (TRUE) {
        if (Current_Environment == E_COUNTRYSIDE)
        {
            p_country_process();
        }
        else
        {
            time_clock(FALSE);
        }
    }
}
