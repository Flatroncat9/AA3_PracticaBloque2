#pragma once
#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>
#include "ClientManager.h"


//client
// send hello_ nik to the server until he recieves challenge
// when recive challenge responds with challengeResponse
// waits for welcome, but if he receive again the challenge he sends the challengeRespons again untel recieve Welcome_id
// when receive Welocome_id player sets is id and wait for other players


int main()
{
	UDPSocket clientSock;
	ClientManager thisClient;
	thisClient.SendHello();
	//int message;
	while (true) {
		OutputMemoryBitStream output;
		std::string msg = "";

		std::cout << "Escribe un mensaje";
		std::cin >> msg;
		output.WriteString(msg, 8);

		// OutputStream, ip, port
		clientSock.Send(output, "localhost", 50000);
	}

	PlayerInfo playerInfo;
	system("pause");

	return 0;
}