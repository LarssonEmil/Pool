#include "Table.h"

Table::~Table()
{

}

void Table::init(InputStruct* _in, sf::RenderWindow* _rw, float* _charge)
{
	in = _in;
	rw = _rw;
	charge = _charge;

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

	//Aim texture and sprite
	aimTex.loadFromFile("src/Textures/aim.png");
	for (int n = 0; n < 10; n++)
	{
		aim[n].setTexture(aimTex, true);
		aim[n].setTextureRect(sf::IntRect(0, 0, aimTex.getSize().x, aimTex.getSize().y));
		//if(n != 0)
			//aim[n].setColor(sf::Color(1, 1, 1));
	}
	aim[0].scale(2, 2);
	
	friction = 100;
	wallCollisionLoss = 0.8f;
	firePow = 1000;

	corners.push_back(vec2(100, 100)); //top left
	corners.push_back(vec2(788, 72)); // top middle
	corners.push_back(vec2(1478, 98)); // top right
	corners.push_back(vec2(100, 771)); // bot left
	corners.push_back(vec2(790, 797)); // bot middle
	corners.push_back(vec2(1479, 772)); // bot right

	audio.init();

	loadSenario(1);
}

void Table::update(float deltaTime)
{
	setAimline();

	if (in->Space)
		run = true;
	if(!run)
		return;
	
	ballFriction(deltaTime);

	moveBalls(deltaTime);

	collideWall(deltaTime);

	collideBall();

	spawnWhite();
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

void Table::collideWall(float deltaTime)
{
	int size = b.size();

	for (int n = 0; n < size; n++)
	{
		bool colse_to_hole = false;
		int nrHoles = corners.size();
		for (int k = 0; k < nrHoles; k++)
		{
			//distance to hole k

			vec2 d = b[n].pos - corners[k];

			float len = d.len();

			if (len < RADIUS * 3.0f)
			{
				colse_to_hole = true;
				
				//Remove the ball from the game
				if (len < RADIUS * 1.5f)
				{
					b.erase(b.begin() + n);
					n--;
					size--;

					audio.play(4, 1, 1, in->Space);
					break;
				}
				//add a force towards hole
				d.normalize();
				d.inverse();
				b[n].push((RADIUS * 3.0f - len) * 50.0f * b[n].mass * deltaTime, d);
				break;
			}
		}

		if (colse_to_hole)
			continue;

		int collide = 0;

		float speedLen = 0;

		// Y DIR

		float yTop = b[n].pos.y - TABLE_EDGE - RADIUS;
		if (yTop < 0)
		{
			b[n].pos.y += (yTop* -1) * 2;
			b[n].v.y *= -1; //inverse y speed dir
			speedLen = b[n].v.y;
			collide++;
		}
	
		float yBot = b[n].pos.y - TABLE_EDGE + RADIUS - TABLE_HEIGHT;
		if (yBot > 0)
		{
			b[n].pos.y -= (yBot * 2);
			b[n].v.y *= -1; //inverse y speed dir
			speedLen = b[n].v.y;
			collide++;
		}

		// X DIR

		float left = b[n].pos.x - TABLE_EDGE - RADIUS;
		if (left < 0)
		{
			b[n].pos.x += (left* -1) * 2;
			b[n].v.x *= -1; //inverse x speed dir
			speedLen = b[n].v.x;
			collide++;
		}

		float right = b[n].pos.x - TABLE_EDGE + RADIUS - TABLE_LENGTH;
		if (right > 0)
		{
			b[n].pos.x -= (right * 2);
			b[n].v.x *= -1; //inverse x speed dir
			speedLen = b[n].v.x;
			collide++;
		}

		//energy loss on each collision
		if (collide > 0)
		{
			if (speedLen < 0)
				speedLen *= -1;

			audio.play(3, 500, speedLen, in->Space);
			for (int k = 0; k < collide; k++)
				b[n].v = b[n].v * wallCollisionLoss;
		}
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
		bool collide = false;

		for (int k = n + 1; k < size; k++)
		{
			//distance vector between the balls
			vec2 d = b[n].pos - b[k].pos;

			//if distance less than 2radius = collision
			if (d.len() < RADIUS * 2) 
			{
				collide = true;

				if (LastCollisionIgnore)
				{
					if (b[n].lastCollide == &b[k])
						continue;

					b[n].lastCollide = &b[k];
				}
					
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

				//speed differance
				float diff = (va0 - va1).len();
				diff += (vb0 - vb1).len();
				diff *= 0.5f;

				audio.play(2, 1000, diff, in->Space);
			}		
		}

		if (collide == false)
			b[n].lastCollide = 0;
	}
}

void Table::fire(vec2 pos, float _charge)
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

	if (closestDist > RADIUS * 6)
		return;

	audio.play(1, 3, *charge, in->Space);

	vec2 d = pos - b[closestID].pos;
	d.inverse();

	b[closestID].push(_charge * firePow, d);
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

	for (int n = 0; n < 10; n++)
		target.draw(aim[n]);
}

