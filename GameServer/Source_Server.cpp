#pragma once
#include "ServerManager.h"
#include <thread>

//Server
// server waits for hellos_nik
// when recieve a hello adds the client to a temporary list and responds with a challenge
// and waits from challenge response, before to send the welcome_id
// when server gets respons adds player to his table and send welcome with playerID in server
// envia mensajes de nevos jugadores, a los clientes ya existentes.

#define SERVER_IP "localhost"
#define SERVER_PORT 50000

void CheckLastMessage(ServerManager server) {
	auto start = std::chrono::system_clock::now();
	while (true) {
		std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
		if (elapsed_seconds.count() > 1) {
			server.CheckLastMessage();
			start = std::chrono::system_clock::now();
		}
	}
}

void SendExitMessage(ServerManager& server) {
	std::string msg;
	std::cin >> msg;
	if (msg == "exit" || msg == "EXIT") {
		server.Disconnect();
	}
}

int main()
{
	ServerManager myServer;
	Status status = Status::Error;
	std::string ip = SERVER_IP;
	Port p = SERVER_PORT;
	status = myServer.BindSock(p);

	std::thread t1(CheckLastMessage, myServer);
	t1.detach();
	std::thread t2(SendExitMessage, myServer);
	t2.detach();
	
	while (myServer.onLoop)
	{
		std::string clientIP;
		Port clientPort = 0;

		Status s = myServer.Receive(myServer.input, clientIP, clientPort);
		if (s == Status::Done) {
			myServer.input->Read(myServer.integer, 32);		// Para leer Int y varios
			switch (static_cast<Message_Protocol>(*myServer.integer)) {
				case Message_Protocol::HELLO:
					myServer.input->Read(myServer.integer, 32);
					myServer.IsNewPlayer(clientIP, clientPort, *myServer.integer);
					myServer.SendNewPlayerConnection();
					myServer.SendChallenge(*myServer.integer);
					break;
				case Message_Protocol::CHR:
					myServer.SendWelcome();
					break;
				case Message_Protocol::MOVE:
					break;
				case Message_Protocol::STARTMATCHMAKING:
					break;
				case Message_Protocol::PONG:
					break;
				case Message_Protocol::END:
					myServer.DisconnectClient();
				case Message_Protocol::ENDR:

					break;
				case Message_Protocol::MESSAGE:
					myServer.SendMessageToPlayers();
					break;
			}
			std::cout << ip << "_" << p << "  " << *myServer.integer << "\n";
		}
	}
	

	return 0;
}

