#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <UDPSocket.h>
#include <vector>
#include <queue>
#include "CriticalPacket.h"

#define PORT 50000
#define BIND_CORRECT "Server has been binded correctly, waiting ...\n"
#define COUNTDOWN_DISCONNECT 10


class ServerManager
{
	struct ClientInfo
	{
	public:
		// time_point last message
		std::chrono::system_clock::time_point lastMessageReceived = std::chrono::system_clock::now();

		// Client Data
		std::string IpClient;
		Port PortClient;
		uint32_t ClientSalt;
		uint32_t ServerSalt;

		ClientInfo() {
			lastMessageReceived = std::chrono::system_clock::now();
			IpClient = "";
			PortClient = NULL;
			ClientSalt = NULL;
			ServerSalt = NULL;
		}; 
		ClientInfo(std::string ip, Port port, uint32_t _clientSalt) {
			lastMessageReceived = std::chrono::system_clock::now();
			IpClient = ip;
			PortClient = port;
			ClientSalt = _clientSalt;
			ServerSalt = rand() % UINT32_MAX;
		};
	};

	struct ClientVerified: ClientInfo
	{
		// Game Data
		uint32_t mmr = 0;
		std::string nickName = "";
		int clientId = 0;
		int moveId = 0;
		int x = 0;
		int y = 0;

		ClientVerified() {
			lastMessageReceived = std::chrono::system_clock::now();
			IpClient = "";
			PortClient = NULL;
			ClientSalt = NULL;
			ServerSalt = NULL;
		}
		ClientVerified(ClientInfo _copy) {
			mmr = 0;
			nickName = "";
			lastMessageReceived = _copy.lastMessageReceived;
			IpClient = _copy.IpClient;
			PortClient = _copy.PortClient;
			ClientSalt = _copy.ClientSalt;
			ServerSalt = _copy.ServerSalt;
			x = rand() % W_NUM_TILES;
			y = rand() % H_NUM_TILES;
			std::cout << "Pos: " << x << " " << y << std::endl;
			
		}
		ClientVerified operator=(ClientVerified second) {
			ClientVerified aux;
			aux.lastMessageReceived = second.lastMessageReceived;
			aux.mmr = second.mmr;
			aux.nickName = second.nickName;
			aux.IpClient = second.IpClient;
			aux.PortClient = second.PortClient;
			aux.ClientSalt = second.ClientSalt;
			aux.ServerSalt = second.ServerSalt;
			aux.x = second.x;
			aux.y = second.y;
			return aux;
		}

	};

	// Critical Packets counter
	unsigned int totalPacketID = 0;

	int clientsCounter = 0;
	

	std::vector<ClientInfo> pendingClients;
	std::queue<AccumMove> moves;

public:
	UDPSocket sock;

	std::map<int, ClientVerified*> clients;
	// Critical Packets
	std::map<int, CriticalPacket*> packets;

	bool onLoop = true;
	InputMemoryBitStream* input;
	int* integer = new int;


	Status BindSock(Port p);
	Status Receive(InputMemoryBitStream*& input, std::string& ip, Port& p);
	void Init();
	void SendMessageToPlayers();
	void SendNewPlayerConnection();
	void SendChallenge(uint32_t ClientSalt);
	void SendWelcome();
	void CheckLastMessage();
	void DisconnectClient();
	void Disconnect();
	void ErasePacket();
	void Accumulate();
	bool IsNewPlayer(std::string ip, Port port, uint32_t clientSalt);
};

