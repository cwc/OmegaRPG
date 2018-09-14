/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* aux1.c */
/* auxiliary functions for those in com.c, also see aux2.c and aux3.c */

#include "glob.h"

/* check to see if too much tunneling has been done in this level */
void tunnelcheck(void)
{
    if ((Level->depth == 0 && Current_Environment != E_DLAIR) ||
            Current_Environment == E_ASTRAL)
        return;
    Level->tunnelled++;
    if ((Level->tunnelled) > (Level->level_length)/4)
        mprint("Dust and stone fragments fall on you from overhead.");
    if ((Level->tunnelled) >(Level->level_length)/2)
        mprint("You hear groaning and creaking noises.");
    if ((Level->tunnelled) > (3*(Level->level_length))/4)
        mprint("The floor trembles and you hear a loud grinding screech.");
    if ((Level->tunnelled) > Level->level_length) {
        mprint("With a scream of tortured stone, the entire dungeon caves in!!!");
        gain_experience(5000);
        if (Player.status[SHADOWFORM]) {
            change_environment(E_COUNTRYSIDE);
            switch (Country[Player.x][Player.y].base_terrain_type)
            {
            case CASTLE:
            case STARPEAK:
            case CAVES:
            case VOLCANO:
                Country[Player.x][Player.y].current_terrain_type = MOUNTAINS;
                break;
            case DRAGONLAIR:
                Country[Player.x][Player.y].current_terrain_type = DESERT;
                break;
            case MAGIC_ISLE:
                Country[Player.x][Player.y].current_terrain_type = CHAOS_SEA;
                break;
            case PALACE:
                Country[Player.x][Player.y].current_terrain_type = JUNGLE;
                break;
            }
            Country[Player.x][Player.y].base_terrain_type =
                Country[Player.x][Player.y].current_terrain_type;
            c_set(Player.x, Player.y, CHANGED);
            print1("In your shadowy state, you float back up to the surface.");
            return;
        }
        mprint("You are flattened into an unpleasant jellylike substance.");
        p_death("dungeon cave-in");
    }
}

/* displays a room's name */
void showroom(int i)
{
    strcpy(Str1,"");
    strcpy(Str2,"");
    switch(Current_Environment) {
    case E_MANSION:
        strcpy(Str2,"A luxurious mansion: ");
        break;
    case E_HOUSE:
        strcpy(Str2,"A house: ");
        break;
    case E_HOVEL:
        strcpy(Str2,"A hovel: ");
        break;
    case E_CITY:
        strcpy(Str2,"The City of Rampart");
        break;
    case E_VILLAGE:
        switch(Villagenum) {
        case 1:
            strcpy(Str2,"The Village of Star View");
            break;
        case 2:
            strcpy(Str2,"The Village of Woodmere");
            break;
        case 3:
            strcpy(Str2,"The Village of Stormwatch");
            break;
        case 4:
            strcpy(Str2,"The Village of Thaumaris");
            break;
        case 5:
            strcpy(Str2,"The Village of Skorch");
            break;
        case 6:
            strcpy(Str2,"The Village of Whorfen");
            break;
        }
        break;
    case E_CAVES:
        strcpy(Str2,"The Goblin Caves: ");
        break;
    case E_CASTLE:
        strcpy(Str2,"The Archmage's Castle: ");
        break;
    case E_ASTRAL:
        strcpy(Str2,"The Astral Plane: ");
        break;
    case E_VOLCANO:
        strcpy(Str2,"The Volcano: ");
        break;
    case E_PALACE:
        strcpy(Str2,"The Palace Dungeons: ");
        break;
    case E_SEWERS:
        strcpy(Str2,"The Sewers: ");
        break;
    case E_TACTICAL_MAP:
        strcpy(Str2,"The Tactical Map ");
        break;
    default:
        strcpy(Str2,"");
        break;
    }
    if (Current_Environment == Current_Dungeon) {
        strcpy(Str1,"Level ");
        if (Level->depth < 10) {
            Str1[6] = Level->depth + '0';
            Str1[7] = 0;
        }
        else {
            Str1[6] = (Level->depth / 10) + '0';
            Str1[7] = (Level->depth % 10) + '0';
            Str1[8] = 0;
        }
        strcat(Str1," (");
        strcat(Str1,roomname(i));
        strcat(Str1,")");
    }
    else if (strlen(Str2) == 0 || Current_Environment == E_MANSION ||
             Current_Environment == E_HOUSE || Current_Environment == E_HOVEL)
        strcpy(Str1,roomname(i));
    strcat(Str2,Str1);
    locprint(Str2);
}


