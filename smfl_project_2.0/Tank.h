#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "TankMessage.h"
#include "GameObject.h"

using namespace std;
using namespace sf;

class Tank : public GameObject/*, public sf::Sprite*/
{
public:
	Tank();
	Tank(std::string color, int tankID_);
	~Tank();

	enum RenderMode {
		REAL_ONLY,
		PREDICTED_ONLY,
		REAL_AND_PREDICTED
	};

	void Update(float dt);
	const void Render(sf::RenderWindow* window);

	void AddMessage(const TankMessage& msg);
	sf::Vector2f RunPrediction(float gameTime);
	std::string Stringify(int value);
	void SetRenderMode(const RenderMode renderMode) { m_RenderMode = renderMode; }
	void setPosition(float x, float y);
	void setGhostPosition(sf::Vector2f pos);
	void setTankID(int tankID_);
	void setIsLocal(int isLocal_) { isLocal = isLocal_; }
	int getTankID();
	void Reset();

	float calculateDistance(float x1, float y1, float x2, float y2);
	double cosineInterpolate(double y1, double y2, double mu);
	double cubicInterpolate(double y0, double y1, double y2, double y3, double mu);
	void setWindow(sf::RenderWindow* win);
	bool isPlayer;
	float health;
	bool isDead;
	//App1 app;


private:
	sf::Sprite	m_BarrelSprite;
	sf::Sprite	m_GhostSprite;
	sf::Sprite m_BodySprite;
	sf::Sprite sprite;

	RenderWindow* window;
	sf::Texture m_BodyTexture;
	sf::Texture m_BarrelTexture;
	float		m_BodyRotation;
	float		m_BarrelRotation;
	const float pi = 3.14;
	int tankID;
	int isLocal;
	RenderMode	m_RenderMode = RenderMode::REAL_AND_PREDICTED;

	std::vector<TankMessage> m_Messages;
	std::vector<TankMessage> m_PredictedPos;
};

