/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* command1.c */

/* This file has the two toplevel command scanners, p_process,
which works everywhere but the countryside, and p_couyntry_process,
which works.... */

#include "glob.h"

/* deal with a new player command in dungeon or city mode*/
void p_process(void)
{
    static int searchval=0;

    if (Player.status[BERSERK])
        if (goberserk()) {
            State.setSkipPlayer( true );
            drawvision(Player.x,Player.y);
        }
    if (!State.getSkipPlayer()) {
        if (searchval > 0) {
            searchval--;
            if (searchval == 0) State.setFastMove(false);
        }
        drawvision(Player.x,Player.y);
        if (State.getFastMove() == false) {
            searchval = 0;
            Cmd = mgetc();
            clear_if_necessary();
        }
        Command_Duration = 0;

        switch (Cmd) {
        case ' ':
        case 13:
            State.setSkipMonsters();
            break; /*no op on space or return*/
        case 6:
            abortshadowform();
            break; /* ^f */
        case 7:
            wizard();
            break; /* ^g */
        case 4:
            player_dump();
            break; /* ^d */
        case 9:
            display_pack();
            morewait();
            xredraw();
            break; /* ^i */
        case 11:
            if (State.getCheater()) frobgamestatus();
            break;
        case 12:
            xredraw();
            State.setSkipMonsters();
            break; /* ^l */
#if !defined(WIN32)
        case 16:
            bufferprint();
            State.setSkipMonsters();
            break; /* ^p */
#else
        case 15:
            bufferprint();
            State.setSkipMonsters();
            break; /* ^o */
#endif
        case 18:
            redraw();
            State.setSkipMonsters();
            break; /* ^r */
        case 23:
            if (State.getCheater()) drawscreen();
            break; /* ^w */
        case 24: /* ^x */
            if (State.getCheater() ||
                    Player.rank[ADEPT])
                wish(1);
            Command_Duration = 5;
            break;
        case 'a':
            zapwand();
            Command_Duration = Player.speed*8/5;
            break;
        case 'c':
            closedoor();
            Command_Duration = Player.speed*2/5;
            break;
        case 'd':
            drop();
            Command_Duration = Player.speed*5/5;
            break;
        case 'e':
            eat();
            Command_Duration = 30;
            break;
        case 'f':
            fire();
            Command_Duration = Player.speed*5/5;
            break;
        case 'g':
            pickup();
            Command_Duration = Player.speed*10/5;
            break;
        case 'i':
            do_inventory_control();
            break;
        case 'm':
            magic();
            Command_Duration = 12;
            break;
        case 'o':
            opendoor();
            Command_Duration = Player.speed*5/5;
            break;
        case 'p':
            pickpocket();
            Command_Duration = Player.speed*20/5;
            break;
        case 'q':
            quaff();
            Command_Duration = 10;
            break;
        case 'r':
            peruse();
            Command_Duration = 20;
            break;
        case 's':
            search(&searchval);
            Command_Duration = 20;
            break;
        case 't':
            talk();
            Command_Duration = 10;
            break;
        case 'v':
            vault();
            Command_Duration = Player.speed*10/5;
            break;
        case 'x':
            examine();
            Command_Duration = 1;
            break;
        case 'z':
            bash_location();
            Command_Duration = Player.speed*10/5;
            break;
        case 'A':
            activate();
            Command_Duration = 10;
            break;
        case 'C':
            callitem();
            break;
        case 'D':
            disarm();
            Command_Duration = 30;
            break;
        case 'E':
            dismount_steed();
            Command_Duration = Player.speed*10/5;
            break;
        case 'F':
            tacoptions();
            break;
        case 'G':
            give();
            Command_Duration = 10;
            break;
        case 'I':
            if (! optionp(TOPINV)) top_inventory_control();
            else {
                display_possessions();
                inventory_control();
            }
            break;
        case 'M':
            city_move();
            Command_Duration = 10;
            break;
        case 'O':
            setoptions();
#if defined(WIN32)
            show_screen();
            xredraw();
#endif
            break;
        case 'P':
            show_license();
            break; /* actually show_license is in file.c */
        case 'Q':
            quit();
            break;
        case 'R':
            rename_player();
            break;
        case 'S':
            save(false);
            break;
        case 'T':
            tunnel();
            Command_Duration =  Player.speed*30/5;
            break;
        case 'V':
            version();
            break;
        case 'Z':
            bash_item();
            Command_Duration = Player.speed*10/5;
            break;
        case '.':
            rest();
            Command_Duration = 10;
            break;
        case ',':
            nap();
            break;
        case '>':
            downstairs();
            break;
        case '<':
            upstairs();
            break;
        case '@':
            p_movefunction(Level->site[Player.x][Player.y].p_locf);
            Command_Duration = 5;
            break;
        case '#':
            if (State.getCheater()) editstats();
            break; /* RAC - char editor */
        case '/':
            charid();
            State.setSkipMonsters();
            break;
        case '?':
            help();
            State.setSkipMonsters();
            break;
        case '4':
        case 'h':
#ifdef KEY_LEFT
        case KEY_LEFT:
#endif
            moveplayer(-1,0);
            Command_Duration = Player.speed*5/5;
            break;
        case '2':
        case 'j':
#ifdef KEY_DOWN
        case KEY_DOWN:
#endif
            moveplayer(0,1);
            Command_Duration = Player.speed*5/5;
            break;
#ifdef KEY_UP
        case KEY_UP:
#endif
        case '8':
        case 'k':
            moveplayer(0,-1);
            Command_Duration = Player.speed*5/5;
            break;
#ifdef KEY_RIGHT
        case KEY_RIGHT:
#endif
        case '6':
        case 'l':
            moveplayer(1,0);
            Command_Duration = Player.speed*5/5;
            break;
        case '1':
        case 'b':
            moveplayer(-1,1);
            Command_Duration = Player.speed*5/5;
            break;
        case '3':
        case 'n':
            moveplayer(1,1);
            Command_Duration = Player.speed*5/5;
            break;
        case '7':
        case 'y':
            moveplayer(-1,-1);
            Command_Duration = Player.speed*5/5;
            break;
        case '9':
        case 'u':
            moveplayer(1,-1);
            Command_Duration = Player.speed*5/5;
            break;
        case '5':
            State.setSkipMonsters(); /* don't do anything; a dummy turn */
            Cmd = mgetc();
            while ((Cmd != ESCAPE) &&
                    ((Cmd < '1') || (Cmd > '9') || (Cmd=='5'))) {
                print3("Run in keypad direction [ESCAPE to abort]: ");
                Cmd = mgetc();
            }
            if (Cmd != ESCAPE) 
                State.setFastMove();
            else
                clearmsg3();
            break;
        case 'H':
#ifdef KEY_SLEFT
        case KEY_SLEFT:
#endif
            State.setFastMove();
            Cmd = 'h';
            moveplayer(-1,0);
            Command_Duration = Player.speed*4/5;
            break;
        case 'J':
#ifdef KEY_SDOWN
        case KEY_SDOWN:
#endif
            State.setFastMove();
            Cmd = 'j';
            moveplayer(0,1);
            Command_Duration = Player.speed*4/5;
            break;
        case 'K':
#ifdef KEY_SUP
        case KEY_SUP:
#endif
            State.setFastMove();
            Cmd = 'k';
            moveplayer(0,-1);
            Command_Duration = Player.speed*4/5;
            break;
        case 'L':
#ifdef KEY_SRIGHT
        case KEY_SRIGHT:
#endif
            State.setFastMove();
            Cmd = 'l';
            moveplayer(1,0);
            Command_Duration = Player.speed*4/5;
            break;
        case 'B':
            State.setFastMove();
            Cmd = 'b';
            moveplayer(-1,1);
            Command_Duration = Player.speed*4/5;
            break;
        case 'N':
            State.setFastMove();
            Cmd = 'n';
            moveplayer(1,1);
            Command_Duration = Player.speed*4/5;
            break;
        case 'Y':
            State.setFastMove();
            Cmd = 'y';
            moveplayer(-1,-1);
            Command_Duration = Player.speed*4/5;
            break;
        case 'U':
            State.setFastMove();
            Cmd = 'u';
            moveplayer(1,-1);
            Command_Duration = Player.speed*4/5;
            break;
        default:
            commanderror();
            State.setSkipMonsters();
            break;
        }
    }
    if (Current_Environment != E_COUNTRYSIDE) roomcheck();
    screencheck(Player.x,Player.y);
}