int player_on_sanctuary(void)
{
    if ((Player.x==Player.sx) &&
            (Player.y==Player.sy))
        return(true);
    else {
        if (Player.patron) {
            if ((Level->site[Player.x][Player.y].locchar == ALTAR) &&
                    (Level->site[Player.x][Player.y].aux == Player.patron))
                return(true);
            else return(false);
        }
        else return(false);
    }
}


/* check a move attempt, maybe attack something, return true if ok to move. */
/* x y is the proposed place to move to */
int p_moveable(int x, int y)
{
    State.setSkipMonsters();
    if (! inbounds(x,y)) return (false);
    else if (Player.status[SHADOWFORM]) {
        switch(Level->site[x][y].p_locf) {
        case L_CHAOS:
        case L_ABYSS:
        case L_VOID:
            return cinema_confirm("That looks dangerous.") == 'y';
        default:
            State.setSkipMonsters(false);
            return(true);
        }
    }
    else if (loc_statusp(x,y,SECRET)) {
        if (State.getFastMove() == false) print3("Ouch!");
        return(false);
    }
    else if (Level->site[x][y].creature != NULL) {
        if (State.getFastMove() == false) {
            Level->site[x][y].creature->fight_monster();
            State.setSkipMonsters(false);
            return(false);
        }
        else return(false);
    }
    else if (Level->site[x][y].locchar == CLOSED_DOOR) // Zop: move into door -> opens doors
    {
        if (Level->site[x][y].aux == LOCKED) {
            print3("That door seems to be locked.");
            return(false);
        }
        else {
            Level->site[x][y].locchar = OPEN_DOOR;
            lset(x, y, CHANGED);
            State.setSkipMonsters(false);
            return(false);
        }
    }
    else if ((Level->site[x][y].locchar == WALL) ||
             (Level->site[x][y].locchar == STATUE) ||
             (Level->site[x][y].locchar == PORTCULLIS) ||
             (State.getFastMove() &&
              ((Level->site[x][y].locchar == HEDGE) ||
               (Level->site[x][y].locchar == LAVA) ||
               (Level->site[x][y].locchar == ABYSS) ||
               (Level->site[x][y].locchar == VOID_CHAR) ||
               (Level->site[x][y].locchar == FIRE) ||
               (Level->site[x][y].locchar == WHIRLWIND) ||
               (Level->site[x][y].locchar == WATER) ||
               (Level->site[x][y].locchar == LIFT) ||
               (Level->site[x][y].locchar == TRAP)))) {
        if (State.getFastMove() == false) print3("Ouch!");
        return(false);
    }
    else if (optionp(CONFIRM)) {
        if ((Level->site[x][y].locchar == HEDGE) ||
                (Level->site[x][y].locchar == LAVA) ||
                (Level->site[x][y].locchar == FIRE) ||
                (Level->site[x][y].locchar == WHIRLWIND) ||
                (Level->site[x][y].locchar == ABYSS) ||
                (Level->site[x][y].locchar == VOID_CHAR) ||
                (Level->site[x][y].locchar == WATER) ||
                (Level->site[x][y].locchar == RUBBLE) ||
                (Level->site[x][y].locchar == LIFT) ||
                (Level->site[x][y].locchar == TRAP)) {
            /* horses WILL go into water... */
            if (State.getMounted()) {
                if (Level->site[x][y].locchar != WATER ||
                        Level->site[x][y].p_locf != L_WATER) {
                    print1("You can't convince your steed to continue.");
                    State.setSkipMonsters();
                    return(false);
                }
                else return(true);
            }
            else if (cinema_confirm("Look where you're about to step!") == 'y') State.setSkipMonsters(false);
            else State.setSkipMonsters();
            return(State.getSkipMonsters() == false);
        }
        else {
            State.setSkipMonsters(false);
            return(true);
        }
    }
    else {
        State.setSkipMonsters(false);
        return(true);
    }
}



