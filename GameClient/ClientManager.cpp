#include "ClientManager.h"

ClientManager::ClientManager()
{
	srand(time(NULL));
	serverInfo.IpServer = SERVER_IP;
	serverInfo.PortClient = SERVER_PORT;
	serverInfo.isConnected = false;
	serverInfo.ClientSalt = rand() % UINT32_MAX;
}

void ClientManager::Connect()
{
	


}

void ClientManager::SendHello()
{
	OutputMemoryBitStream output;
	//std::string msg =
	//	GetMessageProtocolFrom(Message_Protocol::HELLO);
	int protocol = 1;//static_cast<int>(Message_Protocol::HELLO);
	output.Write(protocol, 64);
	//output.Write(serverInfo.ClientSalt, 4);
	sock.Send(output, SERVER_IP, SERVER_PORT);
}

void ClientManager::SendChallengeResponse()
{
}

void ClientManager::Receive()
{
	
}

void ClientManager::ManageMessageReceived(std::string message, std::string ip, Port port)
{
	
	std::vector<std::string> splitedMessage = split(message, '_');
	switch (GetMessageProtocol(splitedMessage[0]))
	{
	case Message_Protocol::CH:

		break;
	case Message_Protocol::WELCOME:

		break;
		//TODO: start Machmaking case
	case Message_Protocol::NEWPLAYER:

		break;
	case Message_Protocol::DISCONNECTED:

		break;
	case Message_Protocol::END:

		break;

	default:
		break;
	}
}
