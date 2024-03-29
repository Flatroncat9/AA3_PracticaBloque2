#pragma once
#include <SFML/Network.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>

#define H_NUM_TILES 60
#define W_NUM_TILES 80


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
    default:
        status = Status::Error;
        break;
    }
    return status;
}

enum class Message_Protocol
{
    HELLO,              // server <- client
    CH,                 // server -> client
    CHR,                // server <- client
    WELCOME,            // server -> client
    NEWPLAYER,          // server -> client
    DISCONNECTED,       // server <- client
    MOVE,               // server -> client
    OKMOVE,
    STARTMATCHMAKING,   // server <- client
    PING,               // server -> client
    PONG,               // server -> client
    MESSAGE,
    END,               // server -> client
    ENDR,
    ACK,
    AFK
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

static std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

struct Pos {
    int x, y;
    Pos() { x = 0; y = 0; }
    Pos(int _x, int _y) { x = _x; y = _y; }
};