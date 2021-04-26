#include "UDPSocket.h"

UDPSocket::UDPSocket()
{
}

UDPSocket::UDPSocket(sf::UdpSocket* _udpSocket)
{
	udpSocket = _udpSocket;
}

UDPSocket::~UDPSocket()
{
	delete udpSocket;
}

sf::UdpSocket* UDPSocket::GetSocket()
{
	return udpSocket;
}

Status UDPSocket::Send(sf::Packet& _packet, const std::string ip, const Port port)
{
	return GetStatus(udpSocket->send(_packet, ip, port));
}

Status UDPSocket::Send(OutputMemoryStream& _oms, const std::string ip, const Port port)
{
	return Status();
}

Status UDPSocket::Receive(sf::Packet& _pack, std::string& ip, Port& port)
{
	return Status();
}

Status UDPSocket::Receive(InputMemoryStream* _ims, std::string& ip, Port& port)
{
	return Status();
}

Status UDPSocket::Bind(Port port)
{
	return Status();
}

Port UDPSocket::GetLocalPort()
{
	return Port();
}
