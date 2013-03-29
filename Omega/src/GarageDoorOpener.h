
#ifndef GARAGEDOOROPENER_H
#define GARAGEDOOROPENER_H

#include "Thing.h"


class GarageDoorOpener : public Thing
{

public:
	GarageDoorOpener( void );
	~GarageDoorOpener( void );

	void use();
    void initialize();
};

#endif
