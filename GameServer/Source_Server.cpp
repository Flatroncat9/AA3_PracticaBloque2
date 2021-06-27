#pragma once
#include "ServerManager.h"


//Server
// server waits for hellos_nik
// when recieve a hello adds the client to a temporary list and responds with a challenge
// and waits from challenge response, before to send the welcome_id
// when server gets respons adds player to his table and send welcome with playerID in server
// envia mensajes de nevos jugadores, a los clientes ya existentes.

#define SERVER_IP "localhost"
#define SERVER_PORT 50000

int main()
{
	ServerManager myServer;
	InputMemoryBitStream* input;
	Status status = Status::Error;
	std::string ip = SERVER_IP;
	Port p = SERVER_PORT;
	status = myServer.BindSock(p);
	
	while (true)
	{
		std::string clientIP;
		Port clientPort = 0;

		Status s = myServer.Receive(input, clientIP, clientPort);
		if (true) {
			int* i = new int[1000];
			input->Read(i, 32);		// Para leer Int y varios
			switch (static_cast<Message_Protocol>(*i)) {
				case Message_Protocol::HELLO:
					input->Read(i, 32);
					myServer.IsNewPlayer(clientIP, clientPort, *i);
					myServer.SendNewPlayerConnection();
					break;
				case Message_Protocol::CHR:
					break;
				case Message_Protocol::MOVE:
					break;
				case Message_Protocol::STARTMATCHMAKING:
					break;
				case Message_Protocol::PONG:
					break;
				case Message_Protocol::ENDR:
					break;
				case Message_Protocol::MESSAGE:
					myServer.SendMessageToPlayers(input);
					break;
			}
			std::cout << ip << "_" << p << "  " << *i << "\n";
		}
	}
	

	//system("pause");
	return 0;
}