/* deal with a new player command in countryside mode */
void p_country_process(void)
{
    int no_op;

    drawvision(Player.x,Player.y);
    do {
        no_op = false;
        Cmd = mgetc();
        clear_if_necessary();
        switch (Cmd) {
        case ' ':
        case 13:
            no_op = true;
            break;
        case 7:
            wizard();
            break; /* ^g */
        case 12:
            xredraw();
            no_op = true;
            break; /* ^l */
#if !defined(WIN32)
        case 16:
            bufferprint();
            no_op = true;
            break; /* ^p */
#else
        case 15:
            bufferprint();
            no_op = true;
            break; /* ^o */
#endif
        case 18:
            redraw();
            no_op = true;
            break; /* ^r */
        case 23:
            if (State.getCheater()) drawscreen();
            break; /* ^w */
        case 24:
            if (State.getCheater() ||
                    Player.rank[ADEPT]) wish(1);
            break; /* ^x */
        case 'd':
            drop();
            break;
        case 'e':
            eat();
            break;
        case 'i':
            do_inventory_control();
            break;
        case 's':
            countrysearch();
            break;
        case 'x':
            examine();
            break;
        case 'E':
            dismount_steed();
            break;
        case 'H':
            hunt(Country[Player.x][Player.y].current_terrain_type);
            break;
        case 'I':
            if (! optionp(TOPINV)) top_inventory_control();
            else {
                menuclear();
                display_possessions();
                inventory_control();
            }
            break;
        case 'O':
            setoptions();
            break;
        case 'P':
            show_license();
            break; /* actually show_license is in file.c */
        case 'Q':
            quit();
            break;
        case 'R':
            rename_player();
            break;
        case 'S':
            save(false);
            break;
        case 'V':
            version();
            break;
        case '>':
            enter_site(Country[Player.x][Player.y].base_terrain_type);
            break;
        case '#':
            if (State.getCheater()) editstats();
            break; /* RAC - char editor */
        case '/':
            charid();
            no_op = true;
            break;
        case '?':
            help();
            no_op = true;
            break;
        case '4':
        case 'h':
            movepincountry(-1,0);
            break;
        case '2':
        case 'j':
            movepincountry(0,1);
            break;
        case '8':
        case 'k':
            movepincountry(0,-1);
            break;
        case '6':
        case 'l':
            movepincountry(1,0);
            break;
        case '1':
        case 'b':
            movepincountry(-1,1);
            break;
        case '3':
        case 'n':
            movepincountry(1,1);
            break;
        case '7':
        case 'y':
            movepincountry(-1,-1);
            break;
        case '9':
        case 'u':
            movepincountry(1,-1);
            break;
        default:
            commanderror();
            no_op = true;
            break;
        }
    } while (no_op);
    screencheck(Player.x,Player.y);
}