/* check a move attempt in the countryside */
int p_country_moveable(int x, int y)
{
    if (! inbounds(x,y)) return (false);
    else if (optionp(CONFIRM)) {
        if ((Country[x][y].current_terrain_type == CHAOS_SEA) ||
                (Country[x][y].current_terrain_type == MOUNTAINS))
            return(cinema_confirm("That's dangerous terrain, and slow going.")=='y');
        else return(true);
    }
    else return(true);
}





/* search once particular spot */
void searchat(int x, int y)
{
    int i;
    if (inbounds(x,y) && (random_range(3) || Player.status[ALERT])) {
        if (loc_statusp(x,y,SECRET)) {
            lreset(x,y,SECRET);
            lset(x, y, CHANGED);
            if ((Level->site[x][y].locchar==OPEN_DOOR) ||
                    (Level->site[x][y].locchar==CLOSED_DOOR)) {
                mprint("You find a secret door!");
                for(i=0; i<=8; i++) { /* FIXED! 12/25/98 */
                    lset(x+Dirs[0][i],y+Dirs[1][i],STOPS);
                    lset(x+Dirs[0][i], y+Dirs[1][i], CHANGED);
                }
            }
            else mprint("You find a secret passage!");
            drawvision(Player.x,Player.y);
        }
        if ((Level->site[x][y].p_locf >= TRAP_BASE) &&
                (Level->site[x][y].locchar != TRAP) &&
                (Level->site[x][y].p_locf <= TRAP_BASE+NUMTRAPS)) {
            Level->site[x][y].locchar = TRAP;
            lset(x, y, CHANGED);
            mprint("You find a trap!");
            drawvision(Player.x,Player.y);
            State.setFastMove(false);
        }
    }
}



