#pragma once
#include "Object.h"

class Food : public Object
{
public:
	Food();
	~Food();

	virtual void initialize();
	virtual void use();
};
