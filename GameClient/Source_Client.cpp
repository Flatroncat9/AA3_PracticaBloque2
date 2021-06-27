#pragma once
#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>
#include "ClientManager.h"
#include <thread>


//client
// send hello_ nik to the server until he recieves challenge
// when recive challenge responds with challengeResponse
// waits for welcome, but if he receive again the challenge he sends the challengeRespons again untel recieve Welcome_id
// when receive Welocome_id player sets is id and wait for other players

void SendMessage(ClientManager thisClient) {
	std::string msg;

	while (true) {
		OutputMemoryBitStream oms;
		std::cout << "Escribe tu mensage\n";
		std::cin >> msg;
		oms.Write(static_cast<int>(Message_Protocol::MESSAGE), 32);
		oms.WriteString(msg, 8);
		thisClient.sock.Send(oms, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);
	}
}

int main()
{
	ClientManager thisClient;
	thisClient.SendHello();
	Port serverPort = 50000;
	std::string ipAdress = "localhost";

	std::thread t1(SendMessage, thisClient);
	t1.detach();
	//int message;
	while (true) {
		InputMemoryBitStream *input;

		Status s = thisClient.sock.Receive(input, ipAdress, serverPort);
		thisClient.ManageMessageReceived(input, ipAdress, serverPort);

	}

	PlayerInfo playerInfo;
	system("pause");

	return 0;
}

