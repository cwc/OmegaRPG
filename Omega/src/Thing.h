/* omega copyright (c) 2013  */
/* thing - parent class for thing objects*/

#ifndef THING_H
#define THING_H

#include "Object.h"
#define NUM_THING_OBJ 31

class Thing : public Object
{
private:

public:
    Thing(void);
    ~Thing(void);
    
    Thing create_object( int );
};

#endif