void Table::setAimline()
{
	//mouse pos
	aim[0].setPosition(in->mouse.x - aimsize * 2, in->mouse.y - aimsize * 2);

	//Find the closest ball
	int closestID = -1;
	float closestDist = 2000;
	int size = b.size();
	vec2 d;
	for (int n = 0; n < size; n++)
	{
		d = in->mouse - b[n].pos;
		if (d.len() < closestDist)
		{
			closestDist = d.len();
			closestID = n;
		}
	}

	//if none found
	if (closestID == -1 || closestDist > RADIUS * 6)
	{
		//Hide the aim dots
		for (int n = 1; n < 10; n++)
			aim[n].setPosition(- 100, -100);
		return;
	}

	d = in->mouse - b[closestID].pos;
	vec2 posOriginate = b[closestID].pos;
	d.normalize();
	d.inverse();
	
	for (int n = 1; n < 10; n++)
	{
		posOriginate = posOriginate + d * RADIUS * 2;
		aim[n].setPosition(posOriginate.x - aimsize, posOriginate.y - aimsize);
	}

	int redCharge = (*charge / 3.0f) * 10;
	for (int n = 1; n < 10; n++)
	{
		if (redCharge - n > 0)
		{
			aim[n].setColor(sf::Color(255, 255, 255));
			aim[n].setScale(2, 2);
			aim[n].move(-aimsize, -aimsize);
		}
		else
		{
			aim[n].setColor(sf::Color(0, 0, 0));
			aim[n].setScale(1.0f, 1.0f);
		}
	}
}

void Table::spawnWhite()
{
	//1145 436
	if (in->enter)
	{
		int size = b.size();

		bool spawn = false;
		if (size > 0)
		{
			//if first an last balls are not white 
			if (b[size - 1].colortype != -1 && b[0].colortype != -1)
				spawn = true;
		}
		else
			spawn = true;

		if (spawn)
		{
			b.push_back(Ball());
			b[size].init(vec2(1145, 436), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);
		}
	}
}

