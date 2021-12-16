#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<conio.h>
#include<vector>

using namespace std;
using namespace sf;

int main()
{
	IpAddress ip = IpAddress(ipAddress);
	string publicIP = ip.toString();
	TcpSocket socket;
	bool done = false;
	string id;
	string text = "";
	cout << "Enter online id: ";
	cin >> id;

	ip = IpAddress(publicIP);

	socket.connect(ip, 2000);

	RenderWindow window(VideoMode(800, 600, 32), "Chat");
	vector<Text> chat;

	Packet packet;
	packet << id;

}