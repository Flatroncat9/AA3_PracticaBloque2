#include "ClientManager.h"

ClientManager::ClientManager()
{
	srand(time(NULL));
	serverInfo.IpServer = SERVER_IP;
	serverInfo.PortServer = SERVER_PORT;
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
	int protocol = static_cast<int>(Message_Protocol::HELLO);
	output.Write(protocol, 32);
	output.Write(serverInfo.ClientSalt, 32);
	sock.Send(output, SERVER_IP, SERVER_PORT);
}

void ClientManager::SendChallengeResponse()
{
}

void ClientManager::Receive()
{
	while (true) {
		InputMemoryBitStream* input;
		std::string ip;
		Port p = 500;
		Status s = sock.Receive(input, ip, p);

		ManageMessageReceived(input, ip, p);
	}
}

void ClientManager::ManageMessageReceived(InputMemoryBitStream*& input, std::string& ip, Port& port)
{
	// Lee header
	int* i = new int;
	input->Read(i, 32);
	Message_Protocol protocol = static_cast<Message_Protocol>(*i);
	std::string msg;
	switch (protocol)
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

	case Message_Protocol::MESSAGE:
		input->ReadString(msg, 8);
		std::cout << msg << std::endl;
		break;

	default:

		break;
	}
}
