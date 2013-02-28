#pragma once


class Village
{
	
private:
	MonsterList *tml;

public:
	
	enum {
		VIL_NONE,
		VIL_STARVIEW,
		VIL_WOODMERE,
		VIL_STORMWAT,
		VIL_THAUMARI,
		VIL_SKORCH,
		VIL_WHORFEN
	};
	
	void load_village(int villagenum, int populate);
	void make_guard(int i, int j);
	void make_sheep(int i, int j);
	void make_food_bin(int i, int j);
	void make_horse(int i, int j);
	void make_merchant(int i, int j);
	void assign_village_function(int x, int y, int setup);
	void special_village_site(int i, int j, int villagenum);

};

