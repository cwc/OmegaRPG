#include "OmegaRPG.h"

using namespace Omega;

void signalquit(int ignore)
{
    quit();
}

void signalexit(int ignored)
{
    clearmsg();
    mprint("Yikes!");
    morewait();
    mprint("Sorry, caught a core-dump signal.");
    signalsave(0);
    endgraf();
    printf("Bye!\n");
    exit(0);
}

/* saves game on SIGHUP */
void signalsave(int ignore)
{
    change_to_user_perms();
    save_game("Omega.Sav");
    print1("Signal - Saving file 'Omega.Sav'.");
    morewait();
    endgraf();
    exit(0);
}

void initSignals() {
// Always catch ^c and hang-up signals
#ifdef SIGINT
    signal(SIGINT,signalquit);
#endif

#ifdef SIGHUP
    signal(SIGHUP,signalsave);
#endif

#ifdef SIGBREAK
	signal(SIGBREAK, signalquit);
#endif

#ifdef SIGQUIT
        signal(SIGQUIT,signalexit);
#endif
#ifdef SIGILL
        signal(SIGILL,signalexit);
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

#ifdef DEBUG
        if( DG_debug_flag ) {
#ifdef SIGTRAP
            signal(SIGTRAP,signalexit);
#endif
#ifdef SIGFPE
            signal(SIGFPE,signalexit);
#endif
#ifdef SIGSEGV
            signal(SIGSEGV,signalexit);
#endif
        }
#endif
}

int main(int argc, char *argv[])
{
    initSignals();

    OmegaRPG* game = new OmegaRPG();

#ifndef NOGETOPT
    int i;

    while((i = getopt(argc, argv, "dshq")) != -1)
    {
        switch (i)
        {
#ifdef DEBUG
        case 'd':
            DG_debug_flag++;
            break;
#endif
        case 's':
            game->ScoresOnly(true);
            game->ShowTitle(false);
            break;
        case 'h':
            OmegaRPG::printUsage();
            exit(0);
            break;
        case 'q':
            game->ShowTitle(false);
            game->ShowScores(false);
        	break;
        case '?':
            /* error parsing args... ignore? */
            printf("'%c' is an invalid option, ignoring\n", optopt);
            break;
        }
    }

    if (!(optind >= argc)) {
        /* savefile given */
        game->SaveFilename(argv[optind]);
        game->Continuing(true);
    }

#else
    // Alternate code for people who don't support getopt() - no enhancement
    if (argc == 2) {
        game->SaveFilename(argv[1]);
        game->Continuing(true);
    }
#endif

    game->startGame();
}
