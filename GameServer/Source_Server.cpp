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
#define W_WINDOW_TITLE 80
#define H_WINDOW_TITLE 60

void CheckLastMessage(ServerManager* server) {
	auto start = std::chrono::system_clock::now();
	while (server->onLoop) {
		std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
		if (elapsed_seconds.count() > 1) {
			server->CheckLastMessage();
			start = std::chrono::system_clock::now();
		}
	}
}

void CheckPackets(ServerManager* server) {
	while (server->onLoop) {
		for (auto it = server->packets.begin(); it != server->packets.end(); it++) {
			it->second->SendAgain(server->sock);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void SendMovement(ServerManager* server) {
	while (server->onLoop) {
		for (auto it = server->clients.begin(); it != server->clients.end(); it++) {
			OutputMemoryBitStream oms;
			oms.Write(static_cast<int>(Message_Protocol::OKMOVE), 32);
			oms.Write(it->second->ClientSalt, 32);
			oms.Write(it->second->ServerSalt, 32);
			oms.Write(it->second->clientId, 32);
			oms.Write(it->second->moveId, 32);
			oms.Write(it->second->x, 32);
			oms.Write(it->second->y, 32);
			server->sock.Send(oms, it->second->IpClient, it->second->PortClient);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void SendExitMessage(ServerManager* server) {
	std::string msg;
	std::cin >> msg;
	if (msg == "exit" || msg == "EXIT") {
		server->Disconnect();
	}
}

int main()
{
	srand(time(NULL));
	ServerManager* myServer = new ServerManager();
	Status status = Status::Error;
	std::string ip = SERVER_IP;
	Port p = SERVER_PORT;
	status = myServer->BindSock(p);

	std::thread t1(CheckLastMessage, myServer);
	t1.detach();
	std::thread t2(SendExitMessage, myServer);
	t2.detach();
	std::thread t3(SendMovement, myServer);
	t3.detach();
	std::thread t4(CheckPackets, myServer);
	t4.detach();
	
	while (myServer->onLoop)
	{
		std::string clientIP;
		Port clientPort = 0;

		Status s = myServer->Receive(myServer->input, clientIP, clientPort);
		if (s == Status::Done) {
			myServer->input->Read(myServer->integer, 32);		// Para leer Int y varios
			switch (static_cast<Message_Protocol>(*myServer->integer)) {
				case Message_Protocol::HELLO:
					myServer->input->Read(myServer->integer, 32);
					myServer->IsNewPlayer(clientIP, clientPort, *myServer->integer);
					myServer->SendNewPlayerConnection();
					myServer->SendChallenge(*myServer->integer);
					break;
				case Message_Protocol::CHR:
					myServer->SendWelcome();
					break;
				case Message_Protocol::MOVE:
					myServer->Accumulate();
					break;
				case Message_Protocol::STARTMATCHMAKING:
					break;
				case Message_Protocol::PONG:
					break;
				case Message_Protocol::END:
					myServer->DisconnectClient();
				case Message_Protocol::ENDR:

					break;
				case Message_Protocol::MESSAGE:
					myServer->SendMessageToPlayers();
					break;
				case Message_Protocol::ACK:
					myServer->ErasePacket();
			}
		}
	}
	

	return 0;
}

