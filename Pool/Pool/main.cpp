#include <iostream>
#include <Windows.h>
#include "../SFML/Graphics.hpp"

#include <ctime>
#include <sstream>

using namespace std;

sf::RenderWindow rw;
sf::ContextSettings setting;
void init();

sf::Event sfEvent;
void input();

void gameInit();
void gameLoop();

using namespace std;

int main()
{
	rw.create(sf::VideoMode(800, 800), "title", sf::Style::Close, setting);
	rw.setVerticalSyncEnabled(false);

	while (true)
	{
		int k = 0;
	}
};
