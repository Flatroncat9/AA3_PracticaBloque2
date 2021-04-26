#pragma once
#include "utils.h"
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"

class UDPSocket
{
	sf::UdpSocket *udpSocket;

public:
	UDPSocket();
	UDPSocket(sf::UdpSocket* _udpSocket);
    ~UDPSocket();

	sf::UdpSocket* GetSocket();

	Status Send(sf::Packet& _packet, const std::string ip, const Port port);
	Status Send(OutputMemoryStream& _oms, const std::string ip, const Port port);

	Status Receive(sf::Packet& _pack, std::string& ip, Port &port);
	Status Receive(InputMemoryStream* _ims, std::string& ip, Port& port);

	Status Bind(Port port);
	Port GetLocalPort();
};

