#include "Game.h"

Game::~Game()
{

}

void Game::init(InputStruct* _in, sf::RenderWindow* _rw)
{
	in = _in;
	rw = _rw;
	table.init(_in, _rw);

	charge = 0;
	lastLMB = false;
}

void Game::update(float deltaTime)
{
	//draws a shadow where the mouse is to make sure it can read the mouse pos correctly
	table.mpostest(in->mouse);

	//Charges upp the power that fires away balls ----
	if (in->LMB)
		charge += 1 * deltaTime;
	else
		if (lastLMB == true) //button released
		{
			table.fire(in->mouse, charge);
			charge = 0;
		}

	// Slow-motion button
	if (in->Space)
		deltaTime = deltaTime / 5;

	//check if a new senario should be loaded
	int load = in->nrReleased();
	if (load != -1)
		table.loadSenario(load);

	table.update(deltaTime);

	lastLMB = in->LMB;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	table.draw(target, states);
}