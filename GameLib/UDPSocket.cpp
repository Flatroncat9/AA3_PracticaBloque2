#include "UDPSocket.h"
#define DATA_SIZE 65507

UDPSocket::UDPSocket()
{
	udpSocket = new sf::UdpSocket();
	udpSocket->setBlocking(0);
}

UDPSocket::UDPSocket(sf::UdpSocket* _udpSocket)
{
	udpSocket = _udpSocket;
	udpSocket->setBlocking(0);
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

Status UDPSocket::Send(OutputMemoryBitStream& _oms, const std::string ip, const Port port)
{
	return GetStatus(udpSocket->send(_oms.GetBufferPtr(), _oms.GetBitLength(), ip, port));
}

Status UDPSocket::Receive(sf::Packet& _pack, std::string& ip, Port& port)
{
	return GetStatus(udpSocket->receive(_pack, sf::IpAddress(ip), port));
}

Status UDPSocket::Receive(InputMemoryBitStream*& _ims, std::string& ip, Port& port)
{
	char data[DATA_SIZE];
	size_t receivedSize;
	sf::IpAddress address;
	Status s = GetStatus(udpSocket->receive(&data, DATA_SIZE, receivedSize, address, port));
	ip = address.toString();
	_ims = new InputMemoryBitStream(data, receivedSize);

	return s;
}

Status UDPSocket::Bind(Port port)
{
	return GetStatus(udpSocket->bind(port));
}

Port UDPSocket::GetLocalPort()
{
	return udpSocket->getLocalPort();
}
