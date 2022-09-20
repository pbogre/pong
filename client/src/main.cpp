#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "config.h"
#include "game.hpp"
using namespace std;

int main(){
	cout << "Connecting TCP socket..." << endl
		 << "IP: " << cfg::IP << endl
		 << "PORT: " << cfg::PORT << endl;
		 
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(cfg::IP, cfg::PORT);
	if(status != sf::Socket::Done){
		cout << "Error while connecting socket!" << endl;
		return -1;
	}

	cout << "Connection accepted from host" << endl
		 << "Sending information to host" << endl;

	player client;
		client.username = cfg::username;
		client.pos = 0;

	sf::Packet packet;
	packet << client.username << client.pos;
	socket.send(packet);
	packet.clear();
	
	cout << "Setup information sent, waiting for opponent..." << endl;
	
	player opponent;
	opponent.pos = 0;
	socket.receive(packet);
	packet >> opponent.username >> client.side;
	opponent.side = !client.side;
	packet.clear();

	cout << "Opponent: " << opponent.username << endl;
	cout << "Side: " << client.side << endl;
	cout << "Opponent found. Starting game loop..." << endl;

	sf::RenderWindow window(sf::VideoMode(1300, 800), "pong");
	window.setFramerateLimit(30);
	while(window.isOpen()){
		sf::Event event;

		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized){
        		sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        		window.setView(sf::View(visibleArea));
    		}

			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Up && client.pos > 0) client.pos -= 4;
				if(event.key.code == sf::Keyboard::Down && client.pos < 60) client.pos += 4;
			}
		}

		window.clear(sf::Color::Black);

		packet << client.pos << ball.px << ball.py;
		socket.send(packet);
		packet.clear();

		if(opponent.pos >= 0){
			socket.receive(packet);
			packet >> opponent.pos >> ball.px >> ball.py;
			packet.clear();
		}

		draw_score();
		update(window, client, opponent);
		cout << client.score << " | " << opponent.score << endl;

		window.display();
	}

	// start game here and let guy move around while waiting for an opponent
	// this means that, server side, as soon as an opponent is found the information exchange 
	// (name + pos) starts
}
