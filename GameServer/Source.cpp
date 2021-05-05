#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <UDPSocket.h>

#define PORT 50000
#define BIND_CORRECT "Server has been binded correctly, waiting ...\n"

//Server
// server waits for hellos_nik
// when recieve a hello adds the client to a temporary list and responds with a challenge
// and waits from challenge response, before to send the welcome_id
// when server gets respons adds player to his table and send welcome with playerID in server
// envia mensajes de nevos jugadores, a los clientes ya existentes.

int main()
{
	UDPSocket serverSock;
	Status status = Status::Error;
	do
	{
		status = serverSock.Bind(PORT);
	} while (status == Status::Error);

	std::cout << BIND_CORRECT;

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