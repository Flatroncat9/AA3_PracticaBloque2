#include "UDPSocket.h"

UDPSocket::UDPSocket()
{
	udpSocket = new sf::UdpSocket();
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

Status UDPSocket::Send(OutputMemoryBitStream& _oms, const std::string ip, const Port port)
{
	return GetStatus(udpSocket->send(_oms.GetBufferPtr(), _oms.GetByteLength(), ip, port));
}

Status UDPSocket::Receive(sf::Packet& _pack, std::string& ip, Port& port)
{
	return GetStatus(udpSocket->receive(_pack, sf::IpAddress(ip), port));
}

Status UDPSocket::Receive(InputMemoryBitStream*& _ims, std::string& ip, Port& port)
{
	char data[1000];
	size_t t(1000);
	size_t receivedSize;
	sf::IpAddress address;

	Status s = GetStatus(udpSocket->receive(data, t, receivedSize, address, port));

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
