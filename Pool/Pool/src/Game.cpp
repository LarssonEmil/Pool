#include "Game.h"

Game::~Game()
{

}

void Game::init(InputStruct* _in)
{
	in = _in;
	table.init();

	charge = 0;
	lastLMB = false;
}

void Game::update(float deltaTime)
{

	table.mpostest(in->mouse);

	if (in->LMB)
		charge += 1 * deltaTime;
	else
	{
		if (lastLMB == true) //button released
		{
			table.fire(in->mouse, charge);
			charge = 0;
		}
	}

	table.update(deltaTime);

	lastLMB = in->LMB;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	table.draw(target, states);
}