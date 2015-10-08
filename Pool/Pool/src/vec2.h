#ifndef VEC2_H
#define VEC2_H

#include <math.h>

class vec2
{
public:
	float x, y;

	vec2()
	{
		x = 0;
		y = 0;
	};

	vec2(float _x, float _y)
	{
		x = _x;
		y = _y;
	};

	float dot(vec2 in)
	{
		return (x * in.x + y * in.y);
	};

	void set(float _x, float _y)
	{
		x = _x;
		y = _y;
	};

	float len()
	{
		return sqrtf( x * x + y * y);
	};

	void normalize()
	{
		float length = len();
		x = x / length;
		y = y / length;
	};

	vec2 operator*(float val)
	{
		return vec2(x * val, y * val);
	};

	vec2 operator+(vec2 in)
	{
		return vec2(x + in.x, y + in.y);
	};

	vec2 operator-(vec2 in)
	{
		return vec2(x - in.x, y - in.y);
	}

	vec2 operator/(float in)
	{
		return vec2(x / in, y / in);
	}

	void operator=(vec2 in)
	{
		x = in.x;
		y = in.y;
	};
	
	void inverse()
	{
		x *= -1;
		y *= -1;
	}
};

#endif