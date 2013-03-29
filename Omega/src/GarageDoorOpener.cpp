
#include "GarageDoorOpener.h"
#include "glob.h"

GarageDoorOpener::GarageDoorOpener( void ) {}
GarageDoorOpener::~GarageDoorOpener( void ) {}
void GarageDoorOpener::use()
{
	
	/* raise the portcullis on the map */
	l_raise_portcullis();
	mprint("The box beeps once and explodes in your hands!");

	/* remove the item from the player's inventory */
	conform_lost_objects(1, this);
}

void GarageDoorOpener::initialize()
{

}

