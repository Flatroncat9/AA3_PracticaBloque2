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
		
		// Game Info
		int x = 0;
		int y = 0;

	};

	// Move_idPlayer_idPlayer_IdMove_x_y
	// 
	int* integer = new int;




public:

	std::vector<std::pair<int, int>> enemyPos;
	bool onLoop = true;
	bool loggedIn = false;
	ServerInfo serverInfo;
	UDPSocket sock;
	ClientManager();

	bool CheckSalts(InputMemoryBitStream*& input);
	void Connect();
	void SendHello();
	void SendChallengeResponse(InputMemoryBitStream*& input);
	void Receive();
	void SetPosition(int _x, int _y);
	void SendAck(InputMemoryBitStream*& input);
	void ManageMessageReceived(InputMemoryBitStream*& input, std::string& ip, Port& port);
};

