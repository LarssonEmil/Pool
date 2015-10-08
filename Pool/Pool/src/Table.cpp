#include "Table.h"

Table::~Table()
{

}

void Table::init()
{
	//Botom texture
	botLayer.setPosition(0, 0);
	botTexture.loadFromFile("src/Textures/Table BotLayer.png");
	botLayer.setTexture(botTexture, true);
	botLayer.setTextureRect(sf::IntRect(0, 0, botTexture.getSize().x, botTexture.getSize().y));

	//Top texture
	topLayer.setPosition(0, 0);
	topTexture.loadFromFile("src/Textures/Table TopLayer.png");
	topLayer.setTexture(topTexture, true);
	topLayer.setTextureRect(sf::IntRect(0, 0, topTexture.getSize().x, topTexture.getSize().y));

	//Ball textures (load once, use same ref by all balls)
	ballTex.loadFromFile("src/Textures/white.png");
	ballStripeTex.loadFromFile("src/Textures/stripe.png");

	//Ball shoadow. One shadow re-positioned for each ball (saves memory), done in Table
	ballShadowTex.loadFromFile("src/Textures/shadow.png");
	ballShadow.setTexture(ballShadowTex, true);
	ballShadow.setTextureRect(sf::IntRect(0, 0, ballShadowTex.getSize().x, ballShadowTex.getSize().y));

	b.push_back(Ball());
	b.push_back(Ball());
	b.push_back(Ball());

	b[0].init(vec2(120, 120), 1, &ballTex, &ballStripeTex, -1, false);
	b[1].init(vec2(200, 200), 1, &ballTex, &ballStripeTex, 0.5, false);
	b[2].init(vec2(400, 400), 1, &ballTex, &ballStripeTex, 0.3, true);
}

void Table::update(float deltaTime)
{
	moveBalls(deltaTime);

	collideWall();
}

void Table::moveBalls(float deltaTime)
{
	int size = b.size();

	for (int n = 0; n < size; n++)
	{	
		b[n].pos = b[n].pos + b[n].v * deltaTime;

		b[n].ball.setPosition(b[n].pos.x - RADIUS, b[n].pos.y - RADIUS);
		b[n].stripe.setPosition(b[n].pos.x - RADIUS, b[n].pos.y - RADIUS);
	}
}

void Table::collideWall()
{
	int size = b.size();

	for (int n = 0; n < size; n++)
	{
		float yTop = b[n].pos.y - TABLE_EDGE - RADIUS;
		if (yTop < 0)
			b[n].pos.y += (yTop* -1) * 2;

		else if()
	}
}


void Table::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(botLayer);
	//draw ball shadows
	//draw balls
	balls[0].draw(target, states);
	balls[1].draw(target, states);
	balls[2].draw(target, states);
	target.draw(topLayer);
}