/* This is to be called whenever anything might change player performance in
melee, such as changing weapon, statistics, etc. */
void calc_melee(void)
{
    calc_weight();

    Player.maxweight = (Player.str * Player.agi * 10);
    Player.absorption = Player.status[PROTECTION];
    Player.defense = 2 * statmod(Player.agi)+(Player.level/2);
    Player.hit = Player.level + statmod(Player.dex)+1;
    Player.dmg = statmod(Player.str)+3;
    Player.speed = 5 - min(4,(statmod(Player.agi)/2));
    if (Player.status[HASTED] > 0) Player.speed = Player.speed / 2;
    if (Player.status[SLOWED] > 0) Player.speed = Player.speed * 2;
    if (Player.itemweight > 0)
        switch(Player.maxweight / Player.itemweight) {
        case 0:
            Player.speed+=6;
            break;
        case 1:
            Player.speed+=3;
            break;
        case 2:
            Player.speed+=2;
            break;
        case 3:
            Player.speed+=1;
            break;
        }

    if (Player.status[ACCURATE]) Player.hit+=20;
    if (Player.status[HERO]) Player.hit+=Player.dex;
    if (Player.status[HERO]) Player.dmg+=Player.str;
    if (Player.status[HERO]) Player.defense+=Player.agi;
    if (Player.status[HERO]) Player.speed=Player.speed / 2;

    Player.speed = max(1,min(25,Player.speed));

    if (State.getMounted()) {
        Player.speed = 3;
        Player.hit += 10;
        Player.dmg += 10;
    }
    else if (Player.rank[MONKS] > 0)
    {
        /* monks are faster when not in armor or on horseback */
        if (Player.possessions[O_ARMOR] == NULL) {
            Player.speed += (min(0,(Player.rank[MONKS] -1)));
        }
    }

    /* weapon */
    /* have to check for used since it could be a 2h weapon just carried
    in one hand */
    if (Player.possessions[O_WEAPON_HAND] != NULL)
        if (Player.possessions[O_WEAPON_HAND]->used &&
                ((Player.possessions[O_WEAPON_HAND]->objchar==WEAPON)||
                 (Player.possessions[O_WEAPON_HAND]->objchar==MISSILEWEAPON))) {
            Player.hit +=
                Player.possessions[O_WEAPON_HAND]->hit +
                Player.possessions[O_WEAPON_HAND]->plus;
            Player.dmg +=
                Player.possessions[O_WEAPON_HAND]->dmg +
                Player.possessions[O_WEAPON_HAND]->plus;
        }

    if (Player.rank[MONKS] > 0)
    {
        /* monks */
        /* aren't monks just obscene? PGM */
        if (Player.possessions[O_WEAPON_HAND] == NULL) /*barehanded*/
        {
            /* all monks get a bonus in unarmed combat */
            Player.hit += ( Player.rank[MONKS] * Player.level );
            Player.dmg += ( Player.rank[MONKS] * Player.level );
            Player.defense += ( Player.rank[MONKS] * Player.level );

            if (Player.rank[MONKS] == MONK_GRANDMASTER)
            {
                /* Grandmaster does 3x damage in unarmed combat. */
                Player.dmg *= 3;
            }
        }
    }

    /* shield or defensive weapon */
    if (Player.possessions[O_SHIELD] != NULL) {
        Player.defense +=
            Player.possessions[O_SHIELD]->aux +
            Player.possessions[O_SHIELD]->plus;
    }

    /* armor */
    if (Player.possessions[O_ARMOR] != NULL) {
        Player.absorption += Player.possessions[O_ARMOR]->dmg;
        Player.defense +=
            Player.possessions[O_ARMOR]->plus -
            Player.possessions[O_ARMOR]->aux;
    }

    if (strlen(Player.combatManeuvers) > 2*maneuvers())
        default_maneuvers();
    comwinprint();
    showflags();
    dataprint();
}

