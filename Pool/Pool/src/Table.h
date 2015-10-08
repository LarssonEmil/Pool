#ifndef TABLE_H
#define TABLE_H

#include "../SFML/Graphics.hpp"
#include "Ball.h"
#include <vector>

#define TABLE_EDGE 84
#define TABLE_LENGTH 1579
#define TABLE_HEIGHT 873

using std::vector;

class Table
{
private:
	sf::Sprite botLayer;
	sf::Texture botTexture;

	sf::Sprite topLayer;
	sf::Texture topTexture;

	sf::Texture ballTex;
	sf::Texture ballStripeTex;

	sf::Sprite ballShadow;
	sf::Texture ballShadowTex;

	//balls
	vector<Ball> b;

public:
	~Table();
	void init();
	void update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
	void moveBalls(float deltaTime);
	void collideWall();
};

#endif