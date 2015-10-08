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
	float mass;
	float e;

	sf::Sprite ball;
	sf::Sprite stripe;
	sf::Sprite shadow;
	bool striped;

	void init(vec2 _pos, int _mass, sf::Texture* ballTex, sf::Texture* ballStripeTex, sf::Texture* shadowTex, int colorType, bool striped = false);
	void push(float force, vec2 dir);
	void drawShadow(sf::RenderTarget& target, sf::RenderStates states) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void recolor(int type, sf::Sprite* _sprite);
};

#endif