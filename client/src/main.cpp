#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "config.h"
#include "game.cpp"
using namespace std;

#define STEP_SIZE 2

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

	// start game here and let guy move around while waiting for an opponent
	// this means that, server side, as soon as an opponent is found the information exchange 
	// (name + pos) starts
	
	player opponent;
	opponent.pos = 0;
	socket.receive(packet);
	packet >> opponent.username >> client.side;
	opponent.side = !client.side;
	packet.clear();

	cout << "Opponent: " << opponent.username << endl;
	cout << "Side: " << client.side << endl;
	cout << "Opponent found. Starting game loop..." << endl;

	Game game(client, opponent, STEP_SIZE);
	socket.setBlocking(false);
	while(game.get_window()->isOpen()){
		sf::Event event;

		// TODO
		// resizing messes up a large portion of the game
		// since its ran client-side.
		// fixes: 
		// 	- handle game server side and just send information about it
		//  - [bandaid] dont allow resize
		//  - use percentages for size of paddle & ball and render based on wx & wy
		while(game.get_window()->pollEvent(event)){
			if(event.type == sf::Event::Closed){
				packet << -1;
				socket.send(packet);
				packet.clear();

				game.get_window()->close();
			}
			if (event.type == sf::Event::Resized){
        		sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        		game.get_window()->setView(sf::View(visibleArea));

			}

			// Paddle movement
			if (event.type == sf::Event::KeyPressed){
			  if(event.key.code == sf::Keyboard::Down && client.pos < 100 - (100 * (client.shape.getSize().y / game.get_window()->getSize().y)))
			    client.pos += STEP_SIZE;
			  if(event.key.code == sf::Keyboard::Up && client.pos > 0)
			    client.pos -= STEP_SIZE;
			}
		}

		packet << client.pos << ball.px << ball.py << ball.dx << ball.dy << client.score << opponent.score;
		socket.send(packet);
		packet.clear();

		if(opponent.pos >= 0 && socket.receive(packet) == sf::Socket::Done){
			float received_x, received_y, received_dx, received_dy;
			int received_client_score, received_opponent_score;
			packet >> opponent.pos >> received_x >> received_y >> received_dx >> received_dy >> received_opponent_score >> received_client_score;
			packet.clear();

			// this gives priority to the first connection, 
			// because in case of desync the ball position & velocity 
			// of the first connection is taken as right and given to 
			// the second connection
			if( (received_x != ball.px || received_y != ball.py || received_dx != ball.dx || received_dy != ball.dy) && client.side){
			  ball.px = received_x;
			  ball.py = received_y;
			  ball.dx = received_dx;
			  ball.dy = received_dy;
			}
			// same thing in case of desync with game score
			if ( ( received_client_score != client.score || received_opponent_score != opponent.score) && client.side){
			  client.score = received_client_score;
			  opponent.score = received_opponent_score;
			}
		}

		game.draw_text();
		game.update();
		game.display();
	}
}
