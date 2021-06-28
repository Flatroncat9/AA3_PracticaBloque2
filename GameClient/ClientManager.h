#pragma once

#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>
#include <list>

#define SERVER_IP "localhost"
#define SERVER_PORT 50000


class ClientManager
{
	struct ServerInfo
	{
		
		std::string IpServer = SERVER_IP;
		Port PortServer = SERVER_PORT;
		bool isConnected = false;
		uint32_t ClientSalt;
		uint32_t ServerSalt;
		
		// Game Info
		
	};

	
	// Move_idPlayer_idPlayer_IdMove_x_y
	// 
	int* integer = new int;

	int myID; 
	unsigned int totalAccum = 0;
	std::list<AccumMove> moves;

public:

	int x = 0;
	int y = 0;
	int accumX = 0;
	int accumY = 0;


	std::vector<std::pair<int, int>> enemyPos;
	bool onLoop = true;
	bool loggedIn = false;
	ServerInfo serverInfo;
	UDPSocket sock;
	ClientManager();


	int GetID();
	bool CheckSalts(InputMemoryBitStream*& input);
	void SendHello();
	void SendChallengeResponse(InputMemoryBitStream*& input);
	void AddAccum(int _x, int _y);
	void SendAccum();
	void MovePlayer(InputMemoryBitStream*& input);
	void SetPosition(int _x, int _y);
	void SendAck(InputMemoryBitStream*& input);
	void ManageMessageReceived(InputMemoryBitStream*& input, std::string& ip, Port& port);
};

