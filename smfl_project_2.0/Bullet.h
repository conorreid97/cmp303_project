#pragma once
#include <SFML\Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include "TankMessage.h"
#include <iostream>
#include "GameObject.h"
#include "Tank.h"
using namespace sf;

class Bullet : public GameObject
{
public:

	Bullet(RenderWindow* win, int tankID);
	~Bullet();

	void update(float dt, Vector2f tank_0_Pos, Vector2f tank_1_Pos);
	const void Render(RenderWindow* window);
	void Reset();

	RenderWindow* window;
private:
	Sprite m_BulletSprite;
	Texture m_BulletTexture;
	Clock timer;
	Time timePassed;

	//Tank tanks[];

	Vector2f* vectorToPlayer;
	Vector2f bulletToP1, bulletToP2;
	float p1Mag, p2Mag;
	const float scaleValue = 0.1;


};

