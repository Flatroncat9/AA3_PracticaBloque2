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
		std::string IpServer = SERVER_IP;
		Port PortServer = SERVER_PORT;
		bool isConnected;
		uint32_t ClientSalt;
		uint32_t ServerSalt;
	};

	short id; // salt
	// Move_idPlayer_idPlayer_IdMove_x_y
	// 

public:
	ServerInfo serverInfo;
	UDPSocket sock;
	ClientManager();

	void Connect();
	void SendHello();
	void SendChallengeResponse();
	void Receive();
	void ManageMessageReceived(InputMemoryBitStream*& input, std::string& ip, Port& port);
};