/* Attempt to break an object o */
int damage_item(Object* o)
{
    /* special case -- break star gem */
    if (o->id == OB_STARGEM) {
        print1("The Star Gem shatters into a million glistening shards....");
        if (Current_Environment == E_STARPEAK) {
            if (!State.getKilledLawbringer())
                print2("You hear an agonizing scream of anguish and despair.");
            morewait();
            print1("A raging torrent of energy escapes in an explosion of magic!");
            print2("The energy flows to the apex of Star Peak where there is");
            morewait();
            clearmsg();
            print1("an enormous explosion!");
            morewait();
            annihilate(1);
            print3("You seem to gain strength in the chaotic glare of magic!");
            Player.str = max(Player.str, Player.maxstr + 5); /* FIXED! 12/25/98 */
            Player.pow = max(Player.pow, Player.maxpow + 5); /* ditto */
            Player.alignment -= 200;
            dispose_lost_objects(1,o);
        }
        else {
            morewait();
            print1("The shards coalesce back together again, and vanish");
            print2("with a muted giggle.");
            dispose_lost_objects(1,o);
            Objects[o->id].uniqueness = UNIQUE_UNMADE; /* FIXED! 12/30/98 */
        }
        return 1;
    }
    else {
        if (o->fragility < random_range(30)) {
            if (o->objchar == STICK && o->charge > 0) {
                strcpy(Str1,"Your ");
                strcat(Str1,(o->blessing >= 0 ? o->truename : o->cursestr));
                strcat(Str1," explodes!");
                print1(Str1);
                morewait();
                nprint1(" Ka-Blamm!!!");
                /* general case. Some sticks will eventually do special things */
                morewait();
                manastorm(Player.x, Player.y, o->charge*o->level*10);
                dispose_lost_objects(1,o);
                return 1;
            }
            else if ((o->blessing > 0) && (o->level > random_range(10))) {
                strcpy(Str1,"Your ");
                strcat(Str1,itemid(o));
                strcat(Str1," glows strongly.");
                print1(Str1);
                return 0;
            }
            else if ((o->blessing < -1) && (o->level > random_range(10))) {
                strcpy(Str1,"You hear an evil giggle from your ");
                strcat(Str1,itemid(o));
                print1(Str1);
                return 0;
            }
            else if (o->plus > 0) {
                strcpy(Str1,"Your ");
                strcat(Str1,itemid(o));
                strcat(Str1," glows and then fades.");
                print1(Str1);
                o->plus--;
                return 0;
            }
            else {
                if (o->blessing > 0) print1("You hear a faint despairing cry!");
                else if (o->blessing < 0) print1("You hear an agonized scream!");
                strcpy(Str1,"Your ");
                strcat(Str1,itemid(o));
                strcat(Str1," shatters in a thousand lost fragments!");
                print2(Str1);
                morewait();
                dispose_lost_objects(1,o);
                return 1;
            }
        }
        return 0;
    }
}

/* do dmg points of damage of type dtype, from source fromstring */
void p_damage(int dmg, int dtype, const char *fromstring)
{
    if (State.getFastMove()) {
        drawvision(Player.x,Player.y);
        State.setFastMove(false);
    }
    if (! p_immune(dtype)) {
        if (dtype == NORMAL_DAMAGE) Player.hp -= max(1,(dmg-Player.absorption));
        else Player.hp -= dmg;
        if (Player.hp < 1) p_death(fromstring);
    }
    else mprint("You resist the effects!");
    dataprint();
}

/* game over, you lose! */
void p_death(const char *fromstring)
{
    Player.hp = -1;
    print3("You died!");
    morewait();
    display_death(fromstring);
    player_dump();
    endgraf();
    exit(0);
}

/* move the cursor around, like for firing a wand, sets x and y to target */
void setspot(int *x, int *y)
{
    char c = ' ';
    mprint("Targeting.... ? for help");
    omshowcursor(*x,*y);
    while ((c != '.') && (c != ESCAPE)) {
        c = lgetc();
        switch(c) {
        case 'h':
        case '4':
            movecursor(x,y,-1,0);
            break;
        case 'j':
        case '2':
            movecursor(x,y,0,1);
            break;
        case 'k':
        case '8':
            movecursor(x,y,0,-1);
            break;
        case 'l':
        case '6':
            movecursor(x,y,1,0);
            break;
        case 'b':
        case '1':
            movecursor(x,y,-1,1);
            break;
        case 'n':
        case '3':
            movecursor(x,y,1,1);
            break;
        case 'y':
        case '7':
            movecursor(x,y,-1,-1);
            break;
        case 'u':
        case '9':
            movecursor(x,y,1,-1);
            break;
        case '?':
            clearmsg();
            mprint("Use vi keys or numeric keypad to move cursor to target.");
            mprint("Hit the '.' key when done, or ESCAPE to abort.");
            break;
        }
    }
    if (c==ESCAPE) {
        *x = *y = ABORT;
        clearmsg();
    }
    screencheck(Player.x,Player.y);
}