void Table::loadSenario(int id)
{
	b.clear();

	//default values
	run = false;
	LastCollisionIgnore = true;
	rw->setVerticalSyncEnabled(false);

	switch(id)
	{
	case 1: // 8 Ball

		for (int n = 0; n < 16; n++)
			b.push_back(Ball());

		//white
		b[0].init(vec2(TABLE_EDGE * 5, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);

		//1st row
		b[1].init(vec2(1244 - RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);
		//2nd row
		b[2].init(vec2(1244 - RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 2);
		b[3].init(vec2(1244 - RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 10);
		//3rd row
		b[4].init(vec2(1244, 433 - RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 11);
		b[5].init(vec2(1244, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 8);
		b[6].init(vec2(1244, 433 + RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 3);
		//4th row
		b[7].init(vec2(1244 + RADIUS * 2.1, 433 - RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 4);
		b[8].init(vec2(1244 + RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 13);
		b[9].init(vec2(1244 + RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 5);
		b[10].init(vec2(1244 + RADIUS * 2.1, 433 + RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 12);
		//5th row
		b[11].init(vec2(1244 + RADIUS * 4.1, 433 - RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 14);
		b[12].init(vec2(1244 + RADIUS * 4.1, 433 - RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 7);
		b[13].init(vec2(1244 + RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 9);
		b[14].init(vec2(1244 + RADIUS * 4.1, 433 + RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 15);
		b[15].init(vec2(1244 + RADIUS * 4.1, 433 + RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 6);
		break;

	case 2: // 9 ball

		for (int n = 0; n < 10; n++)
			b.push_back(Ball());

		//white
		b[0].init(vec2(TABLE_EDGE * 5, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);

		//1st row
		b[1].init(vec2(1244 - RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);
		//2nd row
		b[2].init(vec2(1244 - RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 2);
		b[3].init(vec2(1244 - RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 3);
		//3rd row
		b[4].init(vec2(1244, 433 - RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 4);
		b[5].init(vec2(1244, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 9);
		b[6].init(vec2(1244, 433 + RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 5);
		//4th row		
		b[7].init(vec2(1244 + RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 6);
		b[8].init(vec2(1244 + RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 7);
		//5th row	
		b[9].init(vec2(1244 + RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 8);
		break;

	case 3: //normal side collision

		for (int n = 0; n < 2; n++)
			b.push_back(Ball());

		b[0].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);
		b[1].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT + TABLE_EDGE / 2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);

		b[0].push(800, vec2(1.0f, 0.0f));
		b[0].e = 1;
		b[1].push(1200, vec2(1.4f, 1.0f));
		b[1].e = 1;

		break;
	case 4: // Oelsastisk

		run = false;
		LastCollisionIgnore = true;
		rw->setVerticalSyncEnabled(false);
		for (int n = 0; n < 2; n++)
			b.push_back(Ball());

		b[0].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);
		b[1].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT + TABLE_EDGE / 2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);

		b[0].push(800, vec2(1, 0));
		b[0].e = 0;
		b[1].push(1200, vec2(1.4, 1));
		b[1].e = 0;
		
		break;
	case 5: // mass 5

		run = false;
		LastCollisionIgnore = true;
		rw->setVerticalSyncEnabled(false);
		for (int n = 0; n < 2; n++)
			b.push_back(Ball());

		b[0].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);
		b[1].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT + TABLE_EDGE / 2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);

		b[0].mass = 5;
		b[0].push(4000, vec2(1, 0));
		b[1].push(1200, vec2(1.4, 1));

		break;
	case 6: // mas 0.5

		run = false;
		LastCollisionIgnore = true;
		rw->setVerticalSyncEnabled(false);
		for (int n = 0; n < 2; n++)
			b.push_back(Ball());

		b[0].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);
		b[1].init(vec2(TABLE_EDGE * 2, TABLE_HEIGHT + TABLE_EDGE / 2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);

		b[0].mass = 0.5;
		b[0].push(400, vec2(1, 0));
		b[1].push(1200, vec2(1.4, 1));

		break;
	case 7:

		run = false;
		LastCollisionIgnore = true;
		rw->setVerticalSyncEnabled(false);

		for (int n = 0; n < 16; n++)
			b.push_back(Ball());

		//white
		b[0].init(vec2(TABLE_EDGE * 5, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);

		//1st row
		b[1].init(vec2(1144 - RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);
		//2nd row
		b[2].init(vec2(1144 - RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 2);
		b[3].init(vec2(1144 - RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 10);
		//3rd row
		b[4].init(vec2(1144, 433 - RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 11);
		b[5].init(vec2(1144, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 8);
		b[6].init(vec2(1144, 433 + RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 3);
		//4th row
		b[7].init(vec2(1144 + RADIUS * 2.1, 433 - RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 4);
		b[8].init(vec2(1144 + RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 13);
		b[9].init(vec2(1144 + RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 5);
		b[10].init(vec2(1144 + RADIUS * 2.1, 433 + RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 12);
		//5th row
		b[11].init(vec2(1144 + RADIUS * 4.1, 433 - RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 14);
		b[12].init(vec2(1144 + RADIUS * 4.1, 433 - RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 7);
		b[13].init(vec2(1144 + RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 9);
		b[14].init(vec2(1144 + RADIUS * 4.1, 433 + RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 15);
		b[15].init(vec2(1144 + RADIUS * 4.1, 433 + RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 6);

		b[0].mass = 100;
		b[0].push(80000, vec2(1, 0));

		break;
	case 8:

		run = false;
		LastCollisionIgnore = true;
		rw->setVerticalSyncEnabled(false);

		for (int n = 0; n < 16; n++)
			b.push_back(Ball());

		//white
		b[0].init(vec2(TABLE_EDGE * 5, TABLE_HEIGHT / 2 + TABLE_EDGE), 1, &ballTex, &ballStripeTex, &ballShadowTex, -1);

		//1st row
		b[1].init(vec2(1144 - RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 1);
		//2nd row
		b[2].init(vec2(1144 - RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 2);
		b[3].init(vec2(1144 - RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 10);
		//3rd row
		b[4].init(vec2(1144, 433 - RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 11);
		b[5].init(vec2(1144, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 8);
		b[6].init(vec2(1144, 433 + RADIUS * 2.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 3);
		//4th row
		b[7].init(vec2(1144 + RADIUS * 2.1, 433 - RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 4);
		b[8].init(vec2(1144 + RADIUS * 2.1, 433 - RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 13);
		b[9].init(vec2(1144 + RADIUS * 2.1, 433 + RADIUS * 1.1), 1, &ballTex, &ballStripeTex, &ballShadowTex, 5);
		b[10].init(vec2(1144 + RADIUS * 2.1, 433 + RADIUS * 3.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 12);
		//5th row
		b[11].init(vec2(1144 + RADIUS * 4.1, 433 - RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 14);
		b[12].init(vec2(1144 + RADIUS * 4.1, 433 - RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 7);
		b[13].init(vec2(1144 + RADIUS * 4.1, 433), 1, &ballTex, &ballStripeTex, &ballShadowTex, 9);
		b[14].init(vec2(1144 + RADIUS * 4.1, 433 + RADIUS * 2.2), 1, &ballTex, &ballStripeTex, &ballShadowTex, 15);
		b[15].init(vec2(1144 + RADIUS * 4.1, 433 + RADIUS * 4.3), 1, &ballTex, &ballStripeTex, &ballShadowTex, 6);

		b[0].mass = 100;
		b[0].push(80000, vec2(1, 0));

		for (int n = 0; n < 16; n++)
			b[n].e = 0;

		break;
	case 9:
		break;
	}
}

