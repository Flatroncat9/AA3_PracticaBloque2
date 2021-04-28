#pragma once
#include <SFML/Network.hpp>


typedef unsigned short Port;

struct ClientInfo {
    std::string ip;
    Port port;

    ClientInfo();
    ClientInfo(std::string _ip, Port _port) {
        ip = _ip;
        port = _port;
    }

    std::string to_string() {
        return (ip + ":" + std::to_string(port) ); 
    }
};

enum class Status
{
    Done,
    Error,
    NotReady
};

/*
struct Inputs {
    bool W, A, S, D;
    std::string to_string() {

    };
};
*/

static Status GetStatus(sf::Socket::Status _status)
{
    Status status;
    switch (_status)
    {
    case sf::Socket::Done:
        status = Status::Done;
        break;
    case sf::Socket::NotReady:
        status = Status::NotReady;
        break;
    case sf::Socket::Error:
        status = Status::Error;
        break;
    default:
        break;
    }
    return status;
}

enum class Message_Protocol
{
    HELLO,
    CH,
    CHR,
    WELCOME,
    NEWPLAYER
};

static std::string GetMessageProtocolFrom(Message_Protocol index)
{
    std::string str;

    int identifier = static_cast<int>(index);
    str = std::to_string(identifier) + "_";

    return str;
}

static Message_Protocol GetMessageProtocol(std::string index)
{
    return static_cast<Message_Protocol>(std::stoi(index));
}