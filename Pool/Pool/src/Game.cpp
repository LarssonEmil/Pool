#include "Game.h"

Game::~Game()
{

}

void Game::init()
{
	table.init();
}

void Game::update(float deltaTime)
{
	table.update(deltaTime);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	table.draw(target, states);
}