//#include "App1.h"
#pragma once
#pragma comment(lib, "ws2_32.lib")
#include<SFML/Network.hpp>
#include<SFML/Graphics.hpp>
#include<WinSock2.h>
#include <sstream>
#include <iomanip>
#include<iostream>
#include "Tank.h"
#include "NetworkSimulator.h"
#include "TankMessage.h"
#include "Collision.h"
#include "AITank.h"
using namespace std;
using namespace sf;


// global ipAddress and port we are sending data to
sf::IpAddress ipAddress = "127.0.0.1";
unsigned short hostPort = 4444;
unsigned short clientPort = 5555;
#define WELCOME "Hello!"
char buffer[2000];

#define MESSAGESIZE 400

// crashes the game safely-ish
void die(const char* message) {
	fprintf(stderr, "Error: %s (WSAGetLastError() = )\n", message/*, WSAGetLastError()*/);

#ifdef _DEBUG
	// Debug build -- drop the program into the debugger.
	abort();
#else
	exit(1);
#endif
}

// inits winsock
void StartWinSock() {
	// We want version 2.2.
	WSADATA w;
	int error = WSAStartup(0x0202, &w);
	if (error != 0)
	{
		die("WSAStartup failed");
	}
	if (w.wVersion != 0x0202)
	{
		die("Wrong WinSock version");
	}
	printf("Winsock initialised\n");
}

//Rounds a float to two decimal places and turns it into a string
std::string Stringify(float value) {
	std::stringstream sStream;
	sStream << std::fixed << std::setprecision(2) << value;
	std::string t;
	sStream >> t;
	return t;
}

