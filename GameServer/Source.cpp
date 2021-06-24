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
	UDPSocket serverSock;
	Status status = Status::Error;
	InputMemoryBitStream* input;
	std::string ip = SERVER_IP;
	Port p = SERVER_PORT;
	status = serverSock.Bind(p);
	
	while (true)
	{
		std::string clientIP;
		Port clientPort;

		Status s = serverSock.Receive(input, clientIP, clientPort);
		if (s != Status::Error) {
			int* i = new int[1000];
			//input->ReadString(i, 64);		// Para leer strings
			//input->ReadBits(i, 64);		// Para leer bits
			input->Read(i, 32);		// Para leer Int y varios
			std::cout << ip << "_" << p << "  " << *i << "\n";
		}
		//PlayerInfo playerInfo;
	}
	

	system("pause");
	return 0;
}