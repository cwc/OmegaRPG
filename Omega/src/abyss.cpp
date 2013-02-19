/* omega copyright (C) by Laurence Raphael Brothers, 1987,1988,1989 */
/* abyss.c */
/* some functions to make the abyss level and run the final challenge */

#include "glob.h"

/* loads the abyss level into Level*/
void load_abyss(void)
{
    int i, j;
    char site;
    map *abyss;

    TempLevel = Level;
    if (ok_to_free(TempLevel)) {
        free_level(TempLevel);
        TempLevel = NULL;
    }

    Level = ((plv) checkmalloc(sizeof(levtype)));

    clear_level(Level);

    abyss = map_open(MAP_abyss);
    map_setLevel(abyss,0);
    Level->level_width = map_getWidth(abyss);
    Level->level_length = map_getLength(abyss);

    for(j=0; j<Level->level_length; j++) {
        for(i=0; i<Level->level_width; i++) {
            site = map_getSiteChar(abyss,i,j);
            Level->site[i][j].roomnumber = RS_ADEPT;
            switch(site) {
            case '0':
                Level->site[i][j].locchar = VOID_CHAR;
                Level->site[i][j].p_locf = L_VOID;
                break;
            case 'V':
                Level->site[i][j].locchar = VOID_CHAR;
                Level->site[i][j].p_locf = L_VOID_STATION;
                break;
            case '1':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_VOICE1;
                break;
            case '2':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_VOICE2;
                break;
            case '3':
                Level->site[i][j].locchar = FLOOR;
                Level->site[i][j].p_locf = L_VOICE3;
                break;
            case '~':
                Level->site[i][j].locchar = WATER;
                Level->site[i][j].p_locf = L_WATER_STATION;
                break;
            case ';':
                Level->site[i][j].locchar = FIRE;
                Level->site[i][j].p_locf = L_FIRE_STATION;
                break;
            case '"':
                Level->site[i][j].locchar = HEDGE;
                Level->site[i][j].p_locf = L_EARTH_STATION;
                break;
            case '6':
                Level->site[i][j].locchar = WHIRLWIND;
                Level->site[i][j].p_locf = L_AIR_STATION;
                break;
            case '#':
                Level->site[i][j].locchar = WALL;
                break;
            case '.':
                Level->site[i][j].locchar = FLOOR;
                break;
            }
        }
    }
    map_close(abyss);
}

