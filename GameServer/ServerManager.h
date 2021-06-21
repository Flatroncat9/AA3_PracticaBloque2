#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <UDPSocket.h>
#include <vector>

#define PORT 50000
#define BIND_CORRECT "Server has been binded correctly, waiting ...\n"


class ServerManager
{
	struct ClientInfo
	{
		// var datos juego
		std::string IpClient;
		Port PortClient;
		uint32_t ClientSalt;
		uint32_t ServerSalt;

		ClientInfo(); // generar Server Salt

		uint32_t GetChallenge();
		void CreateServerSlat();
	};

	struct ClientVerified: ClientInfo
	{
		uint32_t mmr;
		std::string nickName;
		

	};



	UDPSocket sock;
	std::vector<ClientInfo> pendingClients;
	// clients key = <ip>:std::tostring(<port>)
	std::map<std::string, ClientVerified> clients;

	void Init();
	void Receive();
	void ManageMessageReceived(std::string message, std::string ip, Port port);

	bool IsNewPlayer(std::string ip, Port port);
};