/* get a direction: return index into Dirs array corresponding to direction */
int getdir(void)
{
    while (1) {
        mprint("Select direction [hjklyubn, ESCAPE to quit]: ");
        switch (mgetc()) {
        case '4':
        case 'h':
        case 'H':
            return(5);
        case '2':
        case 'j':
        case 'J':
            return(6);
        case '8':
        case 'k':
        case 'K':
            return(7);
        case '6':
        case 'l':
        case 'L':
            return(4);
        case '7':
        case 'y':
        case 'Y':
            return(3);
        case '9':
        case 'u':
        case 'U':
            return(1);
        case '1':
        case 'b':
        case 'B':
            return(2);
        case '3':
        case 'n':
        case 'N':
            return(0);
        case ESCAPE:
            clearmsg();
            return(ABORT);
        default:
            print3("That's not a direction! ");
        }
    }
}

/* for the examine function */
void describe_player(void)
{
    if (Player.hp < (Player.maxhp /5))
        print1("A grievously injured ");
    else if (Player.hp < (Player.maxhp /2))
        print1("A seriously wounded ");
    else if (Player.hp < Player.maxhp)
        print1("A somewhat bruised ");
    else print1("A fit ");

    if (Player.status[SHADOWFORM])
        nprint1("shadow");
    else
        nprint1(levelname(Player.level));
    nprint1(" named ");
    nprint1(Player.name);
    if (State.getMounted())
        nprint1(" (riding a horse.)");
}


/* access to player experience... */
/* share out experience among guild memberships */
void gain_experience(int amount)
{
    int i,count=0,share;
    Player.xp += (long) amount;
    gain_level(); /* actually, check to see if should gain level */
    for(i=0; i<NUMRANKS; i++)
        if (Player.guildxp[i] > 0) count++;
    share = amount/(max(count,1));
    for(i=0; i<NUMRANKS; i++)
        if (Player.guildxp[i] > 0) Player.guildxp[i]+=share;
}

/* try to hit a monster in an adjacent space. If there are none
return false. Note if you're berserk you get to attack ALL
adjacent monsters! */
int goberserk(void)
{
    int wentberserk=false,i;
    char combatManeuvers[80];
    strcpy(combatManeuvers,Player.combatManeuvers);
    strcpy(Player.combatManeuvers,"lLlClH");
    for(i=0; i<8; i++)
        if (Level->site[Player.x+Dirs[0][i]][Player.y+Dirs[1][i]].creature
                != NULL) {
            wentberserk=true;
            Level->site[Player.x+Dirs[0][i]][Player.y+Dirs[1][i]].creature->fight_monster();
            morewait();
        }
    strcpy(Player.combatManeuvers,combatManeuvers);
    return(wentberserk);
}

/* identifies a trap for examine() by its aux value */
const char *trapid(int trapno)
{
    switch (trapno) {
    case L_TRAP_SIREN:
        return("A siren trap");
    case L_TRAP_DART:
        return("A dart trap");
    case L_TRAP_PIT:
        return("A pit");
    case L_TRAP_SNARE:
        return("A snare");
    case L_TRAP_BLADE:
        return("A blade trap");
    case L_TRAP_FIRE:
        return("A fire trap");
    case L_TRAP_TELEPORT:
        return("A teleport trap");
    case L_TRAP_DISINTEGRATE:
        return("A disintegration trap");
    case L_TRAP_DOOR:
        return("A trap door");
    case L_TRAP_MANADRAIN:
        return("A manadrain trap");
    case L_TRAP_ACID:
        return("An acid shower trap");
    case L_TRAP_SLEEP_GAS:
        return("A sleep gas trap");
    case L_TRAP_ABYSS:
        return("A concealed entrance to the abyss");
    default:
        return("A completely inoperative trap.");
    }
}


