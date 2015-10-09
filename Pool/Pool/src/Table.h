#ifndef TABLE_H
#define TABLE_H

#include "../SFML/Graphics.hpp"
#include "Ball.h"
#include "InputStruct.h"
#include <vector>

#define TABLE_EDGE 84
#define TABLE_LENGTH 1410
#define TABLE_HEIGHT 705

using std::vector;

class Table
{
private:
	InputStruct* in;
	sf::RenderWindow* rw;

	sf::Sprite botLayer;
	sf::Texture botTexture;

	sf::Sprite topLayer;
	sf::Texture topTexture;

	sf::Texture ballTex;
	sf::Texture ballStripeTex;

	sf::Sprite ballShadow;
	sf::Texture ballShadowTex;

	float friction;
	float wallCollisionLoss;
	float firePow;

	bool LastCollisionIgnore;
	bool run;

	//corner pos
	vector<vec2> corners;

	//balls
	vector<Ball> b;

public:
	~Table();
	void init(InputStruct* _in, sf::RenderWindow* _rw);
	void update(float deltaTime);
	void mpostest(vec2 in);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void ballFriction(float deltaTime);
	void moveBalls(float deltaTime);
	void collideWall(float deltaTime);
	void collideBall();

	void fire(vec2 pos, float charge);

	void loadSenario(int id);
};

#endif