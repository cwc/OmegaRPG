
#include "Grot.h"
#include "glob.h"

/* number of possible grots */
#define NUM_GROTS 21

/* Creating this object without any parameters generates a random object */
Grot::Grot(void)
{
    const char *name = nameGrot();
    truename = cursestr = objstr = name;
    

}

Grot::~Grot(void)
{

}

void Grot::use()
{

}

void Grot::initialize()
{
    weight = 10;
    fragility = number = 1;
    plus = charge = dmg = hit = aux = blessing = usef = 0;
    basevalue = 12;
    uniqueness = COMMON;
    objstr = truename = cursestr = nameGrot();
    

}

const char * Grot::nameGrot()
{
    switch(random_range(NUM_GROTS)) {
        case 0:
            return "pot lid";
        case 1:
            return "mound of offal";
        case 2:
            return "sword that was broken";
        case 3:
            return "salted snail";
        case 4:
            return "key";
        case 5:
            return "toadstool";
        case 6:
            return "greenish spindle";
        case 7:
            return "tin soldier";
        case 8:
            return "broken yo-yo";
        case 9:
            return "NYC subway map";
        case 10:
            return "Nixon's the One! button";
        case 11:
            return "beer can (empty)";
        case 12:
            return "golden bejewelled falcon";
        case 13:
            return "hamster cage";
        case 14:
            return "wooden nickel";
        case 15:
            return "three-dollar bill";
        case 16:
            return "rosebud";
        case 17:
            return "water pistol";
        case 18:
            return "shattered skull";
        case 19:
            return "black bag";
        default:
        case 20:
            return "jawbone of an ass";
    }
}
