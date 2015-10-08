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

	for (int n = 0; n < 16; n++)
	{
		b.push_back(Ball());
	}

	//white
	b[0].init(vec2(TABLE_EDGE * 5, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1, false);

	//1st row
	b[1].init(vec2(1144 - RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1, false);
	//2nd row
	b[2].init(vec2(1144 - RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 2, false);
	b[3].init(vec2(1144 - RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 3, false);
	//3rd row
	b[4].init(vec2(1144, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 4, false);
	b[5].init(vec2(1144, 433 + RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 5, false);
	b[6].init(vec2(1144, 433 - RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 6, false);
	//4th row
	b[7].init(vec2(1144 + RADIUS * 2.1, 433 - RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 7, false);
	b[8].init(vec2(1144 + RADIUS * 2.1, 433 + RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 8, false);
	b[9].init(vec2(1144 + RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 9, true);
	b[10].init(vec2(1144 + RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 10, true);
	//5th row
	b[11].init(vec2(1144 + RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 11, true);
	b[12].init(vec2(1144 + RADIUS * 4.1, 433 + RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 12, true);
	b[13].init(vec2(1144 + RADIUS * 4.1, 433 - RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 13, true);
	b[14].init(vec2(1144 + RADIUS * 4.1, 433 + RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 14, true);
	b[15].init(vec2(1144 + RADIUS * 4.1, 433 - RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 15, true);

	friction = 100;
	wallCollisionLoss = 0.8;
	firePow = 2000;

	b[0].mass = 1;
	b[0].e = 1;

	corners.push_back(vec2(100, 100)); //top left
	corners.push_back(vec2(788, 72)); // top middle
	corners.push_back(vec2(1478, 98)); // top right
	corners.push_back(vec2(100, 771)); // bot left
	corners.push_back(vec2(790, 797)); // bot middle
	corners.push_back(vec2(1479, 772)); // bot right
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
		b[n].shadow.setPosition(b[n].pos.x - 10, b[n].pos.y - 10);
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
			if (d.len() < RADIUS * 2) 
			{
				d.normalize();
				vec2 va0 = d * b[n].v.dot(d);
				vec2 vb0 = d * b[k].v.dot(d);

				//elastisk average
				float e = (b[n].e + b[k].e) / 2;

				vec2 va1 = (va0 * (b[n].mass - b[k].mass * e) +
					vb0 * b[k].mass * (1 + e)) / (b[n].mass + b[k].mass);

				vec2 vb1 = (va0 * b[n].mass * (1 + e) +
					vb0 * (b[k].mass - b[n].mass * e)) / (b[n].mass + b[k].mass);
				
				//apply new velocity

				b[n].v = b[n].v - va0 + va1;
				b[k].v = b[k].v - vb0 + vb1;
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
		{
			closestDist = d.len();
			closestID = n;
		}
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

	int size = b.size();
	//draw ball shadows
	for (int n = 0; n < size; n++)
		b[n].drawShadow(target, states);

	//draw balls
	for (int n = 0; n < size; n++)
		b[n].draw(target, states);

	target.draw(topLayer);
	target.draw(ballShadow);
	
}
