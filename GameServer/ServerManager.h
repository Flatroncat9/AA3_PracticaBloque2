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
	struct ClientProxy
	{
		// var datos juego
		std::string IpClient;
		Port PortClient;
		uint32_t ClientSalt;
		uint32_t ServerSalt;
	};

	UDPSocket sock;
	std::vector<ClientProxy> pendingClients;
	// clients key = <ip>:std::tostring(<port>)
	std::map<std::string, ClientProxy> clients;

	void Init();
	void Receive();
	void ManageMessageReceived(std::string message, std::string ip, Port port);

	bool IsNewPlayer(std::string ip, Port port);
};

