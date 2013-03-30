/* omega copyright (c) 2013  */
/* Scroll - scroll objects */

#ifndef SCROLL_H
#define SCROLL_H

#pragma once

#include "Object.h"

class Scroll : public Object
{
public:
    Scroll(void);
    ~Scroll(void);
    void initialize();
    void use();
};

#endif


