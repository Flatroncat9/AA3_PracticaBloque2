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

}

void ServerManager::SendNewPlayerConnection()
{

	for (auto it = clients.begin(); it != clients.end(); it++) {
		OutputMemoryBitStream* message = new OutputMemoryBitStream();
		//Protocol + Salt
		message->Write(static_cast<int>(Message_Protocol::NEWPLAYER), 32);
		message->Write(it->second->ClientSalt, 32);
		message->Write(it->second->ServerSalt, 32);
		//PacketID
		message->Write(totalPacketID, 32);
		//Pos
		message->Write(it->second->x, 32);
		message->Write(it->second->y, 32);
		sock.Send(*message, it->second->IpClient, it->second->PortClient);
		// Adds packet to criticalMap
		CriticalPacket* packet = new CriticalPacket(message, it->second->PortClient, it->second->ClientSalt, it->second->ServerSalt);
		packets[totalPacketID] = packet;
		totalPacketID++;
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
		message.Write(it->second->clientId, 32);
		message.Write(it->second->x, 32);
		message.Write(it->second->y, 32);
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
				OutputMemoryBitStream* message = new OutputMemoryBitStream();
				message->Write(static_cast<int>(Message_Protocol::WELCOME), 32);
				message->Write(i->ClientSalt, 32);
				message->Write(i->ServerSalt, 32);
				ClientVerified* aux = new ClientVerified(*i);
				aux->clientId = clientsCounter;
				message->Write(aux->clientId, 32);
				message->Write(aux->x, 32);
				message->Write(aux->y, 32);
				sock.Send(*message, i->IpClient, i->PortClient);
				clients[aux->clientId] = aux;
				clientsCounter++;

				std::cout << "New Client with ID: " << aux->clientId;
				SendNewPlayerConnection();
				// Erase client from notVerified
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
			message.Write(it->second->clientId, 32);
			message.Write(it->second->x, 32);
			message.Write(it->second->y, 32);
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
			//TODO enviar Disconnect a los demás
			clients.erase(it);

			for (auto i = clients.begin(); i != clients.end(); i++) {
				OutputMemoryBitStream* message = new OutputMemoryBitStream();
				message->Write(static_cast<int>(Message_Protocol::DISCONNECTED), 32);
				message->Write(i->second->ClientSalt, 32);
				message->Write(i->second->ServerSalt, 32);
				message->Write(totalPacketID, 32);
				sock.Send(*message, i->second->IpClient, i->second->PortClient);
				// Adds packet to criticalMap
				CriticalPacket* packet = new CriticalPacket(message, i->second->PortClient, i->second->ClientSalt, i->second->ServerSalt);
				packets[totalPacketID] = packet;
				totalPacketID++;

			}
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
				OutputMemoryBitStream *message = new OutputMemoryBitStream();
				message->Write(static_cast<int>(Message_Protocol::DISCONNECTED), 32);
				message->Write(i->second->ClientSalt, 32);
				message->Write(i->second->ServerSalt, 32);
				message->Write(totalPacketID, 32);
				sock.Send(*message, i->second->IpClient, i->second->PortClient);
				// Adds packet to criticalMap
				CriticalPacket* packet = new CriticalPacket(message, it->second->PortClient, it->second->ClientSalt, it->second->ServerSalt);
				packets[totalPacketID] = packet;
				totalPacketID++;

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

void ServerManager::ErasePacket()
{
	int clientSalt, serverSalt, packetID;
	input->Read(&clientSalt, 32);
	input->Read(&serverSalt, 32);
	input->Read(&packetID, 32);

	std::map<int, CriticalPacket*>::iterator it = packets.find(packetID);
	if(it != packets.end()){

		if (it->second->CheckPacket(clientSalt, serverSalt)) {
			packets.erase(it);
		}
		
	}

}

void ServerManager::Accumulate()
{
	int clientSalt, serverSalt, idPlayer, idMove, posX, posY;
	input->Read(&clientSalt, 32);
	input->Read(&serverSalt, 32);
	input->Read(&idPlayer, 32);
	input->Read(&idMove, 32);
	input->Read(&posX, 32);
	input->Read(&posY, 32);
	auto it = clients.find(idPlayer);
	if (it != clients.end()) {
		if (it->second->ClientSalt == clientSalt && it->second->ServerSalt == serverSalt) {
			if (it->second->moveId < idMove) {
				// 
				if (posX >= W_NUM_TILES)
					posX = W_NUM_TILES - 1;
				else if (posX < 0)
					posX = 0;
				if (posY >= H_NUM_TILES)
					posY = H_NUM_TILES - 1;
				else if (posY < 0)
					posY = 0;
				it->second->x = posX;
				it->second->y = posY;
				
			}
		}
	}
}

UDPSocket ServerManager::GetSocket()
{
	return sock;
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


