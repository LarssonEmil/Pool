#include <iostream>
#include <Windows.h>
#include "../SFML/Graphics.hpp"
#include <ctime>
#include <sstream>

#include "../src/Game.h"


using namespace std;

sf::RenderWindow rw;
sf::ContextSettings setting;

sf::Event sfEvent;
InputStruct in;
void input();

Game g;

void gameInit();
void gameLoop();

using namespace std;

int main()
{
	rw.create(sf::VideoMode(1579, 873), "Pool", sf::Style::Close, setting);
	rw.setVerticalSyncEnabled(false);

	gameInit();
	gameLoop();
};


void gameInit()
{
	cout << "Physics Project" << endl;
	g.init(&in);

}

void gameLoop()
{
	double deltaTime = 0.0f;
	float timePass = 0.0f;
	int fps = 0;
	unsigned int start = clock();

	while (rw.isOpen())
	{
		input();

		rw.clear();

		//update game
		g.update(deltaTime);

		rw.draw(g);

		rw.display();

		unsigned int temp = clock();
		deltaTime = unsigned int(temp - start) / double(1000);
		timePass += deltaTime;
		start = temp;
		fps++;

		if (timePass > 1.0f)
		{
			stringstream ss;
			ss << fps;
			rw.setTitle(ss.str());
			timePass = 0.0f; //timePass -= 1.0f;
			fps = 0;
		}
	}
}

void input()
{
	rw.pollEvent(sfEvent);
	
	switch (sfEvent.type)
	{
	case sf::Event::Closed:
		rw.close();
		break;
	default:
		break;
	}
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		in.LMB = true;
	else
		in.LMB = false;

	in.mouse.x = sf::Mouse::getPosition().x - rw.getPosition().x - 9;
	in.mouse.y = sf::Mouse::getPosition().y - rw.getPosition().y - 38;

		/*case sf::Event::KeyPressed:
			switch (sfEvent.key.code)
			{
			case sf::Keyboard::W: in.W = true; break;
			case sf::Keyboard::A: in.A = true; break;
			case sf::Keyboard::S: in.S = true; break;
			case sf::Keyboard::D: in.D = true; break;

			default:
				break;
			}
			break;

		case sf::Event::KeyReleased:
			switch (sfEvent.key.code)
			{
			case sf::Keyboard::W: in.W = false; break;
			case sf::Keyboard::A: in.A = false; break;
			case sf::Keyboard::S: in.S = false; break;
			case sf::Keyboard::D: in.D = false; break;
			default:
				break;
			}*/
}