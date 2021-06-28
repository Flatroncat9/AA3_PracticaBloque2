#pragma once
#include<UDPSocket.h>

class CriticalPacket
{
	OutputMemoryBitStream* packet;
	std::string ipAdress = "localHost";
	Port port = 0;
	int clientSalt = 0;
	int serverSalt = 0;
public:
	CriticalPacket() {};
	CriticalPacket(OutputMemoryBitStream* oms, int _port, int _clientSalt, int _serverSalt) {
		packet = oms;
		port = _port;
		clientSalt = _clientSalt;
		serverSalt = _serverSalt;
	}
	~CriticalPacket();
	bool CheckPacket(int _clientSalt, int serverSalt);
	void SendAgain(UDPSocket sock);
};

