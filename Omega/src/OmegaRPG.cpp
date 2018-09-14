#include "OmegaRPG.h"

namespace Omega
{
OmegaRPG::OmegaRPG(void)
{
    _continuing = false;
    _scoresOnly = false;
    _showScores = true;
    _showTitle = true;

    SaveFilename("Omega");
}

OmegaRPG::~OmegaRPG(void)
{
}

const char* OmegaRPG::SaveFilename()
{
    return _saveFilename;
}

void OmegaRPG::SaveFilename(const char* filename)
{
    _saveFilename = filename;
    g_saveFileName = _saveFilename; // TODO Refactor out this global
}

bool OmegaRPG::Continuing()
{
    return _continuing;
}

void OmegaRPG::Continuing(bool value)
{
    _continuing = value;
}

bool OmegaRPG::ScoresOnly()
{
    return _scoresOnly;
}

void OmegaRPG::ScoresOnly(bool value)
{
    _scoresOnly = value;
}

bool OmegaRPG::ShowTitle()
{
    return _showTitle;
}

void OmegaRPG::ShowTitle(bool value)
{
    _showTitle = value;
}

bool OmegaRPG::ShowScores()
{
    return _showScores;
}

void OmegaRPG::ShowScores(bool value)
{
    _showScores = value;
}

void OmegaRPG::printUsage() {
#ifdef DEBUG
    printf("Usage: omega [-shqd] [savefile]\n");
#else
    printf("Usage: omega [-shq] [savefile]\n");
#endif
    printf("Options:\n");
    printf("  -s  Display high score list only\n");
    printf("  -h  Display this message\n");
    printf("  -q  Quick launch (skip title and high score list)\n");
#ifdef DEBUG
    printf("  -d  Enable debug mode\n");
#endif
}

void OmegaRPG::startGame() {
    Omegalib = OMEGALIB;

    // If filecheck is 0, some necessary data files are missing
    if (filecheck() == 0) exit(-1);

    // All kinds of initialization
    init_perms();
    initgraf();
    initrand(E_RANDOM, 0);
    initspells();

#ifdef DEBUG
    /* initialize debug log file */
    DG_debug_log = fopen( "omegadbg.log", "a" );
    assert( DG_debug_log ); /* WDT :) */
    setvbuf( DG_debug_log, NULL, _IOLBF, 0);
    fprintf(DG_debug_log, "##############  new game started ##############\n");
#endif

    for (int count = 0; count < STRING_BUFFER_SIZE; count++)
    {
        strcpy(Stringbuffer[count], "<nothing>");
    }

    if (this->ShowTitle()) {
    	omega_title();
    }

    if (this->ShowScores() || this->ScoresOnly()) {
    	showscores();
    }

    if (this->ScoresOnly()) {
        endgraf();
        exit(0);
    }

    /* game restore attempts to restore game if there is an argument */
    if (this->Continuing())
    {
        game_restore(this->SaveFilename());
        mprint("Your adventure continues....");
    }
    else
    {
        /* monsters initialized in game_restore if game is being restored */
        /* items initialized in game_restore if game is being restored */
        inititem(true);
        Date = random_range(360);
        Phase = random_range(24);

        // Initialize bank
        bank_init();

        this->Continuing(initplayer()); /* RM: 04-19-2000 loading patch */

        this->SaveFilename(Player.name);
    }

    if (!this->Continuing())
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
        terrain_check(false);
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
    if (!this->Continuing())
    {
        time_clock(true);
    }

    while (true) {
        if (Current_Environment == E_COUNTRYSIDE)
        {
            p_country_process();
        }
        else
        {
            time_clock(false);
        }
    }
}
}
