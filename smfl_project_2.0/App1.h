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
using namespace std;
using namespace sf;



class App1
{
public:
	App1();
	void StartWinSock();
	string Stringify(float value);
	void die(const char* message);
	void init();
	void update();
	void setUpInput(RenderWindow* window, Event event, Input input);
	void chooseProgram();
	void windowControls(float, NetworkSimulator);

	
	
//private:
	IpAddress ip;
	//TcpSocket socket;
	string publicIP;

	



	


	//Create two tanks (Can also accept "black" and "red")
	
	



};

