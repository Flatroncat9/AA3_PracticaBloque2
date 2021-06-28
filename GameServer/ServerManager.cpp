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

void ServerManager::SendMessageToPlayers()
{
	std::string msg;
	input->ReadString(msg, 8);
	OutputMemoryBitStream oms;
	oms.Write(static_cast<int>(Message_Protocol::MESSAGE), 32);
	oms.WriteString(msg, 8);
	for (auto it = clients.begin(); it != clients.end(); it++) {
		sock.Send(oms, it->second->IpClient, it->second->PortClient);
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
	for (auto it = clients.begin(); it != clients.end(); it++) {
		OutputMemoryBitStream message;
		message.Write(static_cast<int>(Message_Protocol::MESSAGE), 32);
		message.WriteString("New Client Connected", 8);
		sock.Send(message, it->second->IpClient, it->second->PortClient);
	}
}

void ServerManager::SendChallenge(uint32_t _clientSalt)
{
	// Checks in notVerified clients, if found send Challenge
	for (int i = 0; i < pendingClients.size(); i++) {
		if (pendingClients[i].ClientSalt == _clientSalt) {
			OutputMemoryBitStream message;
			message.Write(static_cast<int>(Message_Protocol::CH), 32);
			message.Write(pendingClients[i].ClientSalt, 32);
			message.Write(pendingClients[i].ServerSalt, 32);
			sock.Send(message, pendingClients[i].IpClient, pendingClients[i].PortClient);
		}
	}

	// Checks in Verified clients, if found send Welcome
	auto it = clients.find(*integer);
	if (it != clients.end()) {
		OutputMemoryBitStream message;
		message.Write(static_cast<int>(Message_Protocol::WELCOME), 32);
		message.Write(it->second->ClientSalt, 32);
		message.Write(it->second->ServerSalt, 32);
		sock.Send(message, it->second->IpClient, it->second->PortClient);
	}
}

void ServerManager::SendWelcome()
{
	// Checks in notVerified clients, if found send Welcome
	input->Read(integer, 32);
	for (auto i = pendingClients.begin(); i != pendingClients.end(); i++) {
		if (i->ClientSalt == *integer) {
			input->Read(integer, 32);
			if (i->ServerSalt == *integer) {
				OutputMemoryBitStream message;
				message.Write(static_cast<int>(Message_Protocol::WELCOME), 32);
				message.Write(i->ClientSalt, 32);
				message.Write(i->ServerSalt, 32);
				sock.Send(message, i->IpClient, i->PortClient);
				ClientVerified* aux = new ClientVerified(*i);
				clients[i->ClientSalt] = aux;
				pendingClients.erase(i);
				break;
			}
			
		}
	}

	// Checks in Verified clients, if found send Welcome
	auto it = clients.find(*integer);
	if (it != clients.end()) {
		input->Read(integer, 32);
		if (it->second->ServerSalt == *integer) {
			OutputMemoryBitStream message;
			message.Write(static_cast<int>(Message_Protocol::WELCOME), 32);
			message.Write(it->second->ClientSalt, 32);
			message.Write(it->second->ServerSalt, 32);
			sock.Send(message, it->second->IpClient, it->second->PortClient);
		}
	}
}

void ServerManager::CheckLastMessage()
{
	// Checks Verified
	auto now = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;
	for (auto it = clients.begin(); it != clients.end(); it++) {
		elapsed_seconds = now - it->second->lastMessageReceived;
		if (elapsed_seconds.count() > COUNTDOWN_DISCONNECT) {
			clients.erase(it);
		}
	}

	// Checks notVerified
	for (auto i = pendingClients.begin(); i != pendingClients.end(); i++) {
		elapsed_seconds = now - i->lastMessageReceived;
		if (elapsed_seconds.count() > COUNTDOWN_DISCONNECT) {
			pendingClients.erase(i);
			break;
		}
	}

}

void ServerManager::DisconnectClient()
{
	input->Read(integer, 32);
	auto it = clients.find(*integer);
	if (it != clients.end()) {
		input->Read(integer, 32);
		if (it->second->ServerSalt == *integer) {
			for (auto i = clients.begin(); i != clients.end(); i++) {
				OutputMemoryBitStream message;
				message.Write(static_cast<int>(Message_Protocol::ENDR), 32);
				message.Write(i->second->ClientSalt, 32);
				message.Write(i->second->ServerSalt, 32);
				sock.Send(message, i->second->IpClient, i->second->PortClient);
			}
			// Deletes client data
			clients.erase(it);
		}
	}

}

void ServerManager::Disconnect()
{
	for (auto i = clients.begin(); i != clients.end(); i++) {
		OutputMemoryBitStream message;
		message.Write(static_cast<int>(Message_Protocol::END), 32);
		message.Write(i->second->ClientSalt, 32);
		message.Write(i->second->ServerSalt, 32);
		sock.Send(message, i->second->IpClient, i->second->PortClient);
	}
	onLoop = false;
	clients.clear();
	pendingClients.clear();
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


