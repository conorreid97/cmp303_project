#include "AITank.h"

AITank::AITank(RenderWindow* win, int tankID) {
	window = win;
	m_TankTexture.loadFromFile("Assets/redTank.png");
	this->setTexture(&m_TankTexture);

	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	setPosition(window->getSize().x / 2, window->getSize().y / 2);
	m_TankSprite.setTexture(m_TankTexture);
	m_TankSprite.setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	m_TankSprite.setPosition(window->getSize().x / 2, window->getSize().y / 2);

	timer.restart();
	vectorToPlayer = NULL;
	velocity = { 0,0 };
	p1Mag, p2Mag = 0;

}

AITank::~AITank() {
	vectorToPlayer = NULL;
	delete vectorToPlayer;
}

void AITank::update(float dt, Vector2f tank_0_Pos, Vector2f tank_1_Pos) {

	// create vectors
	tankToP1 = tank_0_Pos - m_TankSprite.getPosition();
	tankToP2 = tank_1_Pos - m_TankSprite.getPosition();
	// calculate magnitude
	p1Mag = pow(tankToP1.x, 2) + pow(tankToP1.y, 2);
	p1Mag = sqrt(p1Mag);
	p2Mag = pow(tankToP2.x, 2) + pow(tankToP2.y, 2);
	p2Mag = sqrt(p2Mag);

	if (p1Mag > p2Mag)
		vectorToPlayer = &tankToP2;
	else
		vectorToPlayer = &tankToP1;

	timePassed = timer.getElapsedTime();

	velocity = *vectorToPlayer * (timePassed.asSeconds() * scaleValue * dt);

	this->move(velocity);
	m_TankSprite.move(velocity);
}

const void AITank::Render(RenderWindow* window) {
	window->draw(m_TankSprite);
}

void AITank::Reset() {
	m_TankSprite.setPosition(window->getSize().x / 2, window->getSize().y / 2);
	setPosition(window->getSize().x / 2, window->getSize().y / 2);
	timer.restart();
}
