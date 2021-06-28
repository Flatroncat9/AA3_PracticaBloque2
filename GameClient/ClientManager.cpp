#include "ClientManager.h"

ClientManager::ClientManager()
{
	srand(time(NULL));
	serverInfo.IpServer = SERVER_IP;
	serverInfo.PortServer = SERVER_PORT;
	serverInfo.isConnected = false;
	serverInfo.ClientSalt = rand() % UINT32_MAX;
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
		}
		
		break;
		//TODO: start Machmaking case
	case Message_Protocol::NEWPLAYER:

		break;
	case Message_Protocol::DISCONNECTED:
		if (CheckSalts(input)) {
			std::cout << "You've been disconnected for inactivity\n";
			onLoop = false;
		}
		break;
	case Message_Protocol::END:
		if (CheckSalts(input)) {
			onLoop = false;
			std::cout << "The server has disconnected\n";
		}
		break;

	case Message_Protocol::ENDR:
		if(CheckSalts(input))
			std::cout << "A Client has disconnected\n";
		break;

	case Message_Protocol::MESSAGE:
		input->ReadString(msg, 8);
		std::cout << msg << std::endl;
		break;

	default:

		break;
	}
}
