#include "Ball.h"

void Ball::init(vec2 _pos, int _mass, sf::Texture* ballTex, sf::Texture* ballStripeTex, sf::Texture* shadowTex, int colorType)
{
	pos = _pos;
	mass = _mass;
	v = vec2(0, 0);
	e = 1;
	colortype = colorType;

	lastCollide = 0;

	if (colorType > 8)
		striped = true;
	else
		striped = false;

	ball.setPosition(pos.x - RADIUS, pos.y - RADIUS);
	ball.setTexture(*ballTex, true);
	ball.setTextureRect(sf::IntRect(0, 0, ballTex->getSize().x, ballTex->getSize().y));

	shadow.setPosition(pos.x, pos.y);
	shadow.setTexture(*shadowTex, true);
	ball.setTextureRect(sf::IntRect(0, 0, shadowTex->getSize().x, shadowTex->getSize().y));

	if (striped)
	{
		stripe.setPosition(pos.x - RADIUS, pos.y - RADIUS);
		stripe.setTexture(*ballStripeTex, true);
		stripe.setTextureRect(sf::IntRect(0, 0, ballStripeTex->getSize().x, ballStripeTex->getSize().y));

		recolor(colorType, &stripe);
		return;
	}
	if(colorType > 0 && colorType < 16)
		recolor(colorType, &ball);
}

void Ball::push(float force, vec2 dir)
{
	dir.normalize();
	force = force / mass;
	v = v + dir * force;
}

void Ball::drawShadow(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shadow);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ball);
	if(striped)
		target.draw(stripe);
}

void Ball::recolor(int type, sf::Sprite* _sprite)
{
	sf::Color c;
	switch (type)
	{
	case 1: 
	case 9:c.r = 255; c.g = 178; c.b = 60;  break;
	case 2:
	case 10: c.r = 23; c.g = 85; c.b = 124;  break;
	case 3: 
	case 11: c.r = 221; c.g = 66; c.b = 61;  break;
	case 4:
	case 12: c.r = 78; c.g = 42; c.b = 112;  break;
	case 5: 
	case 13: c.r = 243; c.g = 117; c.b = 43;  break;
	case 6:
	case 14: c.r = 40; c.g = 102; c.b = 53;  break;
	case 7: 
	case 15: c.r = 150; c.g = 25; c.b = 29;  break;
	case 8: c.r = 50; c.g = 50; c.b = 50;  break;
	}

	_sprite->setColor(c);
}