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
		ClientInfo(std::string ip, Port port, uint32_t _clientSalt) {
			IpClient = ip;
			PortClient = port;
			ClientSalt = _clientSalt;
			ServerSalt = rand() % UINT32_MAX;
		};

		uint32_t GetChallenge();
		void CreateServerSalt();
	};

	struct ClientVerified: ClientInfo
	{
		uint32_t mmr;
		std::string nickName;
		

	};



	UDPSocket sock;
	std::vector<ClientInfo> pendingClients;
	std::map<std::string, ClientVerified> clients;

public:
	InputMemoryBitStream* input;

	Status BindSock(Port p);
	Status Receive(InputMemoryBitStream*& input, std::string& ip, Port& p);
	void Init();
	void SendMessageToPlayers(InputMemoryBitStream* input);
	void Receive();
	void ManageMessageReceived(std::string message, std::string ip, Port port);
	void SendNewPlayerConnection();

	bool IsNewPlayer(std::string ip, Port port, uint32_t clientSalt);
};

