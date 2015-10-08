#ifndef INPUT_STRUCT_H
#define INPUT_STRUCT_H

#include "../SFML/Graphics.hpp"
#include "vec2.h"

struct InputStruct
{
public:
	bool LMB = false;
	vec2 mouse;
};

#endif