int main() {
	/*App1 app;
	app.init();
	app.update();*/

	WSADATA w;
	int error = WSAStartup(0x0202, &w);// first kermit
	if (error != 0)
	{
		die("WSAStartup failed");
	}
	if (w.wVersion != 0x0202)
	{
		die("Wrong WinSock version");
	}


	/*ip = IpAddress::getLocalAddress();
	publicIP = ip.toString();*/
	
	char connectionType, mode, isLocal;
	int intConnType = 0;
	//these are for storing the ip address when the user has to input it
	int ip1, ip2, ip3, ip4;
	bool done = false;
	string id;

	// network variables
	UdpSocket socket;
	Packet message;
	Packet reply;
	
	
	string messageString;
	string replyString;


	cout << "Enter (s) for server/host, Enter (c) for client: " << endl;
	cin >> connectionType;
	string text = "Connected to: ";
	message << WELCOME;

	//check if its a local or online game
	if (connectionType == 'c') {
		cout << "Are you playing on same pc as host?" << endl;
		cout << "Enter (l) for local game, Enter (o) for online: " << endl;
		cin >> isLocal;
		// repeat if wrong input 
		while (isLocal != 'l') {
			while (isLocal != 'o') {
				cout << "Enter (l) for local game, Enter (o) for online: " << endl;
				cin >> isLocal;
			}
		}

		// if its an online game get the IP address 
		if (isLocal == 'o') {
			//first part
			cout << "Type type first part of IP address eg. (xxx.0.0.1)" << endl;
			cin >> ip1;
			while (ip1 < 0 || ip1 > 999) {
				cout << "Type type first part of IP address eg. (xxx.0.0.1)" << endl;
				cin >> ip1;
			}
			//second part
			cout << "Type type second part of IP address eg. (0.xxx.0.1)" << endl;
			cin >> ip2;
			while (ip2 < 0 || ip2 > 999) {
				cout << "Type type second part of IP address eg. (0.xxx.0.1)" << endl;
				cin >> ip2;
			}
			//third part
			cout << "Type type third part of IP address eg. (0.0.xxx.1)" << endl;
			cin >> ip3;
			while (ip3 < 0 || ip3 > 999) {
				cout << "Type type third part of IP address eg. (0.0.xxx.1)" << endl;
				cin >> ip3;
			}
			//fourth part
			cout << "Type type fourth part of IP address eg. (0.0.0.xxx)" << endl;
			cin >> ip4;
			while (ip4 < 0 || ip4 > 999) {
				cout << "Type type fourth part of IP address eg. (0.0.0.xxx)" << endl;
				cin >> ip4;
			}
			ipAddress = Stringify(ip1) + "." + Stringify(ip2) + "." + Stringify(ip3) + "." + Stringify(ip4);
		}


	}

	message << WELCOME;

	if (connectionType == 's')
	{
		cout << "hosting game at ip: " << ipAddress << " and your port is: " << hostPort << endl;
		// bind socket
		if (socket.bind(hostPort) != Socket::Done) {
			die("failed to bind port");
		}

		cout << "waiting on client joining....\n";
		if (socket.receive(reply, ipAddress, hostPort) != Socket::Done) {
			die("failed to send");
		}

		message >> messageString;
		reply >> replyString;

		message << WELCOME;
		if (replyString == WELCOME)
		{
			cout << "connected!\n";
			cout << replyString << endl;
			cout << "Send reply message..." << endl;

			if (socket.send(message.getData(), message.getDataSize(), ipAddress, clientPort) == Socket::Error) {
				die("failed to send");
			}
		}
		else {
			die("welcome message not correct");
		}
	}

	// clients handshake
	if (connectionType == 'c') {
		cout << "joining game at ip: " << ipAddress << " and your port is " << clientPort << endl;


		if (socket.bind(clientPort) != Socket::Done) {
			die("failed to connect to host");
		}
		cout << "sending welcome message..." << endl;
		if (socket.send(message.getData(), message.getDataSize(), ipAddress, hostPort) == Socket::Error)
		{
			die("failed to send");
		}
		// host will now send a response
		// check if we have recieved a message back every 100 ms
		std::cout << "waiting on reply...\n";
		if (socket.receive(reply, ipAddress, clientPort) != sf::Socket::Done)
		{
			die("failed to send");
		}
		message >> messageString;
		reply >> replyString;
		// check if weve recieved the correct welcome message
		if (replyString == WELCOME) {
			//print the welcome message
			cout << "connected!" << endl;
			cout << replyString << endl;
		}
	}
	// set the connection types to int values
	if (connectionType == 's') {
		intConnType = 0;
	}
	if (connectionType == 'c') {
		intConnType = 1;
	}
	// setup game
	socket.setBlocking(0);

	RenderWindow window(sf::VideoMode(640, 480), "CMP303 - Prediction");

	window.setFramerateLimit(60);	//Request 60 frames per second
	Input input;

	Tank tanks[2] = { Tank("green", 0), Tank("blue", 1) };
	sf::Texture floorTexture;
	sf::Sprite floor;

	sf::Font montserrat;
	sf::Text debugText;
	sf::Text player1HealthText;
	sf::Text player2HealthText;
	sf::Text p1WinText;
	sf::Text p2WinText;
	montserrat.loadFromFile("Assets/Montserrat-Regular.ttf");
	debugText.setFont(montserrat);
	debugText.setOutlineColor(sf::Color::Black);
	debugText.setOutlineThickness(1.f);

	player1HealthText.setFont(montserrat);
	player1HealthText.setOutlineColor(sf::Color::Blue);
	player1HealthText.setOutlineThickness(1.f);
	player1HealthText.setPosition(window.getSize().x / 100, window.getSize().y / 1.1);// was window.getSize().x,window.getSize().y

	player2HealthText.setFont(montserrat);
	player2HealthText.setOutlineColor(sf::Color::Black);
	player2HealthText.setOutlineThickness(1.f);
	player2HealthText.setPosition(window.getSize().x / 1.75, window.getSize().y / 1.1);

	p1WinText.setString("Player 1 WINS!\nHost, press R to reset");
	p1WinText.setScale(1.5f, 1.5f);
	p1WinText.setFont(montserrat);
	p1WinText.setOutlineColor(sf::Color::Yellow);
	p1WinText.setOutlineThickness(1.f);
	p1WinText.setPosition(window.getSize().x / 10.0, window.getSize().y / 2.5);

	p2WinText.setString("Player 2 WINS!\nHost, press R to reset");
	p2WinText.setScale(1.5f, 1.5f);
	p2WinText.setFont(montserrat);
	p2WinText.setOutlineColor(sf::Color::Yellow);
	p2WinText.setOutlineThickness(1.f);
	p2WinText.setPosition(window.getSize().x / 10.0, window.getSize().y / 2.5);

	AITank aiTank(&window, 0);
	aiTank.setInput(&input);
	aiTank.setCollisionBox(25, 0, 30, 30);
	aiTank.setCollider(true);

	tanks[0].setPosition(64, 256);
	tanks[0].SetRenderMode(Tank::RenderMode::REAL_AND_PREDICTED);
	tanks[0].setInput(&input);
	tanks[0].setWindow(&window);
	tanks[0].setCollisionBox(25, 0, 30, 30);
	tanks[0].setCollider(true);
	tanks[0].setIsLocal(intConnType);

	tanks[1].setPosition(416, 128);
	tanks[1].SetRenderMode(Tank::RenderMode::REAL_AND_PREDICTED);
	tanks[1].setInput(&input);
	tanks[1].setWindow(&window);
	tanks[1].setCollisionBox(25, 0, 30, 30);
	tanks[1].setCollider(true);
	tanks[1].setIsLocal(intConnType);

	// label the players locally whether or not they're the actual player
	if (tanks[0].getTankID() == intConnType)
		tanks[0].isPlayer = 1;
	else
		tanks[1].isPlayer = 1;

	//Initialise the background texture and sprite
	floorTexture.loadFromFile("Assets/tileSand1.png");
	floorTexture.setRepeated(true);
	floor.setTexture(floorTexture);
	floor.setTextureRect(sf::IntRect(0, 0, 640, 480));

	//Initialise font and text
	montserrat.loadFromFile("Assets/Montserrat-Regular.ttf");
	debugText.setFont(montserrat);
	debugText.setOutlineColor(sf::Color::Black);
	debugText.setOutlineThickness(1.f);

	//Clock for timing the 'dt' value
	sf::Clock clock;
	Clock gameClock;
	Time gameTime;
	// init variables for the networking part
// used to check if the host locally wants to reset
	float nextPrint;
	float sendRate = 0.1f;
	float latency = 0.3f;
	float gameSpeed = 1.0f;
	float startTime = sendRate * 3.0f;

	String tankReply;
	Packet tankMsgResponse;
	bool wantResetLocal = false;

	nextPrint = startTime;

	//Create a network simulator with that "sends" a message every 0.5 seconds and has a latency of 0.1 seconds
	NetworkSimulator netSimulator(sendRate, latency);
	//netSimulator.m_MyID = 0;	//On the network, we are Tank 0

	//When are we next printing the predicted position (so we don't spam the console)

	gameClock.restart();

	// set prediction for each player
	if (tanks[0].isPlayer)
		netSimulator.m_MyID = 1;
	else
		netSimulator.m_MyID = 0;

	while (window.isOpen()) {
		
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
			case Event::Closed:
				cout << "Game Closed\n";

				socket.unbind();

				window.close();
				break;
			case Event::Resized:
				window.setView(View(FloatRect(0.0f, 0.0f, (float)event.size.width, (float)event.size.height)));
				break;
			case Event::KeyPressed:
				input.setKeyDown(event.key.code);
				break;
			case Event::KeyReleased:
				input.setKeyUp(event.key.code);
				break;
			case Event::MouseMoved:
				input.setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Left) {
					input.setMouseLDown(true);
				}
				else if (event.mouseButton.button == Mouse::Right) {
					input.setMouseRDown(true);
				}
				break;
			case Event::MouseButtonReleased:
				if (event.mouseButton.button == Mouse::Left) {
					input.setMouseLDown(false);
				}
				else if (event.mouseButton.button == Mouse::Right) {
					input.setMouseRDown(false);
				}
				break;
			default:
				// dont handle anymore events
				break;
			}
		}

		float dt = clock.restart().asSeconds() * gameSpeed;

		//close game if esc is presed
		if (input.isKeyDown(Keyboard::Escape)) {
			socket.unbind();
			cout << "Game Closed\n";
			window.close();
		}

		//If we're at the start, just advance the time by 3.5 seconds, so we have a few packets in the queue already
		if (netSimulator.Time() < 1.0f) {
			printf("BEGIN SIMULATION\n");
			netSimulator.Update(dt);
		}

		gameTime = gameClock.getElapsedTime();

		Packet tankMsgPacket;
		TankMessage tankMsg;

		if (tanks[0].isPlayer)
			tankMsg.id = 0;
		else
			tankMsg.id = 1;

		tankMsg.time = gameTime.asSeconds();
		tankMsg.x = tanks[tankMsg.id].getPosition().x;
		tankMsg.y = tanks[tankMsg.id].getPosition().y;
		tankMsg.wantReset = 0;

		// reset
		if (input.isKeyDown(Keyboard::R)) {
			if (tanks[0].isPlayer && tanks[0].getTankID() == 1) {
				tankMsg.wantReset = 1;
				wantResetLocal = 1;
			}
			else if (tanks[1].isPlayer && tanks[1].getTankID() == 1) {
				tankMsg.wantReset = 1;
				wantResetLocal = 1;
			}
			input.setKeyUp(Keyboard::R);
		}

		//load the player message into the packet
		tankMsgPacket << tankMsg.id << tankMsg.x << tankMsg.y << tankMsg.time << tankMsg.wantReset;

		//send tank message
		if (tanks[0].isPlayer) {
			if (socket.send(tankMsgPacket.getData(), tankMsgPacket.getDataSize(), ipAddress, clientPort) != Socket::Done) {
				die("failed to send tank message packet");
			}
		}
		else {
			if (socket.send(tankMsgPacket.getData(), tankMsgPacket.getDataSize(), ipAddress, hostPort) != Socket::Done) {
				die("failed to send tank message packet");
			}
		}
		// recieve tank message
		//store error info
		int error = 0;
		do {
			if (tanks[0].isPlayer) {
				if (socket.receive(tankMsgResponse, ipAddress, hostPort) != Socket::Done) {
					error = WSAGetLastError();
					if (error == WSAEWOULDBLOCK) {
						// try to recieve the message again
					}
					else {
						die("failed to recieve");
					}
				}
			}
			else {
				if (socket.receive(tankMsgResponse, ipAddress, clientPort) != Socket::Done) {
					error = WSAGetLastError();
					if (error == WSAEWOULDBLOCK) {
						// try to recieve again
					}
					else {
						die("failed to recieve");
					}
				}
			}
		} while (tankMsgResponse.getDataSize() < 1);

		// unpack the response
		tankMsgResponse >> tankMsg.id >> tankMsg.x >> tankMsg.y >> tankMsg.time >> tankMsg.wantReset;

		// everything in this will stop if one of the players die
		if (!tanks[0].isDead && !tanks[1].isDead) {

			//Update the tanks
			for (int i = 0; i < sizeof(tanks) / sizeof(Tank); i++) {
				tanks[i].Update(dt);	//Update the real position of the tank with the info from the latest packet


				if (!tanks[i].isPlayer) {
					tanks[i].AddMessage(tankMsg);
					//Get the predicted position of the tank at the current Game Time and move the ghost to that position
					tanks[i].setGhostPosition(tanks[i].RunPrediction(gameTime.asSeconds()));
					if (gameTime.asSeconds() > nextPrint) {
						//Get the predicted position of the tank at a specific interval and print it to the console
						sf::Vector2f predictedPosition = tanks[i].RunPrediction(nextPrint);
						printf("\tPredicted positiion:  (%.2f, %.2f), Time =%.2f\n", predictedPosition.x, predictedPosition.y, nextPrint);
						nextPrint = nextPrint + (sendRate * 0.25f);	//Print 4 times per packet
					}
				}
			}

			aiTank.update(dt, tanks[0].getPosition(), tanks[1].getPosition());

			// collision responses
			if (Collision::checkBoundingBox(&tanks[0], &aiTank)) {
				// handle collision
				//std::cout << "AAA\n";
				tanks[0].health -= 4.0;
			}
			if (Collision::checkBoundingBox(&tanks[1], &aiTank)) {
				// handle collision
				//std::cout << "BBB\n";
				tanks[1].health -= 4.0;
			}

			// if players collide they both take damage
			if (Collision::checkBoundingBox(&tanks[0], &tanks[1])) {
				// handle collision
				tanks[1].health -= 1.0;
			}
			if (Collision::checkBoundingBox(&tanks[1], &tanks[0])) {
				// handle collision
				tanks[0].health -= 1.0;
				
			}
		}
		// check if the players want to reset the game
		if (wantResetLocal || tankMsg.wantReset) {
			tanks[0].Reset(); tanks[1].Reset();
			aiTank.Reset();
			printf("\n\n--------RESET--------\n\n");
			wantResetLocal = 0;
		}

		debugText.setString("Game Time: " + Stringify(gameTime.asSeconds()));
		player1HealthText.setString("P1 Health: " + Stringify(tanks[0].health));
		player2HealthText.setString("P2 Health: " + Stringify(tanks[1].health));
		//Render the scene
		window.clear();

		window.draw(floor);
		for (auto& tank : tanks) {
			tank.Render(&window);
		}
		aiTank.Render(&window);
		window.draw(debugText);
		//displays players health
		window.draw(player1HealthText);
		window.draw(player2HealthText);

		// display win text
		if (tanks[0].isDead) {
			window.draw(p1WinText);
		}
		else if (tanks[1].isDead) {
			window.draw(p2WinText);
		}

		window.display();
	}
	socket.unbind();
	return 0;
}