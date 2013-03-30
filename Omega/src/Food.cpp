
#include "Food.h"
#include "glob.h"

Food::Food(void)
{
}


Food::~Food(void)
{
}

/* for now only initialize basic food object */
void Food::initialize()
{
	weight = 20;
	plus = charge = dmg = hit = fragility = blessing = 0;

	/* how full the food makes you feel */
	aux = 8;
	
	number = 1;
	basevalue = 2;

	uniqueness = COMMON;
	level = 0;
	objchar = FOOD;
	usef = I_FOOD;
	objstr = "red and white striped bucket";
	truename = "food ration";
	cursestr = "food ration";

}


/* Consume the food. Decreasing hunger and destroying
   object is actually in command2.c/eat() */
void Food::use()
{
    switch (random_range(6))
    {
    case 0:
        mprint("That tasted horrible!");
        break;
    case 1:
        mprint("Yum!");
        break;
    case 2:
        mprint("How nauseous!");
        break;
    case 3:
        mprint("Can I have some more? Please?");
        break;
    case 4:
        mprint("Your mouth feels like it is growing hair!");
        break;
    case 5:
        mprint("Tastes like home cooking!");
        break;
    }
}