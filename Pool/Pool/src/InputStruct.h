#ifndef INPUT_STRUCT_H
#define INPUT_STRUCT_H

#include "../SFML/Graphics.hpp"
#include "vec2.h"

struct InputStruct
{
public:
	bool LMB = false;
	bool Space = false;
	vec2 mouse;

	bool released_1 = false;
	bool released_2 = false;
	bool released_3 = false;
	bool released_4 = false;
	bool released_5 = false;
	bool released_6 = false;
	bool released_7 = false;
	bool released_8 = false;
	bool released_9 = false;
	bool released_0 = false;

	bool last_1 = false;
	bool last_2 = false;
	bool last_3 = false;
	bool last_4 = false;
	bool last_5 = false;
	bool last_6 = false;
	bool last_7 = false;
	bool last_8 = false;
	bool last_9 = false;
	bool last_0 = false;

	int nrReleased()
	{
		if (released_1) return 1;
		if (released_2) return 2;
		if (released_3) return 3;
		if (released_4) return 4;
		if (released_5) return 5;
		if (released_6) return 6;
		if (released_7) return 7;
		if (released_8) return 8;
		if (released_9) return 9;
		if (released_0) return 0;

		return -1;
	}
};

#endif