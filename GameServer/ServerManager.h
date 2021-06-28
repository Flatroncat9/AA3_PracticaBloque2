#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <UDPSocket.h>
#include <vector>

#define PORT 50000
#define BIND_CORRECT "Server has been binded correctly, waiting ...\n"
#define COUNTDOWN_DISCONNECT 30


class ServerManager
{
	struct ClientInfo
	{
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
		ClientVerified() {
			lastMessageReceived = std::chrono::system_clock::now();
			IpClient = "";
			PortClient = NULL;
			ClientSalt = NULL;
			ServerSalt = NULL;
		}
		ClientVerified(ClientInfo _copy) {
			lastMessageReceived = _copy.lastMessageReceived;
			IpClient = _copy.IpClient;
			PortClient = _copy.PortClient;
			ClientSalt = _copy.ClientSalt;
			ServerSalt = _copy.ServerSalt;
			
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
			return aux;
		}

	};



	UDPSocket sock;
	std::vector<ClientInfo> pendingClients;
	std::map<uint32_t, ClientVerified*> clients;

public:
	bool onLoop = true;
	InputMemoryBitStream* input;
	int* integer = new int;


	Status BindSock(Port p);
	Status Receive(InputMemoryBitStream*& input, std::string& ip, Port& p);
	void Init();
	void SendMessageToPlayers();
	void Receive();
	void ManageMessageReceived(std::string message, std::string ip, Port port);
	void SendNewPlayerConnection();
	void SendChallenge(uint32_t ClientSalt);
	void SendWelcome();
	void CheckLastMessage();
	void DisconnectClient();
	void Disconnect();
	bool IsNewPlayer(std::string ip, Port port, uint32_t clientSalt);
};

