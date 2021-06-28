#include "CriticalPacket.h"

CriticalPacket::~CriticalPacket()
{
}

bool CriticalPacket::CheckPacket(int _clientSalt, int _serverSalt)
{
	return (_clientSalt == clientSalt && _serverSalt == serverSalt);
}

void CriticalPacket::SendAgain(UDPSocket sock)
{
	sock.Send(*packet, ipAdress, port);
}
