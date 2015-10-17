#ifndef AUDIOO_H
#define AUDIOO_H

#include "../SFML/Audio.hpp"

class Audioo
{
private:
	sf::SoundBuffer b_fire;
	sf::Sound fire;

	sf::SoundBuffer b_ballCollide;
	sf::Sound ballCollide;

	sf::SoundBuffer b_wallCollide;
	sf::Sound wallCollide;

	sf::SoundBuffer b_pocket;
	sf::Sound pocket;
public:
	void init()
	{
		b_fire.loadFromFile("src/Sounds/fire.flac");
		fire.setBuffer(b_fire);

		b_ballCollide.loadFromFile("src/Sounds/ballCollision.flac");
		ballCollide.setBuffer(b_ballCollide);

		b_wallCollide.loadFromFile("src/Sounds/wallCollision.flac");
		wallCollide.setBuffer(b_wallCollide);

		b_pocket.loadFromFile("src/Sounds/pocket.flac");
		pocket.setBuffer(b_pocket);
	}

	void play(int id, float max, float val, bool slow)
	{

		float pow = val / max;
		
		if (!slow)
		{
			switch (id)
			{
			case 1: fire.setVolume(pow * 60); fire.setPitch(0.5 + pow);  fire.play(); return;
			case 2: ballCollide.setVolume(pow * 60); ballCollide.setPitch(0.75 + pow / 2); ballCollide.play(); return;
			case 3: wallCollide.setVolume(pow * 60); wallCollide.setPitch(0.2 + pow);  wallCollide.play(); return;
			case 4: pocket.setVolume(pow * 50);  pocket.setPitch(pow);  pocket.play(); return;
			}
		}
		else
		{
			pow *= 0.3;
			switch (id)
			{
			case 1: fire.setVolume(pow * 80); fire.setPitch(0.3 + pow);  fire.play(); return;
			case 2: ballCollide.setVolume(pow * 80); ballCollide.setPitch(0.5 + pow / 2); ballCollide.play(); return;
			case 3: wallCollide.setVolume(pow * 80); wallCollide.setPitch(0.1 + pow);  wallCollide.play(); return;
			case 4: pocket.setVolume(pow * 50);  pocket.setPitch(0.7);  pocket.play(); return;
			}
		}
	}
};

#endif