/* checks current food status of player, every hour, and when food is eaten */
void foodcheck(void)
{
    if (Player.food > 48) {
        print3("You vomit up your huge meal.");
        Player.food = 12;
    }
    else if (Player.food == 30)
        print3("Time for a smackerel of something.");
    else if (Player.food == 20)
        print3("You feel hungry.");
    else if (Player.food == 12)
        print3("You are ravenously hungry.");
    else if (Player.food == 3) {
        print3("You feel weak.");
        if (State.getFastMove()) {
            drawvision(Player.x,Player.y);
            State.setFastMove(false);
        }
    }
    else if (Player.food < 0) {
        if (State.getFastMove()) {
            drawvision(Player.x,Player.y);
            State.setFastMove(false);
        }
        print3("You're starving!");
        p_damage(-5*Player.food,UNSTOPPABLE,"starvation");
    }
    showflags();
}




/* see whether room should be illuminated */
void roomcheck(void)
{
    static int oldroomno = -1;
#if defined(WIN32)
    static int oldlevel = -1;
#else
    static plv oldlevel = NULL;
#endif
    int roomno = Level->site[Player.x][Player.y].roomnumber;

    if ((roomno == RS_CAVERN) ||
            (roomno == RS_SEWER_DUCT) ||
            (roomno == RS_KITCHEN) ||
            (roomno == RS_BATHROOM) ||
            (roomno == RS_BEDROOM) ||
            (roomno == RS_DININGROOM) ||
            (roomno == RS_CLOSET) ||
            (roomno > ROOMBASE))
        if ((! loc_statusp(Player.x,Player.y,LIT)) &&
                (! Player.status[BLINDED]) &&
                (Player.status[ILLUMINATION] || (difficulty() < 6))) {
            showroom(Level->site[Player.x][Player.y].roomnumber);
            spreadroomlight(Player.x,Player.y,roomno);
            levelrefresh();
        }
    if ((oldroomno != roomno) ||
#if defined(WIN32)
            (oldlevel != Level->depth)) {
#else
            (oldlevel != Level)) {
#endif
        showroom(roomno);
        oldroomno = roomno;
#if defined(WIN32)
        oldlevel = Level->depth;
#else
        oldlevel = Level;
#endif
    }
}



/* name of the player's experience level */
char *levelname(int level)
{
    switch(level) {
    case 0:
        strcpy(Str3,"neophyte");
        break;
    case 1:
        strcpy(Str3,"beginner");
        break;
    case 2:
        strcpy(Str3,"tourist");
        break;
    case 3:
        strcpy(Str3,"traveller");
        break;
    case 4:
        strcpy(Str3,"wayfarer");
        break;
    case 5:
        strcpy(Str3,"peregrinator");
        break;
    case 6:
        strcpy(Str3,"wanderer");
        break;
    case 7:
        strcpy(Str3,"hunter");
        break;
    case 8:
        strcpy(Str3,"scout");
        break;
    case 9:
        strcpy(Str3,"trailblazer");
        break;
    case 10:
        strcpy(Str3,"discoverer");
        break;
    case 11:
        strcpy(Str3,"explorer");
        break;
    case 12:
        strcpy(Str3,"senior explorer");
        break;
    case 13:
        strcpy(Str3,"ranger");
        break;
    case 14:
        strcpy(Str3,"ranger captain");
        break;
    case 15:
        strcpy(Str3,"ranger knight");
        break;
    case 16:
        strcpy(Str3,"adventurer");
        break;
    case 17:
        strcpy(Str3,"experienced adventurer");
        break;
    case 18:
        strcpy(Str3,"skilled adventurer");
        break;
    case 19:
        strcpy(Str3,"master adventurer");
        break;
    case 20:
        strcpy(Str3,"hero");
        break;
    case 21:
        strcpy(Str3,"superhero");
        break;
    case 22:
        strcpy(Str3,"demigod");
        break;
    default:
        if (level < 100) {
            strcpy(Str3,"Order ");
            Str3[6] = ((level/10)-2) + '0';
            Str3[7] = 0;
            strcat(Str3," Master of Omega");
        }
        else strcpy(Str3,"Ultimate Master of Omega");
        break;
    }
    return(Str3);
}
