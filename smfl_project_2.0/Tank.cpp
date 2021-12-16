#include "Tank.h"

//Rounds a float to two decimal places and turns it into a string
std::string Tank::Stringify(int value) {
	std::stringstream sStream;
	sStream << std::fixed << std::setprecision(2) << value;
	std::string t;
	sStream >> t;
	return t;
}

Tank::Tank(std::string color, int tankID_) : GameObject/* sf::Sprite*/()
{
	std::string playerNumString = Stringify(tankID_);// this is needed as loadFromFile only takes std::string
	m_BodyTexture.loadFromFile("Assets/" + color + "Tank.png");
	m_BarrelTexture.loadFromFile("Assets/" + color + "Barrel.png");

	this->setTexture(&m_BodyTexture);
	sprite.setTexture(m_BodyTexture);
	sprite.setOrigin(6, 2);
	sprite.setPosition(getPosition());
	setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);

	m_BodySprite.setTexture(m_BodyTexture);
	m_BodySprite.setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	m_BodySprite.setPosition(getPosition());

	//setTexture(&m_BodyTexture);
	m_BarrelSprite.setTexture(m_BarrelTexture);
	m_BarrelSprite.setOrigin(6, 2);
	m_BarrelSprite.setPosition(getPosition());


	m_GhostSprite.setTexture(m_BodyTexture);
	m_GhostSprite.setColor(sf::Color(255, 255, 255, 128));
	m_GhostSprite.setOrigin(getTextureRect().width / 2, getTextureRect().height / 2);
	setGhostPosition(getPosition());


	tankID = tankID_;
	isLocal = 1;
	isPlayer = 0;
	velocity = { 0,0 };
	health = 100;
	isDead = 0;

}

//Tank::Tank() {
//
//}

Tank::~Tank()
{
}

//Sets the tank's position to the latest network position
void Tank::Update(float dt)
{
	if (health <= 0) {
		isDead = 1;
		cout << "Player " << tankID << " has died" << endl;
	}

	//only does this on the other player not the local player
	if (!isPlayer) {
		if (m_Messages.size() < 1)
			return;
		if (m_PredictedPos.size() > 3) {
			Vector2f interpolatedPos;
			for (int i = 0; i < m_PredictedPos.size(); i++) {
				if (m_PredictedPos[i].time != m_PredictedPos.front().time) {
					if (m_PredictedPos[i].time > m_PredictedPos[i - 1].time) {
						auto temp = m_PredictedPos[i];
						m_PredictedPos[i] = m_PredictedPos[i - 1];
						m_PredictedPos[i - 1] = temp;
					}
				}
			}
			// gets previous messages and predicted positions to interpolate
			const TankMessage& msg1 = m_Messages[m_Messages.size() - 1];
			const TankMessage& msg2 = m_Messages[m_Messages.size() - 2];
			const TankMessage& msg3 = m_Messages[m_Messages.size() - 3];
			const TankMessage& msg4 = m_PredictedPos[m_PredictedPos.size() - 1];
			const TankMessage& msg5 = m_PredictedPos[m_PredictedPos.size() - 2];
			const TankMessage& msg6 = m_PredictedPos[m_PredictedPos.size() - 3];

			interpolatedPos.x = cosineInterpolate(msg1.x, msg4.x, 0.75);
			interpolatedPos.y = cosineInterpolate(msg1.y, msg4.y, 0.75);

			setPosition(interpolatedPos.x, interpolatedPos.y);
		}
		else {
			TankMessage latestMessage = m_Messages.back();
			setPosition(latestMessage.x, latestMessage.y);
		}
	}

	if (isPlayer) {
		if (isLocal == 0 && getTankID() == 0 || isLocal == 1) {
			if (input->isKeyDown(Keyboard::W)) {
				velocity.y = -150 * dt;
			}
			if (input->isKeyDown(sf::Keyboard::S)) {// moves the player down
				velocity.y = 150 * dt;
				//velocity.x = 0;
			}
			if (input->isKeyDown(sf::Keyboard::A)) {// moves the player left
				velocity.x = -150 * dt;
				//velocity.y = 0;
			}
			if (input->isKeyDown(sf::Keyboard::D)) {// moves the player right
				velocity.x = 150 * dt;
				//velocity.y = 0;
			}
		}
		else if (isLocal == 0 && getTankID() == 1) {
			if (input->isKeyDown(sf::Keyboard::Up)) {// moves the player up
				velocity.y = -150 * dt;
				//velocity.x = 0;
			}
			if (input->isKeyDown(sf::Keyboard::Down)) {// moves the player down
				velocity.y = 150 * dt;
				//velocity.x = 0;
			}
			if (input->isKeyDown(sf::Keyboard::Left)) {// moves the player left
				velocity.x = -150 * dt;
				//velocity.y = 0;
			}
			if (input->isKeyDown(sf::Keyboard::Right)) {// moves the player right
				velocity.x = 150 * dt;
				//velocity.y = 0;
			}
		}
	}

	// gradual slowdown
	velocity.x *= 50.0f * dt;
	velocity.y *= 50.0f * dt;

	// move along velocity
	m_BodySprite.move(velocity);
	this->move(velocity);

	//making sure the player doesn't escape the screen by comparing where they are to the edges of the screen
	if (getPosition().x < window->getSize().x * 0.01) {
		setPosition(getPosition().x + 10, getPosition().y);
		velocity.x = -velocity.x;
	}
	if (getPosition().x > window->getSize().x) {
		setPosition(getPosition().x - 10, getPosition().y);
		velocity.x = -velocity.x;
	}
	if (getPosition().y < window->getSize().y * 0.01) {
		setPosition(getPosition().x, getPosition().y + 10);
		velocity.y = -velocity.y;
	}
	if (getPosition().y > window->getSize().y) {
		setPosition(getPosition().x, getPosition().y - 10);
		velocity.y = -velocity.y;
	}
}

