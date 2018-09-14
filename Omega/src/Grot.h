/* omega copyright (c) 2013  */
/* grot - misc world flavor objects */

#ifndef GROT_H
#define GROT_H

#include "Thing.h"

class Grot : public Thing
{
private:    
    const char* nameGrot();
    void initialize();

public:
    Grot(void);
    ~Grot(void);    
    void createObject();    
    void use();

};

#endif
