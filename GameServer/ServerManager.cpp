#include "ServerManager.h"

void ServerManager::Init()
{
	Status status = Status::Error;
	do
	{
		status = sock.Bind(PORT);
	} while (status == Status::Error);

	std::cout << BIND_CORRECT;

}

void ServerManager::Receive()
{
	while (true){
		InputMemoryBitStream* input;
		std::string ip;
		Port p;

		Status s = sock.Receive(input, ip, p);

		std::string i;
		input->ReadString(i, 8);

		ManageMessageReceived(i, ip, p);
	}
}

void ServerManager::ManageMessageReceived(std::string message, std::string ip, Port port)
{
	std::vector<std::string> splitedMessage = split(message, '_');
	
	
	switch (GetMessageProtocol(splitedMessage[0]))
	{
	case Message_Protocol::HELLO:
		//IsNewPlayer(splitedMessage[1]);
		break;
	case Message_Protocol::CHR:

		break;
	//TODO: start Machmaking case
	case Message_Protocol::MOVE:

		break;
	case Message_Protocol::ENDR:

		break;
	default:
		break;
	}
}

bool ServerManager::IsNewPlayer(std::string ip, Port port)
{
	/*
	for (int i = 0; i < clients.size(); i++)
	{
		if (clients[i].ip == ip)
		{
			return false;
		}
	}
	//clients.push_back(ClientInfo(ip, port));
	std::cout << "New client added";

	*/
	return true;
}
