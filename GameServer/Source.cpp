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
	
	while (true)
	{
		InputMemoryBitStream* input;
		std::string ip = SERVER_IP;
		Port p = SERVER_PORT;

		Status s = serverSock.Receive(input, ip, p);
		if (s != Status::Error) {
			int* i = new int;
			input->ReadBits(i, 64);
		}


		int* i = new int;
		input->ReadBits(i, 64);
		if(*i == 1)
			std::cout << ip << "_" << p << "  " << i << "\n";

		//PlayerInfo playerInfo;
	}
	

	system("pause");
	return 0;
}