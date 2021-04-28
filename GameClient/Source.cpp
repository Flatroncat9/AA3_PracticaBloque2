#pragma once
#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>



int main()
{
	UDPSocket clientSock;
	//int message;
	//std::cin >> message;

	OutputMemoryStream output;
	output.Write(5);

	// OutputStream, ip, port
	clientSock.Send(output, "localhost", 50000);

	PlayerInfo playerInfo;
	system("pause");
	return 0;
}