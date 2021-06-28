#pragma once
#include <iostream>
#include <PlayerInfo.h>
#include <SFML\Network.hpp>
#include <UDPSocket.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <fcntl.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <thread>
#include "ClientManager.h"

#define SIZE 10.f
///TAMAÃ‘O EN PX DE LA VENTANA
#define W_WINDOW_PX 800
#define H_WINDOW_PX 600

///TAMAÃ‘O EN CELDAS DE LA VENTANA
#define W_WINDOW_TITLE 80
#define H_WINDOW_TITLE 60

///TAMAÃ‘O EN CELDAS DE UNA ZONA DE MAPA
#define W_WINDOW_TITLE_PART 40
#define H_WINDOW_TITLE_PART 30

///NUMERO DE ENEMIGOS Y DE OBSTACULOS QUE TIENEN QUE GENERARSE POR ZONA
#define NUM_ENEMIES 5
#define NUM_OBSTACLES_2x2 10

#define COUNTDOWN_CONNECT 30

//client
// send hello_ nik to the server until he recieves challenge
// when recive challenge responds with challengeResponse
// waits for welcome, but if he receive again the challenge he sends the challengeRespons again untel recieve Welcome_id
// when receive Welocome_id player sets is id and wait for other players

void SendMessage(ClientManager* thisClient) {
	std::string msg;

	while (true) {
		OutputMemoryBitStream oms;
		std::cout << "Escribe tu mensage\n";
		std::cin >> msg;

		if (msg == "exit" || msg == "EXIT") {
			oms.Write(static_cast<int>(Message_Protocol::END), 32);
			oms.Write(thisClient->serverInfo.ClientSalt, 32);
			oms.Write(thisClient->serverInfo.ServerSalt, 32);
			thisClient->sock.Send(oms, thisClient->serverInfo.IpServer, thisClient->serverInfo.PortServer);
			thisClient->onLoop = false;
			break;
		}
		oms.Write(static_cast<int>(Message_Protocol::MESSAGE), 32);
		oms.WriteString(msg, 8);
		thisClient->sock.Send(oms, thisClient->serverInfo.IpServer, thisClient->serverInfo.PortServer);
	}
}

void ReceiveMessages(ClientManager* thisClient) {
    // Receive messages
    while (thisClient->onLoop) {
        InputMemoryBitStream* input;

        Status s = thisClient->sock.Receive(input, thisClient->serverInfo.IpServer, thisClient->serverInfo.PortServer);
        if (s == Status::Done)
            thisClient->ManageMessageReceived(input, thisClient->serverInfo.IpServer, thisClient->serverInfo.PortServer);

    }
}

void SendMovement(ClientManager* thisClient) {
    while (thisClient->onLoop) {
        thisClient->SendAccum();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void DrawDungeon(ClientManager* thisClient)
{
    sf::RenderWindow _window(sf::VideoMode(800, 600), "Ventanita");
    sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.f);
    while (_window.isOpen() && thisClient->onLoop)
    {
        sf::Event event;
        bool playerMoved = false;
        while (_window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                _window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                {
                    _window.close();
                }
                if (event.key.code == sf::Keyboard::Left)
                {
                    std::cout << "LEFT\n";
                    thisClient->AddAccum(-1,0);
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    std::cout << "UP\n";
                    thisClient->AddAccum(0, -1);
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    std::cout << "RIGTH\n";
                    thisClient->AddAccum(1, 0);
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    std::cout << "DOWN\n";
                    thisClient->AddAccum(0, 1);
                }
                break;
            }
        }
        _window.clear();
        for (int i = 0; i < W_WINDOW_TITLE; i++)
        {
            for (int j = 0; j < H_WINDOW_TITLE; j++)
            {

                shape.setFillColor(sf::Color(90, 90, 90, 255));


                shape.setPosition(sf::Vector2f(i * SIZE, j * SIZE));
                _window.draw(shape);
            }
        }

        sf::Vector2f position;
        position.x = thisClient->serverInfo.x; position.y = thisClient->serverInfo.x;
        shape.setFillColor(sf::Color::Blue);
        shape.setFillColor(sf::Color(0, 0, 255, 255));
        shape.setPosition(sf::Vector2f(position.x * SIZE, position.y * SIZE));
        _window.draw(shape);


        shape.setFillColor(sf::Color::Green);
        shape.setFillColor(sf::Color(255, 255, 0, 255));
        for (int i = 0; i < thisClient->enemyPos.size(); i++) {
            position.x = thisClient->enemyPos[i].first; position.y = thisClient->enemyPos[i].second;
            shape.setPosition(sf::Vector2f(position.x * SIZE, position.y * SIZE));
            _window.draw(shape);
        }

        _window.display();
    }

}

int main()
{
	ClientManager* thisClient = new ClientManager();
	auto initProgram = std::chrono::system_clock::now();
	auto lastMessageSent = std::chrono::system_clock::now();
	// Until not verified and received Welcome, sends HELLO every 0.2s
	while(!thisClient->loggedIn) {
		std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - lastMessageSent;
		if (elapsed_seconds.count() > 0.2) {
			thisClient->SendHello();
			lastMessageSent = std::chrono::system_clock::now();
		}
		InputMemoryBitStream* input;

		Status s = thisClient->sock.Receive(input, thisClient->serverInfo.IpServer, thisClient->serverInfo.PortServer);
		if (s == Status::Done)
			thisClient->ManageMessageReceived(input, thisClient->serverInfo.IpServer, thisClient->serverInfo.PortServer);
		elapsed_seconds = std::chrono::system_clock::now() - initProgram;
		if (elapsed_seconds.count() > COUNTDOWN_CONNECT) {
			std::cout << "You cannot connect to the server, try again\n";
			return 1;
		}
	}

    // Threads
	std::thread t1(SendMessage, thisClient);
	t1.detach();
	
    std::thread t2(ReceiveMessages, thisClient);
    t2.detach();

    std::thread t3(SendMovement, thisClient);
    t2.detach();
	
    DrawDungeon(thisClient);
	return 0;
}

