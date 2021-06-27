#include "ServerManager.h"

Status ServerManager::BindSock(Port p)
{
	return sock.Bind(p);
}

Status ServerManager::Receive(InputMemoryBitStream*& input, std::string& ip, Port& p)
{
	return sock.Receive(input, ip, p);
}

void ServerManager::Init()
{
	Status status = Status::Error;
	do
	{
		status = sock.Bind(PORT);
	} while (status == Status::Error);

	std::cout << BIND_CORRECT;

}

void ServerManager::SendMessageToPlayers(InputMemoryBitStream* input)
{
	std::string msg;
	input->ReadString(msg, 8);
	OutputMemoryBitStream oms;
	oms.WriteString(msg, 8);
	for (int i = 0; i < pendingClients.size(); i++) {
		sock.Send(oms, pendingClients[i].IpClient, pendingClients[i].PortClient);
	}
}

void ServerManager::Receive()
{
	while (true){
		InputMemoryBitStream* input;
		std::string ip;
		Port p = 50000;

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

void ServerManager::SendNewPlayerConnection()
{
	for (int i = 0; i < pendingClients.size(); i++) {
		OutputMemoryBitStream message;
		message.Write(static_cast<int>(Message_Protocol::MESSAGE), 32);
		message.WriteString("New Client Connected", 8);
		sock.Send(message, pendingClients[i].IpClient, pendingClients[i].PortClient);
	}
}

bool ServerManager::IsNewPlayer(std::string ip, Port port, uint32_t clientSalt)
{
	
	for (int i = 0; i < pendingClients.size(); i++)
	{
		if (pendingClients[i].IpClient == ip && pendingClients[i].PortClient == port)
		{
			return false;
		}
	}
	pendingClients.push_back(ClientInfo(ip, port, clientSalt));
	std::cout << "New client added: " << ip << " " << port << " " << clientSalt << std::endl;

	return true;
}
