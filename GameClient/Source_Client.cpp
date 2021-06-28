#pragma once
#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>
#include "ClientManager.h"
#include <thread>

#define COUNTDOWN_CONNECT 30

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

		if (msg == "exit" || msg == "EXIT") {
			oms.Write(static_cast<int>(Message_Protocol::END), 32);
			oms.Write(thisClient.serverInfo.ClientSalt, 32);
			oms.Write(thisClient.serverInfo.ServerSalt, 32);
			thisClient.sock.Send(oms, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);
			thisClient.onLoop = false;
			break;
		}
		oms.Write(static_cast<int>(Message_Protocol::MESSAGE), 32);
		oms.WriteString(msg, 8);
		thisClient.sock.Send(oms, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);
	}
}

int main()
{
	ClientManager thisClient;
	auto initProgram = std::chrono::system_clock::now();
	auto lastMessageSent = std::chrono::system_clock::now();
	// Until not verified and received Welcome, sends HELLO every 0.2s
	while(!thisClient.loggedIn) {
		std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - lastMessageSent;
		if (elapsed_seconds.count() > 0.2) {
			thisClient.SendHello();
			lastMessageSent = std::chrono::system_clock::now();
		}
		InputMemoryBitStream* input;

		Status s = thisClient.sock.Receive(input, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);
		if (s == Status::Done)
			thisClient.ManageMessageReceived(input, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);
		elapsed_seconds = std::chrono::system_clock::now() - initProgram;
		if (elapsed_seconds.count() > COUNTDOWN_CONNECT) {
			std::cout << "You cannot connect to the server, try again\n";
			return 1;
		}
	}
	std::thread t1(SendMessage, thisClient);
	t1.detach();
	
	// Receive messages
	while (thisClient.onLoop) {
		InputMemoryBitStream* input;

		Status s = thisClient.sock.Receive(input, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);
		if (s == Status::Done)
			thisClient.ManageMessageReceived(input, thisClient.serverInfo.IpServer, thisClient.serverInfo.PortServer);

	}
	system("pause");
	return 0;
}

