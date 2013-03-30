/* omega copyright (c) 2013  */
/* food - basic food objects and super class for special foods */

#ifndef FOOD_H
#define FOOD_H

#pragma once

#include "Object.h"

class Food : public Object
{
public:
    Food(void);
    ~Food(void);
    void initialize();
    void use();
};

#endif


