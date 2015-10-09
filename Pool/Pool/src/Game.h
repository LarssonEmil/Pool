#ifndef GAME_H
#define GAME_H

#include "../SFML/Graphics.hpp"
#include <string>

#include "Table.h"
#include "../src/InputStruct.h"

class Game : public sf::Drawable
{

private:
	Table table;
	InputStruct* in;
	sf::RenderWindow* rw;

	float charge;
	bool lastLMB;
public:
	void init(InputStruct* _in, sf::RenderWindow* _rw);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	~Game();
	void update(float deltaTime);

};

#endif