void Tank::setWindow(sf::RenderWindow* win) {
	window = win;
}


void Tank::setPosition(float x, float y) {
	RectangleShape::setPosition(x, y);
	m_BodySprite.setPosition(x, y);
}

//Use this to set the prediction position
void Tank::setGhostPosition(sf::Vector2f pos) {
	m_GhostSprite.setPosition(pos);
}

//Draw the tank / or the ghost / or both
const void Tank::Render(sf::RenderWindow* window) {
	if (m_RenderMode > 0)
		window->draw(m_GhostSprite);
	if (m_RenderMode != 1) {
		window->draw(m_BodySprite);
	}
}

//Add a message to the tank's network message queue
void Tank::AddMessage(const TankMessage& msg) {
	m_Messages.push_back(msg);
}

//This method calculates and stores the position, but also returns it immediately for use in the main loop
//This is my where prediction would be... IF I HAD ANY
sf::Vector2f Tank::RunPrediction(float gameTime) {
	float predictedX = -1.0f;
	float predictedY = -1.0f;

	Vector2f speed;
	Vector2f displacement;

	const auto msize = m_Messages.size();
	const auto msizePred = m_PredictedPos.size();
	if (msize < 3) {
		return sf::Vector2f(predictedX, predictedX);
	}
	const TankMessage& msg0 = m_Messages[msize - 1];
	const TankMessage& msg1 = m_Messages[msize - 2];
	const TankMessage& msg2 = m_Messages[msize - 3];

//quadratic method 2.0
	///Quadratic Model
	//S = ut + � at^2
	//Next position = previous pos + displacement
	//displacement = speed (at last message) * time (since last message) + 0.5 * acceleration * time^2
	//acceleration = difference in speed at the last two positions / time between those positions
	//speed = distance between last two positions / time between last two positions

	//get the speed
	//get distance between last 2 pos
	float dispX = msg0.x - msg1.x;
	float dispY = msg0.y - msg1.y;
	//time between last 2 pos
	float time = msg0.time - msg1.time;
	//Now get speed
	float speedX = dispX / time;
	float speedY = dispY / time;

	//get acceleration
	//get second speed value
	float dispX2 = sqrt(pow(msg1.x - msg2.x, 2) + pow(msg1.y - msg2.y, 2));
	float dispY2 = sqrt(pow(msg1.x - msg2.x, 2) + pow(msg1.y - msg2.y, 2));
	float time2 = msg1.time - msg2.time;
	float speedX2 = dispX2 / time2;
	float speedY2 = dispY2 / time2;
	//difference in speed
	float speedDiffX = speedX - speedX2;
	float speedDiffY = speedY - speedY2;
	//calc acceleration
	float accelX = speedDiffX / time;
	float accelY = speedDiffY / time;

	//get the displacement
	//time since last message
	float timePassed = gameTime - msg0.time;
	//Now do S = ut + � at^2
	float displacementX = speedX * timePassed + 0.5 * accelX * pow(timePassed, 2);
	float displacementY = speedY * timePassed + 0.5 * accelY * pow(timePassed, 2);

	//Finally, do our prediction
	predictedX = msg0.x + displacementX;
	predictedY = msg0.y + displacementY;

	//updated pridicted postion for interpolation
	TankMessage predictedPos_;
	predictedPos_.x = predictedX;
	predictedPos_.y = predictedY;
	m_PredictedPos.push_back(predictedPos_);

	sf::Vector2f nextPos = sf::Vector2f(predictedX, predictedY);
	return nextPos;
}

float Tank::calculateDistance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
// takes in the three points and a double that represents how close to the prediction it should be
//https://stackoverflow.com/questions/27447563/c-interpolate-array-with-cosine
double Tank::cosineInterpolate(double y1, double y2, double mu) {
	double mu2;

	mu2 = (1 - cos(mu * pi)) / 2;
	return(y1 * (1 - mu2) + y2 * mu2);
}

// takes in the three points and a double that represents how close to the prediction it should be
//http://paulbourke.net/miscellaneous/interpolation/
double Tank::cubicInterpolate(double y0, double y1, double y2, double y3, double mu) {
	double a0, a1, a2, a3, mu2;
	double result;
	mu2 = mu * mu;// bias squared
	// quick maff
	a0 = y3 - y2 - y0 + y1;
	a1 = y0 - y1 - a0;
	a2 = y2 - y0;
	a3 = y1;
	result = (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
	return result;
}

void Tank::Reset() {

	//m_Messages.clear();

	if (tankID == 0)
		setPosition(64, 256);
	else
		setPosition(416, 128);
	health = 100;
	isDead = 0;
}

void Tank::setTankID(int tankID_) {
	tankID = tankID_;
}

int Tank::getTankID() {
	return tankID;
}

