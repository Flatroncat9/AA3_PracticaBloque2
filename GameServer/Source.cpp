#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <UDPSocket.h>

#define PORT 50000

int main()
{
	
	UDPSocket serverSock;
	serverSock.Bind(PORT);

	InputMemoryStream* input;
	std::string ip;
	Port p;

	serverSock.Receive(input, ip, p);

	int i;
	input->Read(&i);

	std::cout << ip << "_" << p << "  " << i << "\n";

	PlayerInfo playerInfo;

	system("pause");
	return 0;
}