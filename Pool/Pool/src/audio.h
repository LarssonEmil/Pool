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
		float slowIncrease = 1;
		if (slow)
		{
			pow *= 0.2;
			slowIncrease = 6;
		} 


		switch (id)
		{
		case 1: fire.setVolume(pow * 60 * slowIncrease); fire.setPitch(0.5 + pow);  fire.play(); return;
		case 2: ballCollide.setVolume(pow * 60 * slowIncrease); ballCollide.setPitch(0.75 + pow / 2); ballCollide.play(); return;
		case 3: wallCollide.setVolume(pow * 60 * slowIncrease); wallCollide.setPitch(0.5 + pow);  wallCollide.play(); return;
		case 4: pocket.setVolume(pow * 60 * slowIncrease);  pocket.setPitch(pow);  pocket.play(); return;
		}
	}
};

#endif