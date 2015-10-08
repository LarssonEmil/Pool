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

	b[0].push(1000, vec2(1, 0));
	b[1].push(1000, vec2(1, 1));
	b[2].push(1000, vec2(1, -0.75));

	friction = 100;
	wallCollisionLoss = 0.8;
	firePow = 20000;
}

void Table::update(float deltaTime)
{
	ballFriction(deltaTime);

	moveBalls(deltaTime);

	collideWall();

	collideBall();
}

void Table::ballFriction(float deltaTime)
{
	int size = b.size();

	for (int n = 0; n < size; n++)
	{
		//If velocity is low, set it to 0
		if (b[n].v.len() < 3)
		{
			b[n].v = vec2(0, 0);
			continue;
		}

		//Friction force
		vec2 fricVec = b[n].v;
		fricVec.normalize();
		fricVec.inverse();

		b[n].push(friction * deltaTime, fricVec);
			
	}
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
		int collide = 0;

		// Y DIR

		float yTop = b[n].pos.y - TABLE_EDGE - RADIUS;
		if (yTop < 0)
		{
			b[n].pos.y += (yTop* -1) * 2;
			b[n].v.y *= -1; //inverse y speed dir
			collide++;
		}
	
		float yBot = b[n].pos.y - TABLE_EDGE + RADIUS - TABLE_HEIGHT;
		if (yBot > 0)
		{
			b[n].pos.y -= (yBot * 2);
			b[n].v.y *= -1; //inverse y speed dir
			collide++;
		}

		// X DIR

		float left = b[n].pos.x - TABLE_EDGE - RADIUS;
		if (left < 0)
		{
			b[n].pos.x += (left* -1) * 2;
			b[n].v.x *= -1; //inverse x speed dir
			collide++;
		}

		float right = b[n].pos.x - TABLE_EDGE + RADIUS - TABLE_LENGTH;
		if (right > 0)
		{
			b[n].pos.x -= (right * 2);
			b[n].v.x *= -1; //inverse x speed dir
			collide++;
		}

		//energy loss on each collision
		if (collide > 0)
			for (int k = 0; k < collide; k++)
				b[n].v = b[n].v * wallCollisionLoss;
	}
}

void Table::collideBall()
{
	int size = b.size();

	for (int n = 0; n < size; n++)
	{
		/*To not detect same collision more than once, 
		ball 1 checks the rest of the balls
		ball 2 checks all other balls exept ball 1
		ball 3 checks all balls other than ball 1 and 2 ...*/

		for (int k = n + 1; k < size; k++)
		{
			//distance vector between the balls
			vec2 d = b[n].pos - b[k].pos;

			//if distance less than radius = collision
			if (d.len() < RADIUS) 
			{
				float va = b[n].v.dot(d);
				float vb = b[k].v.dot(d);

				//elastisk collision

			}
		}

	}
}

void Table::fire(vec2 pos, float charge)
{
	//Find the closest ball
	int closestID = -1;
	float closestDist = 2000;
	int size = b.size();
	for (int n = 0; n < size; n++)
	{
		vec2 d = pos - b[n].pos;
		if (d.len() < closestDist)
			closestID = n;
	}

	//if none found
	if (closestID == -1)
		return;

	vec2 d = pos - b[closestID].pos;
	d.inverse();

	b[closestID].push(charge * firePow, d);
}

void Table::mpostest(vec2 in)
{
	ballShadow.setPosition(in.x - RADIUS, in.y - RADIUS);
}

void Table::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(botLayer);
	//draw ball shadows
	//draw balls
	b[0].draw(target, states);
	b[1].draw(target, states);
	b[2].draw(target, states);
	target.draw(topLayer);
	target.draw(ballShadow);
	
}
