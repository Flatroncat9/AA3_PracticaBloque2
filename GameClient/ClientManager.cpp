#include "ClientManager.h"

ClientManager::ClientManager()
{
	srand(time(NULL));
	serverInfo.IpServer = SERVER_IP;
	serverInfo.PortServer = SERVER_PORT;
	serverInfo.isConnected = false;
	serverInfo.ClientSalt = rand() % UINT32_MAX;
}

int ClientManager::GetID()
{
	return myID;
}

bool ClientManager::CheckSalts(InputMemoryBitStream*& input)
{
	input->Read(integer, 32);
	if (serverInfo.ClientSalt == *integer) {
		input->Read(integer, 32);
		return (serverInfo.ServerSalt == *integer);
	}
	else
		return false;
}

void ClientManager::Connect()
{
	


}

void ClientManager::SendHello()
{
	OutputMemoryBitStream output;
	int protocol = static_cast<int>(Message_Protocol::HELLO);
	output.Write(protocol, 32);
	output.Write(serverInfo.ClientSalt, 32);
	sock.Send(output, SERVER_IP, SERVER_PORT);
}

void ClientManager::SendChallengeResponse(InputMemoryBitStream*& input)
{
	input->Read(integer, 32);
	if (*integer == serverInfo.ClientSalt) {
		input->Read(integer, 32);
		serverInfo.ServerSalt = *integer;
		OutputMemoryBitStream oms;
		oms.Write(static_cast<int>(Message_Protocol::CHR), 32);
		oms.Write(serverInfo.ClientSalt, 32);
		oms.Write(serverInfo.ServerSalt, 32);
		sock.Send(oms, serverInfo.IpServer, serverInfo.PortServer);
	}
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

void ClientManager::AddAccum( int _x, int _y)
{
	serverInfo.accumX += _x;
	serverInfo.accumY += _y;
}

void ClientManager::SendAccum()
{
	AccumMove aux;
	aux.idMove = totalAccum;
	aux.idPlayer = myID;
	aux.x = serverInfo.accumX + serverInfo.x;
	aux.y = serverInfo.accumY + serverInfo.y;
	totalAccum++;
	moves.push_back(aux);
	
	OutputMemoryBitStream oms;
	oms.Write(static_cast<int>(Message_Protocol::MOVE), 32);
	oms.Write(serverInfo.ClientSalt, 32);
	oms.Write(serverInfo.ServerSalt, 32);
	oms.Write(aux.idPlayer, 32);
	oms.Write(aux.idMove, 32);
	oms.Write(aux.x, 32);
	oms.Write(aux.y, 32);
	sock.Send(oms, serverInfo.IpServer, serverInfo.PortServer);
}

void ClientManager::SetPosition(int _x, int _y)
{
	serverInfo.x = _x;
	serverInfo.y = _y;
}

void ClientManager::SendAck(InputMemoryBitStream*& input)
{
	int packetID;
	input->Read(&packetID, 32);
	OutputMemoryBitStream oms;
	oms.Write(static_cast<int>(Message_Protocol::ACK), 32);
	oms.Write(serverInfo.ClientSalt, 32);
	oms.Write(serverInfo.ServerSalt, 32);
	oms.Write(packetID, 32);
	sock.Send(oms, serverInfo.IpServer, serverInfo.PortServer);

	// Saves enemy position to show in the map
	int x, y;
	input->Read(&x, 32);
	input->Read(&y, 32);
	enemyPos.push_back(std::pair<int, int>(x, y));
}

void ClientManager::ManageMessageReceived(InputMemoryBitStream*& input, std::string& ip, Port& port)
{
	// Lee header
	input->Read(integer, 32);
	Message_Protocol protocol = static_cast<Message_Protocol>(*integer);
	std::cout << GetMessageProtocolFrom(protocol) << std::endl;
	std::string msg;
	switch (protocol)
	{
	case Message_Protocol::CH:
		SendChallengeResponse(input);
		break;
	case Message_Protocol::WELCOME:
		if (CheckSalts(input)) {
			loggedIn = true;
			std::cout << "I've received a Welcome\n";
			int x, y;
			input->Read(&myID, 32);
			input->Read(&x, 32);
			input->Read(&y, 32);
			SetPosition(x,y);
		}
		
		break;
		//TODO: start Machmaking case
	case Message_Protocol::NEWPLAYER:
		if (CheckSalts(input)) {
			SendAck(input);
		}
		break;
	case Message_Protocol::ENDR:
		if (CheckSalts(input)) {
			std::cout << "You've been disconnected for inactivity\n";
			onLoop = false;
		}
		break;
	case Message_Protocol::MESSAGE:
		input->ReadString(msg, 8);
		std::cout << msg << std::endl;
		break;
	case Message_Protocol::END:
		if (CheckSalts(input)) {
			onLoop = false;
			std::cout << "The server has disconnected\n";
		}
		break;

	case Message_Protocol::DISCONNECTED:
		if(CheckSalts(input))
			// TODO : enviar ACK
			// Recibir packetID
			std::cout << "A Client has disconnected\n";
		break;


	

	default:

		break;
	}
}
