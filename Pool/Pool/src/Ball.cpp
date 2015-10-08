#include "Ball.h"

void Ball::init(vec2 _pos, int _mass, sf::Texture* ballTex, sf::Texture* ballStripeTex, float hueShift, bool _striped)
{
	pos = _pos;
	mass = _mass;
	striped = _striped;

	ball.setPosition(pos.x - RADIUS, pos.y - RADIUS);
	ball.setTexture(*ballTex, true);
	ball.setTextureRect(sf::IntRect(0, 0, ballTex->getSize().x, ballTex->getSize().y));

	if (striped)
	{
		stripe.setPosition(pos.x - RADIUS, pos.y - RADIUS);
		stripe.setTexture(*ballStripeTex, true);
		stripe.setTextureRect(sf::IntRect(0, 0, ballStripeTex->getSize().x, ballStripeTex->getSize().y));

		hue(hueShift, &stripe);
		return;
	}
	if(hueShift > 0)
		hue(hueShift, &ball);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ball);
	if(striped)
		target.draw(stripe);
}

void Ball::hue(float hueShift, sf::Sprite* _sprite)
{
	float* lastHue = new float(0);
	*lastHue = 0;

	int r = 0;
	int g = 0;
	int b = 0;

	if (hueShift > 1 || hueShift < -1)
		return;

	*lastHue += hueShift;
	if (*lastHue > 1.0f)
		*lastHue -= 1.0f;
	else if (*lastHue < 0.0f)
		*lastHue += 1.0f;

	hueShift = *lastHue;

	//what color to keep
	if (hueShift > 0.82f || hueShift < 0.16f)
		r = 254;
	else if (hueShift > 0.15999f && hueShift < 0.5f)
		g = 254;
	else
		b = 254;

	//what color to scale
	int* temp;

	if (hueShift < 0.16f)
	{
		g = (hueShift / 0.16f) * 250;
	}
	else if (hueShift < 0.32f)
	{
		hueShift -= 0.16f;
		r = 255 - (hueShift / 0.16f) * 250;
	}
	else if (hueShift < 0.5f)
	{
		hueShift -= 0.32f;
		b = (hueShift / 0.18f) * 250;
	}
	else if (hueShift < 0.66f)
	{
		hueShift -= 0.5f;
		g = 255 - (hueShift / 0.16f) * 250;
	}
	else if (hueShift < 0.82f)
	{
		hueShift -= 0.66f;
		r = (hueShift / 0.16f) * 250;
	}
	else
	{
		hueShift -= 0.82f;
		b = 255 - (hueShift / 0.18f) * 250;
	}
	_sprite->setColor(sf::Color(r, g, b));
	delete lastHue;
}
