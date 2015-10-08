#ifndef BALL_H
#define BALL_H

#include "../SFML/Graphics.hpp"
#include "vec2.h"

#define RADIUS 25

class Ball
{
public:
	vec2 pos;
	vec2 v;
	int mass;

	sf::Sprite ball;
	sf::Sprite stripe;
	bool striped;

	void init(vec2 _pos, int _mass, sf::Texture* ballTex, sf::Texture* ballStripeTex, float hueShift, bool striped = false);
	void push(float force, vec2 dir);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void hue(float hueShift, sf::Sprite* _sprite);
};

#endif