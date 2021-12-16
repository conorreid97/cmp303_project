#include "Bullet.h"

Bullet::Bullet(RenderWindow* win, int tankID) {
	window = win;
	m_BulletTexture.loadFromFile("Assets/redTank.png");
	this->setTexture(&m_BulletTexture);

	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	setPosition(window->getSize().x / 2, window->getSize().y / 2);
	m_BulletSprite.setTexture(m_BulletTexture);
	m_BulletSprite.setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	m_BulletSprite.setPosition(window->getSize().x / 2, window->getSize().y / 2);

	timer.restart();
	vectorToPlayer = NULL;
	velocity = { 0,0 };
	p1Mag, p2Mag = 0;

}

Bullet::~Bullet() {
	vectorToPlayer = NULL;
	delete vectorToPlayer;
}

void Bullet::update(float dt, Vector2f tank_0_Pos, Vector2f tank_1_Pos) {

	// create vectors
	bulletToP1 = tank_0_Pos - m_BulletSprite.getPosition();
	bulletToP2 = tank_1_Pos - m_BulletSprite.getPosition();
	// calculate magnitude
	p1Mag = pow(bulletToP1.x, 2) + pow(bulletToP1.y, 2);
	p1Mag = sqrt(p1Mag);
	p2Mag = pow(bulletToP2.x, 2) + pow(bulletToP2.y, 2);
	p2Mag = sqrt(p2Mag);

	if (p1Mag > p2Mag)
		vectorToPlayer = &bulletToP2;
	else
		vectorToPlayer = &bulletToP1;

	timePassed = timer.getElapsedTime();

	velocity = *vectorToPlayer * (timePassed.asSeconds() * scaleValue * dt);

	this->move(velocity);
	m_BulletSprite.move(velocity);
}

const void Bullet::Render(RenderWindow* window) {
	window->draw(m_BulletSprite);
}

void Bullet::Reset() {
	m_BulletSprite.setPosition(window->getSize().x / 2, window->getSize().y / 2);
	setPosition(window->getSize().x / 2, window->getSize().y / 2);
	timer.restart();
}