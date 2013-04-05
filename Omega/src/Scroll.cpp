
#include "Scroll.h"
#include "glob.h"

Scroll::Scroll(void)
{
}


Scroll::~Scroll(void)
{
}

/* Initialize a basic scroll */
void Scroll::initialize()
{
    id = SCROLLID+0;
	weight = 5;
	plus = charge = dmg = hit = blessing = aux = 0;
    known = 0;
	fragility = number = 1 ;
	
	basevalue = 5;
    uniqueness = COMMON;
    usef = I_NOTHING;
    type = 0;
    used = 0;
    level = 0;
    objchar = SCROLL;
    objstr = "?";
    truename = "blank scroll";
    cursestr = "Antinomy!";



}


/* Use the scroll */
void Scroll::use()
{

}
