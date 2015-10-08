#ifndef GAME_H
#define GAME_H

#include "../SFML/Graphics.hpp"
#include <string>

#include "Table.h"

class Game : public sf::Drawable
{

private:
	Table table;

public:
	void init();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	~Game();
	void update(float deltaTime);

};

#endif