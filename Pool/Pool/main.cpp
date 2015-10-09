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
}


void gameInit()
{
	cout << "Physics Project" << endl;
	g.init(&in, &rw);

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
	case sf::Event::Closed: rw.close(); break;
	case sf::Event::KeyPressed:
		switch (sfEvent.key.code)
		{
		case sf::Keyboard::Num1: in.last_1 = true; break;
		case sf::Keyboard::Num2: in.last_2 = true; break;
		case sf::Keyboard::Num3: in.last_3 = true; break;
		case sf::Keyboard::Num4: in.last_4 = true; break;
		case sf::Keyboard::Num5: in.last_5 = true; break;
		case sf::Keyboard::Num6: in.last_6 = true; break;
		case sf::Keyboard::Num7: in.last_7 = true; break;
		case sf::Keyboard::Num8: in.last_8 = true; break;
		case sf::Keyboard::Num9: in.last_9 = true; break;
		case sf::Keyboard::Num0: in.last_0 = true; break;
		}
	case sf::Event::KeyReleased:
		switch (sfEvent.key.code)
		{
		case sf::Keyboard::Num1: in.last_1 == true ? in.released_1 = true : in.released_1 = false; in.last_1 = false; break;
		case sf::Keyboard::Num2: in.last_2 == true ? in.released_2 = true : in.released_2 = false; in.last_2 = false; break;
		case sf::Keyboard::Num3: in.last_3 == true ? in.released_3 = true : in.released_3 = false; in.last_3 = false; break;
		case sf::Keyboard::Num4: in.last_4 == true ? in.released_4 = true : in.released_4 = false; in.last_4 = false; break;
		case sf::Keyboard::Num5: in.last_5 == true ? in.released_5 = true : in.released_5 = false; in.last_5 = false; break;
		case sf::Keyboard::Num6: in.last_6 == true ? in.released_6 = true : in.released_6 = false; in.last_6 = false; break;
		case sf::Keyboard::Num7: in.last_7 == true ? in.released_7 = true : in.released_7 = false; in.last_7 = false; break;
		case sf::Keyboard::Num8: in.last_8 == true ? in.released_8 = true : in.released_8 = false; in.last_8 = false; break;
		case sf::Keyboard::Num9: in.last_9 == true ? in.released_9 = true : in.released_9 = false; in.last_9 = false; break;
		case sf::Keyboard::Num0: in.last_0 == true ? in.released_0 = true : in.released_0 = false; in.last_0 = false; break;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		in.Space = true;
	else
		in.Space = false;
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		in.LMB = true;
	else
		in.LMB = false;

	in.mouse.x = sf::Mouse::getPosition().x - rw.getPosition().x - 9;
	in.mouse.y = sf::Mouse::getPosition().y - rw.getPosition().y - 38;
}