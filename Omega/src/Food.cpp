#include <string>
#include "glob.h"
#include "Food.h"

Food::Food(void)
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
	objstr = "food ration";
	truename = "food ration";
	cursestr = "food ration";
}

Food::~Food(void)
{
}

void Food::initialize()
{
}

// Consume the food. Destroying object takes place in eat command: command2.c/eat()
void Food::use()
{
	// Adjust player's hunger
	Player.food = max(0, Player.food + aux);

	std::string comments[] = {
		"That tasted horrible!",
		"Yum!",
		"How nauseous!",
		"Can I have some more? Please?",
		"Your mouth feels like it is growing hair!",
		"Tastes like home cooking!",
		"Appalling."
	};

	mprint((char*)comments[random_range(7)].c_str());
}
