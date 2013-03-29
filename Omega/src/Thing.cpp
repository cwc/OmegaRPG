

#include "Thing.h"
#include "glob.h"
#include "Grot.h"
#include "GarageDoorOpener.h"

// Percent chance that a grot will be created on random generation
#define GROT_CHANCE 15

// Total number of possible thing objects
#define NUM_THING_OBJS 1


Thing::Thing( void )
{
}


Thing::~Thing( void )
{
}


Thing Thing::create_object( int id = -1 )
{
    int chance = random_range(100);
    if ( chance < GROT_CHANCE )
    {        
    	return Grot();
    } 
    else
    {
    	int chance = random_range(NUM_THING_OBJS);
    	switch ( chance )
    	{
    		case 1:
    			return GarageDoorOpener();
    		default:
    			return Grot();
    	}

    }    
    return Thing();
        
}
