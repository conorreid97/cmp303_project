#include "App1.h"





App1::App1() {
	
}



void App1::init() {

	// Initialise the WinSock library -- we want version 2.2.
	

	//ip = IpAddress(publicIP);
	//socket.connect(ip, 2000); // was 2000 before i changed it to host port
	//sockaddr
	
	// for clock

	// tanks
	

	

	//program(ip, socket,debugText);
}

void App1::chooseProgram() {

	//beginning of program
	
	
	// repeat if wrong input 
	//while (connectionType != 's' /*|| connectionType != 'c'*/)
	//{
	//	//cout << "Enter (s) for server/host, Enter (c) for client: " << endl;
	//	//cin >> connectionType;
	//	while (connectionType != 'c')
	//	{
	//		cout << "Enter (s) for server/host, Enter (c) for client: " << endl;
	//		cin >> connectionType;
	//	}
	//}


	
}

void App1::setUpInput(RenderWindow* window, Event event, Input input) {

}

/*void App1::windowControls(float dt, NetworkSimulator netSimulator) {
	
	if (netSimulator.Time() < 1200.0f) {
		TankMessage msg;

		//Update the network simulation
		netSimulator.Update(dt);
		//Get any 'network' messages that are available
		while (netSimulator.ReceiveMessage(msg)) {
			printf("Received message: ID= %d, Pos = (%.2f, %.2f), Time =%.2f\n", msg.id, msg.x, msg.y, msg.time);
			tanks[msg.id].AddMessage(msg);
		}

		//Update the tanks
		for (int i = 0; i < sizeof(tanks) / sizeof(Tank); i++) {
			tanks[i].Update(dt);	//Update the real position of the tank with the info from the latest packet
			if (i != netSimulator.m_MyID) {
				//Get the predicted position of the tank at the current Game Time and move the ghost to that position
				tanks[i].setGhostPosition(tanks[i].RunPrediction(netSimulator.Time()));
				if (netSimulator.Time() > nextPrint) {
					//Get the predicted position of the tank at a specific interval and print it to the console
					sf::Vector2f predictedPosition = tanks[i].RunPrediction(nextPrint);
					printf("\tPredicted positiion:  (%.2f, %.2f), Time =%.2f\n", predictedPosition.x, predictedPosition.y, nextPrint);
					nextPrint = nextPrint + (sendRate * 0.25f);	//Print 4 times per packet
				}
			}
		}
	}

}*/

void App1::update()
{
	chooseProgram();
	//socket is non blocking
	

	



	
}


