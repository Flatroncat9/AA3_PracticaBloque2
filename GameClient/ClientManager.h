#pragma once

#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>

#define SERVER_IP "localhost"
#define SERVER_PORT 50000


class ClientManager
{
	struct ServerInfo
	{
		std::string IpServer;
		Port PortClient;
		bool isConnected;
		uint32_t ClientSalt;
		uint32_t ServerSalt;
	};

	ServerInfo serverInfo; 
	short id; // salt
	// Move_idPlayer_idPlayer_IdMove_x_y
	// 
	UDPSocket sock;

public:
	ClientManager();

	void Connect();
	void SendHello();
	void SendChallengeResponse();
	void Receive();
	void ManageMessageReceived(std::string message, std::string ip, Port port);
};

